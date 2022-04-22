#include <ctype.h>
#include "program4.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 1024

int main(int argc, char *argv[]) {

    if (argc != 2) {
        exit(EXIT_FAILURE);
        std::cout << "Please Include File Name" << std::endl;
    }

    char *shmpath = argv[1];

               
    int fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR,
                    S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("shm_open");

    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        errExit("ftruncate");

    //this is mapping but due to the fact that c++ cannot convert pointer type like c can, you have to typecase a dummy void pointer.
    struct shmbuf *shmp;
    void* dummy = mmap(NULL, sizeof(*shmp),
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
    shmp = (shmbuf *) dummy;
    if (shmp == MAP_FAILED)
        errExit("mmap");
    if (sem_init(&shmp->sem1, 1, 0) == -1)
        errExit("sem_init-sem1");
    if (sem_init(&shmp->sem2, 1, 0) == -1)
        errExit("sem_init-sem2");
    //basically this just waits until the server populates the shared memory before editing
    if (sem_wait(&shmp->sem1) == -1)
        errExit("sem_wait");

    //I didn't use threads but this turns all lower case letters into upppercase
    for (int j = 0; j < shmp->cnt; j++)
        shmp->buf[j] = toupper((unsigned char) shmp->buf[j]);

    exit(EXIT_SUCCESS);

    // struct stat l;
    // size_t filesize;

    // if(stat(argv[1], &l) != 0) {
    //     return 0;
    // }
    // else {
    //     filesize = l.st_size;
    // }
    
    // char buffer[filesize];

    // FILE *file;
    // file = fopen(argv[1], "r");
    // fread(buffer, sizeof(buffer), 1, file);
    // fclose(file);

    // FILE *writeToFile;
    // writeToFile = fopen(argv[1], "w");
    // for (int i = 0, j = sizeof(buffer); i < j; i++) {
    //     if(islower(buffer[i])) {
    //         buffer[i] = toupper(buffer[i]);
    //     }
    // }
    // fwrite(buffer, sizeof(buffer), 1, writeToFile);
    // fclose(writeToFile);
    // return 0;

    // char *addr;
    // int fd;

    // if (argc < 2 || strcmp(argv[1], "--help") == 0)
    //     printf("%s file [new-value]\n", argv[0]);

    // fd = open(argv[1], O_RDWR);
    // if (fd == -1)
    //     printf("open");

    // void * dummy = mmap(NULL, strlen(argv[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // addr = (char*)dummy;
    // if (addr == MAP_FAILED)
    //     printf("mmap");

    // if (close(fd) == -1)                /* No longer need 'fd' */
    //     printf("close");

    // printf("Current string=%.*s\n", MEM_SIZE, addr);
    //                     /* Secure practice: output at most MEM_SIZE bytes */

    // if (argc > 2) {                     /* Update contents of region */
    //     if (strlen(argv[2]) >= MEM_SIZE)
    //         printf("'new-value' too large\n");

    //     memset(addr, 0, MEM_SIZE);      /* Zero out region */
    //     strncpy(addr, argv[2], strlen(argv[2]));
    //     if (msync(addr, strlen(argv[2]), MS_SYNC) == -1)
    //         printf("msync");

    //     printf("Copied \"%s\" to shared memory\n", argv[2]);
    // }

    // exit(EXIT_SUCCESS);
}