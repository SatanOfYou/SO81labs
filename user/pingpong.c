#include "kernel/types.h"
#include "user/user.h"
int main(int argc,char**argv){
    if(argc!=1){
        printf("argument amount error!\n");
	exit(-1);
    }
    int p[2],pid;
    char buf[2]={0,0};
    if(pipe(p)){
        printf("parent process pipe failed!\n");
	exit(-1);
    }
    if(write(p[1],"pipe",1)!=1){
        printf("parent process write pipe failed\n");
	close(p[0]);
	close(p[1]);
	exit(-1);
    }
    pid=fork();
    if(pid==0){
        if(read(p[0],buf,1)!=1||write(p[1],"pipe",1)!=1){
	    printf("child read error!\n");
	    close(p[0]);
	    close(p[1]);
	    exit(-1);
	}
        printf("%d: received ping\n",getpid());
        close(p[0]);
	close(p[1]);
	exit(0);
    }else{
        wait((int*)0);
        if(read(p[0],buf+1,1)!=1){
	    printf("parent read error\n");
	    close(p[0]);
	    close(p[1]);
	    exit(-1);
	}
	printf("%d: received pong\n",getpid());
	exit(0);
    }
}
