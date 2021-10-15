#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t child = fork();
    if (child == 0) {
        printf("Child process:\n");
        printf("Pid: %d\n", getpid());
        printf("Ppid: %d\n", getppid());
        printf("Gid: %d\n", getgid());
        printf("Egid: %d\n", getegid());
        printf("Sid: %d\n", getsid(0));
        printf("Uid: %d\n", getuid());
        printf("Euid: %d\n", geteuid());
        // while(1) {
        //     sleep(1);
        //     printf("Child process: PID: %i \n", getpid());
        // }
        printf("Stoped child process\n");
    } else {
        printf("Parent process:\n");
        printf("Pid: %d\n", getpid());
        printf("Ppid: %d\n", getppid());
        printf("Gid: %d\n", getgid());
        printf("Egid: %d\n", getegid());
        printf("Sid: %d\n", getsid(0));
        printf("Uid: %d\n", getuid());
        printf("Euid: %d\n", geteuid());
        printf("Child process pid: %i\n", child);
        //  while(1) {
        //     sleep(1);
        //     printf("Parent process: PID: %i \n", getpid());
        // }
        wait(NULL);
        printf("Stoped parent process\n");
    }
    return 0;
}