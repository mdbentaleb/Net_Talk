#include "client_server.h"

void get_server_ip() {
    char hostbuffer[256];
    struct hostent *host_entry;
    char *IPbuffer;

    if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1)
    {
        perror("Failed to get host name");
        return;
    }

    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL)
    {
        perror("Failed to get host information");
        return;
    }

    IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    printf("🔹 Server IP address: %s\n", IPbuffer);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("❌ Failed to create socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("❌ Failed to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("❌ Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("✅ Server is running and waiting for connections on port %d...\n", PORT);
    get_server_ip();

    while (1) {
        printf("\n📡 Waiting for a new connection...\n");

        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("❌ Failed to accept connection");
            continue;
        }

        printf("🔗 New connection from: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);
        printf("📩 Message received: %s\n", buffer);

        char *response = "Your message has been received ✅";
        send(new_socket, response, strlen(response), 0);

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
