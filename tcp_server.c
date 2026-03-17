#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <port> <welcome_file> <output_file>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    char *welcome_file = argv[2];
    char *output_file = argv[3];

    // Đọc file chào
    FILE *fw = fopen(welcome_file, "rb");
    if (fw == NULL) {
        perror("Cannot open welcome file");
        return 1;
    }

    fseek(fw, 0, SEEK_END);
    long size = ftell(fw);
    fseek(fw, 0, SEEK_SET);

    char *welcome_msg = malloc(size);
    fread(welcome_msg, 1, size, fw);
    fclose(fw);

    // Tạo socket
    int listener = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind() failed");
        return 1;
    }

    listen(listener, 5);
    printf("Server listening on port %d...\n", port);

    while (1) {
        int client = accept(listener, NULL, NULL);
        if (client < 0) {
            perror("accept() failed");
            continue;
        }

        printf("Client connected!\n");

        // Gửi câu chào
        send(client, welcome_msg, size, 0);

        // Mở file ghi
        FILE *fo = fopen(output_file, "ab"); // append
        if (fo == NULL) {
            perror("Cannot open output file");
            close(client);
            continue;
        }

        char buf[1024];
        int len;

        // Nhận dữ liệu từ client
        while ((len = recv(client, buf, sizeof(buf), 0)) > 0) {
            fwrite(buf, 1, len, fo);
            fflush(fo);
        }

        fclose(fo);
        close(client);
        printf("Client disconnected!\n");
    }

    free(welcome_msg);
    close(listener);
    return 0;
}