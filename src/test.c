#include <stdio.h>

#include <stdio.h>
#include <string.h> 
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <sys/sendfile.h>//sendfile
#include <errno.h>
#include <stdlib.h>

int main(){
    mkdir("small1",0766);
    int file=open("small1/q.txt",O_WRONLY | O_CREAT,0666);
    // char buf[128];
    // int result=read(file,buf,128);
    // printf("result %d\n",result);
    // for(int i;i<20;++i){
    //     printf("%c\n",buf[i]);
    // }
    if (file ==-1){
        printf("file openfail\n");
        exit(2);
    }
    char buf[129]="11215125\n";
    write(file,buf,129);
}