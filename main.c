#include <stdio.h>
#include <string.h> 

int main(){
    printf("HELLL\n");
    
    char name[64]="./data";//max는 4096인데
    char file[256]="file2";
    char file2[256]="file3323";
    printf("%s %d\n",name,strlen(name));
    strcpy(name+strlen(name),"/");
    printf("%s %d\n",name,strlen(name));
    
    // memcpy(name,file,sizeof(char)*strlen(file));
    char * nameptr=name+strlen(name);
    memcpy(nameptr,file,strlen(file)+1);
    printf("%sx\n",name);
    memcpy(nameptr,file2,strlen(file2)+1);
    printf("%sx\n",name);
}