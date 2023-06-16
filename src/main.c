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
    // printf("argc %d\n",argc);
    if (argc!=4){
        if (strcmp(argv[1],"list")==0){
        list(argv);
        return 0;
        }
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
    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int metadest=open( tmpfilename,O_WRONLY | O_CREAT,0666);
    unsigned int metasize=0xf1234567;
    int src;
    unsigned int offset=8;
    struct stat file_status;
    int numoffile=0;
    if (dr) {
        int i=0xf1234567;
        write(dest,&i,sizeof(int));
        metasize=0;
        write(dest,&metasize,sizeof(int));
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
            unsigned int filesize=file_status.st_size;
            printf("%s  size : %d byte\n",en->d_name,filesize);
            write(metadest,&size,1);
            write(metadest,&filesize,sizeof(int));
            write(metadest,&(en->d_name),strlen(en->d_name));
            metasize+=(size+5);
            numoffile+=1;
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
        close(dest);
        close(metadest);
        remove(argv[2]);
        remove(tmpfilename);
        exit(1);
    }
    close(metadest);
    metadest=open(tmpfilename, O_RDONLY,0666);
    sendfile(dest,metadest,NULL,metasize);
    lseek(dest,sizeof(int),0);
    // offset=0x345678ff;
    write(dest,&offset,sizeof(int));
    printf("offset: %d\n",offset);

    close(dest);
    close(metadest);
    remove(tmpfilename);
    // stat(dirname, &file_status);
    printf("meta size: %d\n",metasize);
    printf("%d file(s) archived\narchive size: %d\n",numoffile,offset+metasize);


}

void unpack(char* argv[]){
    char dirname[1024];
    strcpy(dirname,argv[3]);
    mkdir(dirname,0766);
    chmod(dirname,0766);
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

    int howread=read(meta,buf,14);
    if (howread<5){
        printf("sth wrong in the file\n");
        exit(1);
    }
    int tag = *(int*)(buf);
    unsigned int offset=*(unsigned int*)(buf+4);
    if (tag!=0xf1234567){
        printf("not an arcx archive file\n");
        exit(1);
    }
    printf("tag %d, offset %d \n",tag,offset);


    //start unarchive
    unsigned int start=8;
    lseek(meta,offset,0);
    lseek(archive,8,0);
    int count=0;
    // printf("unarchivew start");
    while(start<offset){
        // printf("in while\n");
        char dirname2[1024];
        memset(dirname2,0,1024);
        strcpy(dirname2,argv[3]);
        printf("%s\n",dirname2);
        strcpy(dirname2+strlen(dirname2),"/");
        printf("%s\n",dirname2);
        char * dirnameptr2=dirname2+strlen(dirname2);
        char buf1[20];
        char buf2[512];
        memset(buf2,0,512);
        read(meta,buf1,5);
        char namelength = buf1[0];
        unsigned int filesize=*(unsigned int*)(buf1+1);
        printf("namelength %d qqq   %d\b",namelength,filesize);
        read(meta,buf2,namelength);
        memcpy(dirnameptr2,buf2,strlen(buf2));
        int newfile=open(dirname2,O_WRONLY | O_CREAT,0666);
        // int newfile=open(buf2,O_WRONLY | O_CREAT,0666);
        if (newfile==-1){
            printf("file openfail\n");
            printf("%s\n",dirname2);
            exit(1);
        }
        sendfile(newfile,archive,NULL,filesize);
        // printf("file name: [[%s]] fileend\n",dirname2);
        start+=filesize;
        close(newfile);
    }

}

