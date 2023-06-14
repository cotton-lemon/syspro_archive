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

int main(void) {
    char name[64]=".";
    DIR *dr;
    struct dirent *en;
    dr = opendir(name); //open all directory
    // dr = opendir("./data"); //open all directory
    
    int dest = open("b.out", O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0666);
    // sendfile(dest,source);
    if (dr) {
        while ((en = readdir(dr)) != NULL) {
        //  cout<<" \n"<<en->d_name; //print all directory name
            struct stat file_status;
            if ((stat(en->d_name, &file_status) < 0)||(S_ISREG(file_status.st_mode)==0)) {//check if it is valid file
                continue;
            }
            // if ((stat(en->d_name, &file_status) < 0)) {
            //     printf("\n\nerror erronum %d\n\n",errno);
            //     continue;
            // }
            printf("\n %s  size : %d byte",en->d_name,file_status.st_size);
            
        }
        closedir(dr); //close all directory
    }
    long a=1;
    printf("%d",sizeof(a));
    return(0);
}

