#include <string.h>
#include "program4.h"
#include <iostream>
#include <string>

#define MEM_SIZE 1024

int main(int argc, char *argv[])
{
    std::cout << "SERVER STARTED" << std::endl;
    char *shmpath = argv[1];
    char *string = argv[2];
    size_t len = strlen(string);

    int fd = shm_open(shmpath, O_RDWR, 0);
    if (fd == -1)
        errExit("shm_open");

    std::cout << "OPENING: "<< shmpath << std::endl;
    struct shmbuf *shmp;
    void *dummy = mmap(NULL, sizeof(*shmp),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED, fd, 0);
    shmp = (shmbuf *) dummy;
    if (shmp == MAP_FAILED)
        errExit("mmap");

    shmp->cnt = len;
    memcpy(&shmp->buf, string, len);

    write(STDOUT_FILENO, &shmp->buf, len);
    write(STDOUT_FILENO, "\n", 1);
    std::clog << "FILE MAPPED TO SHARED MEMORY" << std::endl;
    std::clog << "FILE CLOSED" << std::endl;

    if (sem_post(&shmp->sem1) == -1)
        errExit("sem_post");

    if (sem_wait(&shmp->sem2) == -1)
        errExit("sem_wait");
    
    
    

    // char *addr;
    // int fd;

    // if (argc < 2 || strcmp(argv[1], "--help") == 0)
    //     printf("%s file [new-value]\n", argv[0]);

    // fd = open(argv[1], O_RDWR);
    // if (fd == -1)
    //     std::cout<<"OPENING:" << argv[1] << std::endl;

    // void * dummy = mmap(NULL, strlen(argv[2]), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // addr = (char*)dummy;
    // if (addr == MAP_FAILED)
    //     std::cout << "mmap" << std::endl;

    // if (close(fd) == -1)                /* No longer need 'fd' */
    //     std::cout << "close" << std::endl;

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

