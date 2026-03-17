#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

struct Student {
    char mssv[20];
    char name[50];
    char dob[20];
    float gpa;
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <PORT> <LOG_FILE>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    char *logfile = argv[2];

    int listener = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    listen(listener, 5);
    printf("Server listening on port %d...\n", port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int client = accept(listener, (struct sockaddr *)&client_addr, &addr_len);
        if (client < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected!\n");

        // Lấy IP
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));

        struct Student sv;

        //  nhận nhiều lần từ 1 client
        while (1) {
            int ret = recv(client, &sv, sizeof(sv), 0);

            if (ret <= 0) {
                printf("Client disconnected!\n\n");
                break;
            }

            // Lấy thời gian
            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            char time_str[64];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

            // In ra màn hình
            printf("IP: %s\n", ip);
            printf("Time: %s\n", time_str);
            printf("MSSV: %s\n", sv.mssv);
            printf("Name: %s\n", sv.name);
            printf("DOB: %s\n", sv.dob);
            printf("GPA: %.2f\n\n", sv.gpa);

            // Ghi file
            FILE *f = fopen(logfile, "a");
            if (f != NULL) {
                fprintf(f, "%s %s %s %s %s %.2f\n",
                        ip, time_str,
                        sv.mssv,
                        sv.name,
                        sv.dob,
                        sv.gpa);
                fclose(f);
            }
        }

        close(client);
    }

    close(listener);
    return 0;
}