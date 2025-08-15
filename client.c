#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Configuration du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Liaison du socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, 3) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    // Acceptation d'un client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Erreur accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connecté !\n");

    // Boucle de communication
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(new_socket, buffer, BUFFER_SIZE);

        if (bytes_read <= 0) {
            printf("Client déconnecté.\n");
            break;
        }

        printf("Client : %s\n", buffer);

        // Vérifier si le client a envoyé "bye"
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Fermeture de la connexion...\n");
            break;
        }

        // Répondre au client
        printf("Serveur : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Supprimer le saut de ligne
        send(new_socket, buffer, strlen(buffer), 0);

        // Vérifier si le serveur a envoyé "bye"
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Fermeture de la connexion...\n");
            break;
        }
    }

    // Fermeture des sockets
    close(new_socket);
    close(server_fd);

    return 0;
}