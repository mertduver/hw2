#include<stdio.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include<time.h>
#include <netinet/in.h>
#include <sys/types.h>

#define BUFSIZE 1024 // Buffer Size

time_t currentTime;
char myString[35];

int main(int argc, char *argv[]) {
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char *message, server_reply[3000];

    int rfd; // Create-Descriptor
    int cfd; // Connection-Descriptor (accept)


    int bytes_read;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        puts("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8889);

    // Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");
    printf("listening to localhost:8889\n");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    while (new_socket =
                   accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c)) {
        puts("Connection accepted");
        sleep(1);
        message = "Hello client, I received your connection. See you\n\n";

        char in[BUFSIZE];
        // read a line from a socket (Client's data)
        int bytes_idx = -1;
        while (1) {

            if (bytes_idx >= (int)sizeof(in)) {
                fprintf(stderr, "input buffer overflow\n");
                break;
            }
            // Receive on byte (character) at a time
            bytes_read = recv(new_socket, &in[++bytes_idx], 1, 0);
            if (bytes_read <= 0)  // Check error or no data read
                break;
            /*
                            printf("sending back the %d bytes I received...\n", bytes_read);
                            if (send(cfd, &in[bytes_idx], 1, 0) <= 0) {
                                fprintf(stderr, "send() failed with error %d\n", WSAGetLastError());
                                exit(1);
                            }
            */
            if (in[bytes_idx] == '\n') {
                // Received a complete line, including CRLF
                // Remove ending CR
                bytes_idx--;
                if ((bytes_idx >= 0) && (in[bytes_idx] == '\r'))
                    in[bytes_idx] = 0;
                break;
            }
        }
        if (bytes_idx > 0) {   // Check for empty line
            printf("Received \"%s\"\n", in);
            // Check for client command
            if (strcmp(in, "exit") == 0)
                break;
            else {
//                printf("Client sent unknown command\n");

                currentTime = time(NULL);
                strcpy(myString, ctime(&currentTime));


                puts("date sending\n");
                message = myString;
                write(new_socket, message, strlen(message));
                puts("date sended\n");
            }
        }




        close(socket_desc);

    }

    return 0;
}

