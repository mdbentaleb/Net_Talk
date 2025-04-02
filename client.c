#include "client_server.h"

#define TIMEOUT 5

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    fd_set write_fds;
    struct timeval timeout;
    int ret;

    if (argc != 3)
    {
        printf("❌ Usage: %s <server IP address> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("❌ Failed to create socket");
        exit(EXIT_FAILURE);
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
    {
        perror("❌ Invalid or unsupported IP address");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ret = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    if (ret < 0) {
        if (errno == EINPROGRESS) {
            printf("⏳ Waiting for connection...\n");
            
            timeout.tv_sec = TIMEOUT;
            timeout.tv_usec = 0;

            FD_ZERO(&write_fds);
            FD_SET(sock, &write_fds);

            ret = select(sock + 1, NULL, &write_fds, NULL, &timeout);
            if (ret == 0) {
                perror("❌ Connection timed out");
                close(sock);
                exit(EXIT_FAILURE);
            } else if (ret < 0) {
                perror("❌ select() error");
                close(sock);
                exit(EXIT_FAILURE);
            } else {
                printf("✅ Connection established successfully!\n");
            }
        } else {
            perror("❌ Failed to connect to the server");
            close(sock);
            exit(EXIT_FAILURE);
        }
    }

    send(sock, argv[2], strlen(argv[2]), 0);
    printf("✅ Message sent: %s\n", argv[2]);

    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0)
    {
        printf("📩 Server response: %s\n", buffer);
    }
    else
        printf("⚠️ No response received from the server.\n");

    close(sock);
    return 0;
}
