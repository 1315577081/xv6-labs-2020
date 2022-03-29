#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[]){
    int fds0[2];
    int fds1[1];
    int pid;
    char buf = '6';


    if(pipe(fds0) != 0 && pipe(fds1) != 0){
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    pid = fork();
    if(pid == 0){
        close(fds0[1]);
        close(fds1[0]);
        read(fds0[0], &buf, sizeof(buf));
        fprintf(1, "%d: received ping\n", getpid());
        write(fds1[1], &buf, sizeof(buf));
    } else if(pid > 0){
        close(fds0[0]);
        close(fds1[1]);
        write(fds0[1], &buf, sizeof(buf));
        sleep(1);
        read(fds1[0], &buf, sizeof(buf));
        fprintf(1, "%d: received pong\n", getpid());
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
}
