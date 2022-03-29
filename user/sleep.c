#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(2, "sleep need 2 args.\n");
        exit(1);
    }

    sleep(atoi(argv[1]));
    exit(0);
}


