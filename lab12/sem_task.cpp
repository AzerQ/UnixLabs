// Сделать семафор, установить значение 5, вывести значение на экран, обнулить, затем вывести на экран снова

#include <cerrno>
#include <cstring>
#include <iostream>
#include <semaphore.h>

int main()
{
    sem_t semaphore;
    if (sem_init(&semaphore, 0, 5) != 0)
    {
        std::cerr << "sem_init: " << std::strerror(errno) << "\n";
        return 1;
    }

    int value = 0;
    if (sem_getvalue(&semaphore, &value) != 0)
    {
        std::cerr << "sem_getvalue: " << std::strerror(errno) << "\n";
        sem_destroy(&semaphore);
        return 1;
    }
    std::cout << "value=" << value << "\n";

    while (true)
    {
        if (sem_trywait(&semaphore) != 0)
        {
            if (errno == EAGAIN)
            {
                break;
            }
            std::cerr << "sem_trywait: " << std::strerror(errno) << "\n";
            sem_destroy(&semaphore);
            return 1;
        }
    }

    if (sem_getvalue(&semaphore, &value) != 0)
    {
        std::cerr << "sem_getvalue: " << std::strerror(errno) << "\n";
        sem_destroy(&semaphore);
        return 1;
    }
    std::cout << "value=" << value << "\n";

    sem_destroy(&semaphore);
    return 0;
}
