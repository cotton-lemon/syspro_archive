// #include <a.h>
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
    for (int q=0;q<100;++q){
        char d[1024*1024];
        for (int z=0;z<1024*1024;++z){
            d[z]=rand()/256;
        }
        char buff[256];            // 변환될 int형 변수를 저장할 공간
        int x = q;                    // 변환 시킬 int형 변수
        sprintf(buff,"%d",q); 
        int r= open(buff,O_WRONLY|O_CREAT,0666);
        write(r,d,1024*1024);
    }
}