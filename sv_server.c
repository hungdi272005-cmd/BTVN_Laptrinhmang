#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct Student {
    char mssv[20];
    char name[50];
    char dob[20];
    float gpa;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <PORT>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    // Tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Fix lỗi bind bị chiếm port
    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    listen(listener, 5);
    printf("Server listening on port %d...\n", port);

    while (1) {
        int client = accept(listener, NULL, NULL);
        if (client < 0) {
            perror("accept failed");
            continue;
        }

        printf("\nClient connected!\n");

        struct Student sv;
        int ret = recv(client, &sv, sizeof(sv), 0);

        if (ret > 0) {
            printf("=== Thong tin sinh vien ===\n");
            printf("MSSV: %s\n", sv.mssv);
            printf("Ho ten: %s\n", sv.name);
            printf("Ngay sinh: %s\n", sv.dob);
            printf("GPA: %.2f\n", sv.gpa);
        }

        close(client);
        printf("Client disconnected!\n");
    }

    close(listener);
    return 0;
}