#include "kernel/types.h"
#include "user/user.h"

#define N 35


void forkAndprint(int p, int * fds){
    int n, i, pid;
    int fds1[2];
    int firstFork = 1;

    printf("prime %d \n", p);
    while((n = read(fds[0], &i, sizeof(int))) > 0){
        if(i % p==0)
            continue;
        if(1 == firstFork){
            firstFork = 0;
            pipe(fds1);
            pid = fork();
        }
        if(pid > 0){            //this is parent
            close(fds1[0]);
            write(fds1[1], &i, sizeof(int));
        } else if(pid == 0) {   //this is child
            close(fds1[1]);
            forkAndprint(i, fds1);
            wait(0);
            exit(0);
        }
    }
    close(fds1[1]);
}

int main(int argc, char * argv[]){
    int fds[2];
    int i, pid;

    pipe(fds);
    pid = fork();
    if (pid == 0){      // this is child process
        close(fds[1]);
        forkAndprint(2, fds);
        wait(0);
        exit(0);
    } else if(pid > 0) {// this is parent process
        close(fds[0]);
        for(i = 2; i <= N; i++){
            write(fds[1], &i, sizeof(int));
        }
        close(fds[1]);
        wait(0);
        exit(0);
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
}


