#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <порт> <количество_секторов>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);  // Конвертируем номер порта из аргументов командной строки
    int sectors = atoi(argv[2]);  // Конвертируем количество секторов из аргументов командной строки
    int explored_sectors = 0;  // Счётчик исследованных секторов

    // Создание сокета
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Параметры адреса для сокета
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Привязка сокета к IP-адресу и порту
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Устанавливаем сокет на прослушивание
    listen(server_fd, 10);

    while (1) {
        // Принимаем соединение от клиента
        int client_fd = accept(server_fd, NULL, NULL);

        // Если все секторы исследованы, отправляем -1 клиенту и закрываем соединение
        if (explored_sectors >= sectors) {
            int end_of_sectors = -1;
            send(client_fd, &end_of_sectors, sizeof(int), 0);
            close(client_fd);
            printf("Винни-Пуха так нигде и не нашли, он спрятался.... Начнем поиск сначала");
            explored_sectors = 0;
            continue;
        }

        // Отправляем номер сектора клиенту
        send(client_fd, &explored_sectors, sizeof(int), 0);

        // Получаем результат поиска от клиента
        int found;
        recv(client_fd, &found, sizeof(int), 0);

        // Если клиент нашел Винни-Пуха, завершаем работу
        if (found) {
            printf("Винни-Пух найден в секторе %d!!!!!\n", explored_sectors);
            break;
        } else {
            printf("Винни-Пух не найден в секторе %d\n", explored_sectors);
        }

        // Увеличиваем счетчик исследованных секторов
        explored_sectors++;

        // Закрываем соединение с клиентом
        close(client_fd);
    }

    // Закрываем сокет сервера
    close(server_fd);
    return 0;
}

