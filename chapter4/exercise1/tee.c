#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int appendMode = 0;

    int opt;
    while ((opt = getopt(argc, argv, "a")) != -1)
    {
        switch (opt)
        {
        case 'a':
            appendMode = 1;
            break;
        }
    }

    int fd, openFlags;
    mode_t filePerms;

    if (appendMode == 1)
    {
        openFlags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else
    {
        openFlags = O_WRONLY | O_CREAT | O_TRUNC;
    }

    filePerms = S_IRUSR | S_IWUSR |
                S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;

    fd = open(argv[optind], openFlags, filePerms);

    if (fd == -1)
    {
        printf("Failed to open file: %s", argv[optind]);
        return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    ssize_t numRead;
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buf, numRead) != numRead ||
                write(fd, buf, numRead) != numRead)
        {
            printf("Failed to write");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
