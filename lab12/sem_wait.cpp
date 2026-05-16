#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: sem_wait /name\n";
        return 1;
    }
    sem_t *s = sem_open(argv[1], O_RDWR);
    if (s == SEM_FAILED)
    {
        std::cerr << "sem_open: " << strerror(errno) << "\n";
        return 1;
    }
    std::cout << "waiting...\n";
    if (sem_wait(s) < 0)
    {
        std::cerr << "sem_wait: " << strerror(errno) << "\n";
        return 1;
    }
    std::cout << "locked\n";
    sem_close(s);
}
