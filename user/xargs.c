#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc,char**argv){
    if(argc<=1){
        printf("argument amount error!\n");
	exit(-1);
    }
    int argNum=MAXARG,index=0,count=0,pid,subPid;
    char argContent[MAXARG][MAXARG];
    char *argArr[MAXARG];
    char buf;
    if(!strcmp("-n",argv[1])){
        argNum=atoi(argv[2]);
	for(int i=3;i<argc;i++)strcpy(argContent[count++],argv[i]);
    }else for(int i=1;i<argc;i++)strcpy(argContent[count++],argv[i]);
    while((read(0,&buf,1)==1)){
        if(buf==' '||buf=='\n'){
	    argContent[count][index]='\0';
	    count++;
	    index=0;
	}else{
	    if(index>=MAXARG||count>=MAXARG){
	        printf("input argument length or amount error\n");
		exit(-1);
	    }
	    argContent[count][index++]=buf;
	}
    }
    for(int i=0;i<count;i++)argArr[i]=argContent[i];
    if(argNum>=count){
        printf("-n option error!\n");
	exit(-1);
    }
    argArr[count]=0;
    pid=fork();
    if(pid==0){
        if(argNum==MAXARG){
            exec(argArr[0],argArr);
        }else{
	    index=0;
	    while(index<count-1){
                for(int i=1;i<=argNum;i++){
	            argArr[i]=argContent[index+i];
	        }
	        argArr[argNum+1]=0;
		subPid=fork();
		if(subPid==0)exec(argArr[0],argArr);
		else wait((int*)0);
	        index+=argNum;
	    }
	}
	exit(0);
    }else{
        wait((int*)0);
        exit(0);
    }
}
