#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: sem_post /name\n";
        return 1;
    }
    sem_t *s = sem_open(argv[1], O_RDWR);
    if (s == SEM_FAILED)
    {
        std::cerr << "sem_open: " << strerror(errno) << "\n";
        return 1;
    }
    sem_post(s);
    std::cout << "posted\n";
    sem_close(s);
}
