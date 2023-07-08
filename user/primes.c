#include "kernel/types.h"
#include "user/user.h"
int primes(int*p){
    int pd[2],pid;
    unsigned char buf[2];
    if(read(p[0],buf,1)!=1)exit(0);
    printf("prime %d\n",buf[0]);
    if(read(p[0],buf+1,1)!=1)exit(0);
    if(pipe(pd)){
        printf("child: %d pipe failed\n",getpid());
	close(p[0]);
	close(p[1]);
	exit(-1);
    }
    do{
        if(buf[1]%buf[0]!=0 && (write(pd[1],buf+1,1)!=1)){
	    close(pd[0]);
	    close(pd[1]);
	    close(p[0]);
	    exit(-1);
	}
    }while(read(p[0],buf+1,1)!=0);
    close(p[0]);
    close(pd[1]);
    pid=fork();
    if(pid==0){
        primes(pd);
	exit(0);
    }else{
        wait((int*)0);
	close(pd[0]);
	exit(0);
    }
}
int main(int argc,char**argv){
    if(argc!=1){
        printf("argument amount error\n");
	exit(-1);
    }
    int p[2],pid;
    if(pipe(p)){
        printf("%d: pipe failed\n",getpid());
	exit(-1);
    }
    for(unsigned char i=2;i<=35;i++){
        if(write(p[1],&i,1)!=1){
	    printf("write 2-35 error\n");
	    close(p[0]);
	    close(p[1]);
	    exit(-1);
	}
    }
    pid=fork();
    close(p[1]);
    if(pid==0){
	if(primes(p))exit(-1);
	else exit(0);
    }else{
        wait((int*)0);
        close(p[0]);
	exit(0);
    }
}
