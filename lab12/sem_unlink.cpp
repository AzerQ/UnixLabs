#include <cerrno>
#include <cstring>
#include <iostream>
#include <semaphore.h>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: sem_unlink /name\n";
        return 1;
    }
    if (sem_unlink(argv[1]) < 0)
    {
        std::cerr << "sem_unlink: " << strerror(errno) << "\n";
        return 1;
    }
    std::cout << "unlinked " << argv[1] << "\n";
}
