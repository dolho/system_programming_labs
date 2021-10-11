#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Need identificator\n");
        exit(1);
    }
    char *id = argv[1];
    fd_set rfds;
    struct timeval tv;
    int retval; 
    char buf[1024];

    while(1) {
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            printf("Error: %s\n", strerror(errno));
        }
        else if (retval)
        {
            if (FD_ISSET(STDIN_FILENO, &rfds)) {
                if (fgets(buf,sizeof(buf), stdin)) 
                    printf("%s: %s\n",id ,buf);
                else
                    printf("Error: %s\n", strerror(errno));
            }
        }
        else
        {
            printf("Timeout\n");
        }
    }
    return 0;
}