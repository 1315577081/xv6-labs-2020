#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


int main(int argc, char * argv[])
{
	char buf[32];
	
	char xbuf[32];
	int ixb = 0;
	char *pxbuf = xbuf;

	char * lineSplit[32];
	int iLineSplit = 0;

	int n = 0;
	int i;
	
	for(i = 1; i < argc; i++){
		lineSplit[iLineSplit++] = argv[i];
		//printf("%d: %s \n", i, argv[i]);
	}

	while((n = read(0, buf, sizeof(buf))) > 0){
		for(i = 0; i < n; i++){
			if(buf[i] == '\n'){
				xbuf[ixb] = 0;
				ixb = 0;
				lineSplit[iLineSplit++] = pxbuf;
				pxbuf = xbuf;
				lineSplit[iLineSplit] = 0;
				iLineSplit = argc - 1;
				if(fork() == 0){
					exec(argv[1], lineSplit);
				}
				wait(0);

			} else if (buf[i] == ' '){
				xbuf[ixb++] = 0;
				lineSplit[iLineSplit++] = pxbuf;
				pxbuf = &xbuf[ixb];
			} else{
				xbuf[ixb++] = buf[i];
			}
		}
	}
	exit(0);
}
