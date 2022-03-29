#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char * path, char * target){
    char buf[64], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //step1:打开path路径
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    //step2:获取path路径目录的元数据
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s", path);
        return;
    }

    switch(st.type){
    case T_FILE:
        fprintf(2, "find: path:%s is not dir\n", path);
        break;
    //step3:path路径目录
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
            fprintf(2, "find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        //step4:循环遍历fd对应的目录下面的条目
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if(stat(buf, &st) < 0){
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }

            if(T_DIR == st.type){
                if(0 == strcmp(".", de.name) || 0 == strcmp("..", de.name))
                    continue;
                find(buf, target);
                continue;
            } else if(T_FILE == st.type){
                if(0 == strcmp(de.name, target))
                    printf("%s\n", buf);
            }
        }
        break;
    }
    close(fd);
}


int main(int argc, char * argv[]){

    if(argc != 3){
        fprintf(2, "find path target\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
