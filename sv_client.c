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
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    int client = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        return 1;
    }

    struct Student sv;

    while (1) {
        printf("\n=== Nhap thong tin sinh vien ===\n");

        printf("Nhap MSSV (go 'exit' de thoat): ");
        fgets(sv.mssv, sizeof(sv.mssv), stdin);
        sv.mssv[strcspn(sv.mssv, "\n")] = 0;

        //  thoát
        if (strcmp(sv.mssv, "exit") == 0) {
            break;
        }

        printf("Nhap ho ten: ");
        fgets(sv.name, sizeof(sv.name), stdin);
        sv.name[strcspn(sv.name, "\n")] = 0;

        printf("Nhap ngay sinh: ");
        fgets(sv.dob, sizeof(sv.dob), stdin);
        sv.dob[strcspn(sv.dob, "\n")] = 0;

        printf("Nhap GPA: ");
        scanf("%f", &sv.gpa);
        getchar(); //  ăn newline còn sót

        // Gửi
        send(client, &sv, sizeof(sv), 0);

        printf("Da gui!\n");
    }

    close(client);
    return 0;
}