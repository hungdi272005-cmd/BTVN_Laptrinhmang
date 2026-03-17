#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Kiểm tra tham số
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    // Tạo socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("socket() failed");
        return 1;
    }

    // Khai báo địa chỉ server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(server_ip);

    // Kết nối
    if (connect(client, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect() failed");
        return 1;
    }

    printf("Connected to server!\n");

    char buf[1024];

    while (1) {
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);

        // Gửi dữ liệu
        send(client, buf, strlen(buf), 0);

        // Nếu nhập "exit" thì thoát
        if (strncmp(buf, "exit", 4) == 0)
            break;
    }

    close(client);
    return 0;
}