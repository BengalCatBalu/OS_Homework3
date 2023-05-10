#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Использование: %s <IP-адрес> <порт>\n", argv[0]);
        return 1;
    }

    char *ip_address = argv[1];  // Получаем IP-адрес из аргументов командной строки
    int port = atoi(argv[2]);    // Конвертируем номер порта из аргументов командной строки

// Создание сокета
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

// Параметры адреса для сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_address, &server_addr.sin_addr);

// Устанавливаем соединение с сервером
    connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    while (1) {
        // Получаем номер сектора от сервера
        int sector;
        recv(client_fd, &sector, sizeof(int), 0);

        // Если полученный номер сектора равен -1, завершаем работу
        if (sector == -1) {
            printf("Все сектора уже исследованы, результат работы посмотрите в терминале сервера");
            break;
        }

        printf("Исследование сектора %d...\n", sector);
        sleep(1);

        // С вероятностью 1/2 считаем, что нашли Винни-Пуха
        int found = (rand() % 2);

        // Отправляем результат поиска серверу
        send(client_fd, &found, sizeof(int), 0);

        // Если нашли Винни-Пуха, завершаем работу
        if (found == 0) {
            printf("Винни-Пух найден в секторе %d!\n", sector);
            break;
        } else {
            printf("Винни-Пух не найден в секторе %d\n", sector);
        }
    }

// Закрываем сокет клиента
    close(client_fd);
    return 0;
}
