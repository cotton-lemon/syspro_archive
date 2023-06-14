// #include <stdio.h>

// int main()
// {
//     printf("Hello World\n");

//     return 0;
// }



// #include <iostream>
#include <fcntl.h>   // open
#include <unistd.h>  // read, write, close
#include <stdio.h>    // BUFSIZ

#include <sys/sendfile.h>//sendfile
// #include <ctime>

int main() {
    // // clock_t start, end;

    // // BUFSIZE defaults to 8192
    // // BUFSIZE of 1 means one chareter at time
    // // good values should fit to blocksize, like 1024 or 4096
    // // higher values reduce number of system calls
    // // size_t BUFFER_SIZE = 4096;

    // char buf[BUFSIZ];
    // size_t size;

    // int source = open("s3.txt", O_RDONLY, 0);
    // int dest = open("a.out", O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);

    // while ((size = read(source, buf, BUFSIZ)) > 0) {
    //     write(dest, buf, size);
    // }

    // close(source);
    // source=  open("s2.txt", O_RDONLY, 0);
    // while ((size = read(source, buf, BUFSIZ)) > 0) {
    //     write(dest, buf, size);
    // }
    // close(source);
    // close(dest);


    // ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
    int source = open("s3.txt", O_RDONLY, 0);
    int dest = open("b.out", O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);
    sendfile(dest,source);
    return 0;
}