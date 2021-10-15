#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("No identificator given\n");
        exit(1);
    }
    char *id = argv[1];
    const unsigned int buffer_length = 1024;
    char buffer[buffer_length];
    
    struct timeval timeout_setting;
    fd_set read_file_descriptors;

    while(1) {
        FD_ZERO(&read_file_descriptors);
        FD_SET(STDIN_FILENO, &read_file_descriptors);

        timeout_setting.tv_sec = 5;
        timeout_setting.tv_usec = 0;
        
        
        int select_status = select(STDIN_FILENO + 1, &read_file_descriptors, NULL, NULL, &timeout_setting);
        if (select_status == -1)
        {
            fprintf(stderr, "Error: %s\n", strerror(errno));
        }
        else if (select_status == 0)
        {
            fprintf(stderr, "Timeout\n");
        }
        else
        {
            if (fgets(buffer, buffer_length, stdin))
                printf("%s: %s\n", id, buffer);
            else
                printf("Error: %s\n", strerror(errno));
        }
    }
}