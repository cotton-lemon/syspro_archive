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

void wrong();
void pack();
void unpack();
void add();
void del();
void list();
// void test(ch)
int main(int argc,char* argv[]){
    int a=open("123.txt",O_WRONLY,0666);
    int b=open("11.txt",O_RDONLY);
    int q;
    q=lseek(a,3,0);
    // printf("%d\n",q);
    printf("error %d\n",errno);
    q=sendfile(a,b,NULL,10);
    // printf("%d\n",q);
    printf("error %d\n",errno);
}