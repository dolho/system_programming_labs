#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>

int main(void) {
    int fd;
    pid_t fork_id;
    

    fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
      //printf("Error: %s\n", strerror(errno));
      return -1;
    }
    write(fd, "Start\n", strlen("Start\n"));

    fork_id = fork();
    if (fork_id < 0) {
        write(fd, "Fork error ", strlen("Fork error "));
        write(fd, strcat(strerror(errno), '\n') , strlen(strcat(strerror(errno), '\n')));
        close(fd);
        exit(1);
    }
    if (fork_id > 0) {
        write(fd, "Close parent\n", strlen("Close parent\n"));
        close(fd);
        exit(0);
    }
    else {
        struct sockaddr_in socket_in;
        int my_variant = 10;
        socket_in.sin_family = PF_INET;
        socket_in.sin_port = htons(3200 + my_variant);
        socket_in.sin_addr.s_addr = htonl(INADDR_ANY);
        int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            write(fd, "Socket error ", strlen("Socket error "));
            write(fd, strcat(strerror(errno), '\n') , strlen(strcat(strerror(errno), '\n')));
            close(fd);
            exit(1);
        }
        int bind_result = bind(socket_fd, (struct sockaddr *)&socket_in, sizeof(socket_in));
        if (bind_result == -1) {
            write(fd, "Error while binding ", strlen("Error while binding "));
            write(fd, strcat(strerror(errno), '\n') , strlen(strcat(strerror(errno), '\n')));
            close(fd);
            exit(1);
        }
        char string[1024];
        sprintf(string, "(%d) Listening\n", getpid());
        write(fd, string, strlen(string));

        listen(socket_fd, 5);

        struct sockaddr_in client_socket_in;
        int client_socket_fd;
        pid_t worker_fork_id;
        int client_socket_size = sizeof(client_socket_in);
        while (1)
        {
            client_socket_fd = accept(socket_fd, (struct sockaddr *)&client_socket_in, &client_socket_size);

            if (client_socket_fd == -1)
            {
                write(fd, strcat(strerror(errno), '\n'), strlen(strcat(strerror(errno), '\n')));
                write(fd, "Error while connecting ", strlen("Error while connecting "));
                
            }
            else
            {
                worker_fork_id = fork();
                if (fork_id == -1)
                {
                    write(fd, "Fork error ", strlen("Fork error "));
                    write(fd, strcat(strerror(errno), '\n'), strlen(strcat(strerror(errno), '\n')));
                    close(fd);
                }
                else if (fork_id > 0)
                {
                    close(client_socket_fd);
                }
                else
                {
                    close(socket_fd);
                    char recv_buffer[1024];
                    int recv_bytes;
                    char send_buffer[1024];
                    int send_bytes;
                    time_t rawtime;
                    struct tm cur_time;

                    while (1)
                    {
                        recv_bytes = recv(client_socket_fd, recv_buffer, sizeof(recv_buffer), 0);

                        time(&rawtime);
                        cur_time = (*localtime(&rawtime));
                        send_bytes = sprintf(send_buffer, "(%d) %s%s\n\n", getpid(), asctime(&cur_time), recv_buffer);
                        //send_buffer[send_bytes - 1] = '\0';
                        write(fd, send_buffer, strlen(send_buffer));
                        if (send(client_socket_fd, send_buffer, send_bytes, 0) == -1)
                        {
                            write(fd, "Send error ", strlen("Send error "));
                            write(fd, strcat(strerror(errno), '\n'), strlen(strcat(strerror(errno), '\n')));
                            close(fd);
                        }

                        if (strcmp(recv_buffer, "close") == 0)
                            break;
                        recv_buffer[0] = '\0';
                        send_buffer[0] = '\0';
                    }

                    write(fd, "Close\n", strlen("Close\n"));
                    close(client_socket_fd);
                    close(fd);
                    exit(0);
                }
            }
        }
    }
    close(fd);
    printf("Finished\n");
    return 0;
}