void add(char* argv[]){
    int meta=open(argv[2], O_RDONLY, 0);
    char buf[8];
    int howread=read(meta,buf,14);
    if (howread<5){
        printf("sth wrong in the file\n");
        exit(1);
    }
    int tag = *(int*)(buf);
    unsigned int offset=*(unsigned int*)(buf+4);
    if (tag!=0xf1234567){
        printf("not an arcx archive file\n");
        exit(1);
    }


    unsigned int start=8;
    lseek(meta,offset,0);
    while(start<offset){
        char buf1[20];
        char buf2[512];
        memset(buf2,0,512);
        read(meta,buf1,5);
        char namelength = buf1[0];
        unsigned int filesize=*(unsigned int*)(buf1+1);
        read(meta,buf2,namelength);
        printf("%s size: %d\n",buf2,filesize);
        start+=filesize;
        if(strcmp(buf2,argv[3])==0){
            printf("already in archive file\n");
            exit(1);
        }
    }
    close(meta);


    meta=open(argv[2], O_RDONLY, 0);
    struct stat file_status;
    int tmp=(stat(argv[3], &file_status));
            if ((tmp < 0)||(S_ISREG(file_status.st_mode)==0)) {//check if it is valid file
                printf("not a vaild file\n");
                exit(1);
            }
    int newfile=open(argv[3], O_RDONLY, 066);
    if(meta==-1){
        printf("archive file open fail\n");
        exit(1);
    }
    if (newfile==-1){
        printf("new file open fail\n");
        exit(1);
    }



    printf("offset %d\n",offset);
    start=8;
    int p=lseek(meta,0,SEEK_SET);
    printf("offset?? %d\n",p);
    // printf("size t%d\n",sizeof(off_t));
    // printf("size t%d\n",sizeof(signed int));
    // printf("size t%d\n",offset);why int offset not working?
    unsigned int qwe=offset;
    p=lseek(meta,qwe,SEEK_SET);
    printf("offset?? %d\n",p);
    printf("err %d\n",errno);
    const char* tmpfilename=".filemeta.tmp.tmp";
    int metatmp=open( tmpfilename,O_WRONLY | O_CREAT | O_TRUNC,0666);
    sendfile(metatmp,meta,NULL,(2<<16)-1);//meta 임시
    close(meta);
    meta=open(argv[2], O_WRONLY);
    //offset뒤에 파일추가
    // lseek(meta,offset,0);
    p=lseek(meta,qwe,SEEK_SET);
    // printf("qqqqoffset?? %d\n",p);
    printf("err %d\n",errno);
    sendfile(meta,newfile,NULL,file_status.st_size);
    printf("newfile size %d\n",file_status.st_size);
    
    unsigned int filesize=file_status.st_size;
    char size=strlen(argv[3]);
    write(metatmp,&size,1);
    write(metatmp,&filesize,sizeof(int));
    write(metatmp,&(argv[3]),size);
    unsigned int newoffset=offset+filesize;
    int metasize=lseek(metatmp,0,2);
    metatmp=open(tmpfilename, O_RDONLY,0666);
    sendfile(meta,metatmp,NULL,metasize);
    close(metatmp);
    lseek(meta,sizeof(int),0);
    write(meta,&newoffset,sizeof(int));
    close(newfile);
    close(meta);
    
    // printf("metasize %d\n",metasize);


    // remove(tmpfilename);
    
    






}

void del(char* argv[]){

}
void list(char* argv[]){
    int meta=open(argv[2], O_RDONLY, 0);
    char buf[15];
    int howread=read(meta,buf,14);
    if (howread<5){
        printf("sth wrong in the file\n");
        exit(1);
    }
    int tag = *(int*)(buf);
    unsigned int offset=*(unsigned int*)(buf+4);
    if (tag!=0xf1234567){
        printf("not an arcx archive file\n");
        exit(1);
    }


    unsigned int start=8;
    unsigned int q=offset;
    printf("tag %d, offset %d\n",tag,offset);
    lseek(meta,q,0);
    int count=1;
    while(start<offset){
        char buf1[20];
        char buf2[512];
        memset(buf2,0,512);
        read(meta,buf1,5);
        char namelength = buf1[0];
        unsigned int filesize=*(unsigned int*)(buf1+1);
        read(meta,buf2,namelength);
        printf("%s size: %d\n",buf2,filesize);
        printf("%d %d\n",filesize,start);
        printf("start %d offset %d\n",start,offset);
        printf("%d\n",start<offset);
        start+=filesize;
        if(++count>9){
            exit(1);
        }
    }

}