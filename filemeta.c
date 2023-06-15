#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <stdio.h>    // BUFSIZ
#include <sys/sendfile.h>//sendfile
#include <string.h> 
#include <errno.h>

// long get_file_size(char *filename) {
//     struct stat file_status;
//     if (stat(filename, &file_status) < 0) {
//         return -1;
//     }

//     return file_status.st_size;
// }

int main() {
    char dirname[512]="./data";
    // char dirname[512]=".";
    DIR *dr;
    struct dirent *en;
    dr = opendir(dirname); //open all directory
    strcpy(dirname+strlen(dirname),"/");
    char * dirnameptr=dirname+strlen(dirname);
    
    int dest = open("meta.out", O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0666);
    int src;
    long offset=0;
    // sendfile(dest,source);
    // char * cptr=name;
    // cptr+=sizeof(char)*strlen(name);

    struct stat file_status;
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
            memcpy(dirnameptr,en->d_name,strlen(en->d_name)+1);
            // printf("dirname %s\n",dirname);
            int tmp=(stat(dirname, &file_status));
            if ((tmp < 0)||(S_ISREG(file_status.st_mode)==0)) {//check if it is valid file
                // printf("what %s  error %d eeeeeee %d\n",en->d_name,S_ISREG(file_status.st_mode),errno);
                continue;
            }
            src = open(dirname, O_RDONLY, 0);
            // sendfile(dest,src,NULL,file_status.st_size);
            offset+=file_status.st_size;
            printf("%s  size : %d byte\n",en->d_name,file_status.st_size);
            tmp=strlen(en->d_name);
            int tmplong=file_status.st_size;
            write(dest,&tmp,sizeof(int));
            write(dest,&tmplong,sizeof(long));
            // write(dest,en->d_name,strlen(en->d_name));
            
        }
        closedir(dr); //close all directory
    }
    close(dest);

    // sendfile(dest,src,NULL,97962);
    // close(dest);
    // dest=open("newb.txt", O_WRONLY | O_CREAT ,0666);
    // sendfile(dest,src,NULL,20);



    int t=open("meta.out",O_RDONLY, 0);
    char buf[1024];
    read(t,buf,1024);
    printf("start\n");
    printf("%X\n",buf);
    return(0);
}

