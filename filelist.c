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
void readfile();
int main(void) {
    char dirname[512]="./data";
    DIR *dr;
    struct dirent *en;
    dr = opendir(dirname); //open all directory
    strcpy(dirname+strlen(dirname),"/");
    char * dirnameptr=dirname+strlen(dirname);
    
    int dest = open("out.txt", O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0666);
    int src;
    long offset=0;
    // sendfile(dest,source);
    // char * cptr=name;
    // cptr+=sizeof(char)*strlen(name);

    //todo 앞에 폴더명 붙여야함
    struct stat file_status;
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
        //  cout<<" \n"<<en->d_name; //print all directory name
            memcpy(dirnameptr,en->d_name,strlen(en->d_name)+1);
            // printf("dirname %s\n",dirname);
            int tmp=(stat(dirname, &file_status));
            if ((tmp < 0)||(S_ISREG(file_status.st_mode)==0)) {//check if it is valid file
                // printf("what %s  error %d eeeeeee %d\n",en->d_name,S_ISREG(file_status.st_mode),errno);
                continue;
            }
            // if ((stat(en->d_name, &file_status) < 0)) {
            //     printf("\n\nerror erronum %d\n\n",errno);
            //     continue;
            // }
            src = open(dirname, O_RDONLY, 0);
            sendfile(dest,src,NULL,file_status.st_size);
            offset+=file_status.st_size;
            printf("%s  size : %d byte\n",en->d_name,file_status.st_size);
            close(src);
            
        }
        closedir(dr); //close all directory
    }
    close(dest);
    long a=1;
    printf("\n%d\n",sizeof(a));
    dest=open("newa.txt", O_WRONLY | O_CREAT ,0666);
    src = open("out.txt", O_RDONLY, 0);

    // sendfile(dest,src,NULL,97962);
    // close(dest);
    // dest=open("newb.txt", O_WRONLY | O_CREAT ,0666);
    // sendfile(dest,src,NULL,20);
    
    return(0);
}

