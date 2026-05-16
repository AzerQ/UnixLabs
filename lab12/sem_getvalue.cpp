#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: sem_getvalue /name\n";
        return 1;
    }
    sem_t *s = sem_open(argv[1], O_RDWR);
    if (s == SEM_FAILED)
    {
        std::cerr << "sem_open: " << strerror(errno) << "\n";
        return 1;
    }
    int v = 0;
    sem_getvalue(s, &v);
    std::cout << argv[1] << " value=" << v << "\n";
    sem_close(s);
}
