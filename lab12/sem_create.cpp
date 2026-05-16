// sem_create.cpp — создание именованного POSIX-семафора.
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char **argv)
{
    bool excl = false;
    unsigned int value = 1;
    int opt;
    while ((opt = getopt(argc, argv, "ei:")) != -1)
    {
        if (opt == 'e')
            excl = true;
        else if (opt == 'i')
            value = std::strtoul(optarg, nullptr, 10);
        else
        {
            std::cerr << "usage: sem_create [-e] [-i value] /name\n";
            return 1;
        }
    }
    if (optind != argc - 1)
    {
        std::cerr << "usage: sem_create [-e] [-i value] /name\n";
        return 1;
    }
    int flags = O_CREAT | O_RDWR | (excl ? O_EXCL : 0);
    sem_t *sem = sem_open(argv[optind], flags, 0644, value);
    if (sem == SEM_FAILED)
    {
        std::cerr << "sem_open: " << strerror(errno) << "\n";
        return 1;
    }
    sem_close(sem);
    std::cout << "created/opened semaphore " << argv[optind] << " value=" << value << "\n";
}
