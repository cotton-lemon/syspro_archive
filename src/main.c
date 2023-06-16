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
// void test(ch)
int main(int argc,char* argv[]){
    // printf("argc %d\n",argc);
    if (argc!=4){
        // printf("argc %d\n",argc);
        wrong();
    }
    if (strcmp(argv[1],"pack")==0){
        printf("pack\n");
        pack(argv);
    }
    else if (strcmp(argv[1],"unpack")==0){
        // printf("pack");
        unpack(argv);
    }
    else if (strcmp(argv[1],"add")==0){
        add(argv);
    }
    else if (strcmp(argv[1],"del")==0){
        del(argv);
    }
    else{
        wrong();
    }
}

void wrong(){
    printf("wrong argument\n");
    printf("pack <archive-filename> <src-directory>\n");
    printf("unpack <archive-filename> <dest-diretory>\n");
    printf("add <archive-filename> <target-filename>\n");
    printf("del <archive-filename> <target-filename>\n");
    exit(1);
}

void pack(char* argv[]){
    char dirname[512];
    strcpy(dirname,argv[3]);
    DIR *dr;
    struct dirent *en;
    dr = opendir(dirname);
    strcpy(dirname+strlen(dirname),"/");
    char * dirnameptr=dirname+strlen(dirname);
    const char* tmpfilename=".filemeta.tmp.tmp";
    int dest = open(argv[2], O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0666);
    int metadest=open( tmpfilename,O_WRONLY | O_CREAT,0666);
    unsigned long metasize=0xf1234567;
    int src;
    unsigned long offset=12;
    struct stat file_status;
    if (dr) {
        int i=0xf1234567;
        write(dest,&i,sizeof(int));
        metasize=0;
        write(dest,&metasize,sizeof(long));
        while ((en = readdir(dr)) != NULL) {
            memcpy(dirnameptr,en->d_name,strlen(en->d_name)+1);
            int tmp=(stat(dirname, &file_status));
            if ((tmp < 0)||(S_ISREG(file_status.st_mode)==0)) {//check if it is valid file
                // printf("what %s  error %d eeeeeee %d\n",en->d_name,S_ISREG(file_status.st_mode),errno);
                continue;
            }
            src = open(dirname, O_RDONLY, 0);
            sendfile(dest,src,NULL,file_status.st_size);
            offset+=file_status.st_size;
            char size=strlen(en->d_name);
            unsigned long filesize=file_status.st_size;
            printf("%s  size : %d byte\n",en->d_name,filesize);
            write(metadest,&size,1);
            write(metadest,&filesize,sizeof(long));
            write(metadest,&(en->d_name),strlen(en->d_name));
            metasize+=(size+9);
            close(src);
        }
        closedir(dr); //close all directory
    }
    else{
        printf("no such directory\n");
        exit(1);
    }
    if (metasize==0){
        printf("empty folder\n");
        exit(1);
    }
    close(metadest);
    metadest=open(tmpfilename, O_RDONLY,0666);
    sendfile(dest,metadest,NULL,metasize);
    lseek(dest,sizeof(int),0);
    // offset=0x345678ff;
    write(dest,&offset,sizeof(long));
    printf("offset: %d\n",offset);

    close(dest);
    close(metadest);
    remove(tmpfilename);
    // stat(dirname, &file_status);
    printf("meta size: %d\n",metasize);

}

void unpack(char* argv[]){
    char dirname[1024];
    strcpy(dirname,argv[3]);
    mkdir(dirname,0766);
    strcpy(dirname+strlen(dirname),"/");
    char * dirnameptr=dirname+strlen(dirname);
    int archive = open(argv[2], O_RDONLY, 066);
    if(archive==-1){
        printf("archive file open fail\n");
        exit(1);
    }
    int meta=open(argv[2], O_RDONLY, 0);
    char buf[20];
    // memset(buf,0,20);

    int howread=read(meta,buf,20);
    printf("how read %d\n",howread);
        for(int i=0;i<10;++i){
        printf("%x\n",buf[i]);
    }
    int tag = *(int*)(buf);
    unsigned long offset=*(unsigned long*)(buf+4);
    if (tag!=0xf1234567){
        printf("not an arcx archive file\n");
    }
    printf("tag %d, offset %d \n",tag,offset);


    //start unarchive
    unsigned long start=12;
    lseek(meta,offset,0);
    lseek(archive,12,0);
    int count=0;
    printf("unarchivew start");
    while(start<offset){
        printf("in while\n");
        char dirname2[1024];
        memset(dirname2,0,1024);
        strcpy(dirname2,argv[3]);
        mkdir(dirname2,0766);
        chmod(dirname2,0766);
        strcpy(dirname2+strlen(dirname2),"/");
        char * dirnameptr2=dirname2+strlen(dirname2);
        char buf1[20];
        char buf2[512];
        memset(buf2,0,512);
        read(meta,buf1,9);
        char namelength = buf1[0];
        unsigned long filesize=*(unsigned long*)(buf1+1);
        printf("namelength %d qqq   %d\b",namelength,filesize);
        read(meta,buf2,namelength);
        memcpy(dirnameptr2,buf2,strlen(buf2));
        int newfile=open(dirname2,O_WRONLY | O_CREAT,0666);
        // int newfile=open(buf2,O_WRONLY | O_CREAT,0666);
        if (newfile==-1){
            printf("file openfail\n");
        }
        sendfile(newfile,archive,NULL,filesize);
        printf("file name: [[%s]] fileend\n",dirname2);
        start+=filesize;
        ++count;
        printf("%d start\n",start);
        printf("%s",dirname2);
        if(count>10){
            exit(1);
        }
        close(newfile);
    }
    // int q= open("small1/1.txt",O_WRONLY | O_CREAT,0666);
    // char * dd[32]={'1','2','3','4'};
    // write(q,dd,32);
    // close(q);

}

void add(char* argv[]){

}

void del(char* argv[]){

}