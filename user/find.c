#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(const char*path,const char*subStr){
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st; 

    if((fd=open(path,0))<0){
        fprintf(2,"find: can't open: %s\n",path);
	return;
    }
    if(fstat(fd,&st)<0){
        fprintf(2,"find can't stat %s\n",path);
	close(fd);
        return;
    }
    if(strstr(path,subStr))printf("%s\n",path);
    switch(st.type){
	case T_DIR:
	    if(strlen(path)+1+DIRSIZ+1 > sizeof(buf)){
	        printf("find: path too long!\n");
		break;
	    }
	    strcpy(buf,path);
	    p=buf+strlen(path);
	    *p++='/';
	    while(read(fd,&de,sizeof(de))==sizeof(de)){
	        if(de.inum==0)continue;
		if((strcmp(".",de.name)==0)||(strcmp("..",de.name)==0))continue;
		memmove(p,de.name,DIRSIZ);
		p[DIRSIZ]='\0';
                find(buf,subStr);
	    }
    }
    close(fd);
}
int main(int argc,char**argv){
   if(argc!=3){
       printf("argument amount error!\n");
       exit(-1);
   } 
   find(argv[1],argv[2]);
   exit(0);
}
