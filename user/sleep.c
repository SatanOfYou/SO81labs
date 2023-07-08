#include "kernel/types.h"
#include "user/user.h"
int main(int argc,char**argv){
    if(argc!=2){
        printf("argument amount error!\n");  
	exit(-1);
    }
    int time=atoi(argv[1]);
    if(sleep(time)){
        printf("sleep system call error!\n");
	exit(-1);
    }
    exit(0);
}
