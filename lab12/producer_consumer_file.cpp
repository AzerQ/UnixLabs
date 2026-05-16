// Модель производитель-потребитель с буфером в виде файла и именованными семафорами.
#include <chrono>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <semaphore.h>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
static const char *SEM_PUT = "/lab12_put";
static const char *SEM_GET = "/lab12_get";
static const char *BUF = "buffer.txt";
int main()
{
    sem_unlink(SEM_PUT);
    sem_unlink(SEM_GET);
    sem_t *put = sem_open(SEM_PUT, O_CREAT | O_RDWR, 0644, 1);
    sem_t *get = sem_open(SEM_GET, O_CREAT | O_RDWR, 0644, 0);
    if (put == SEM_FAILED || get == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }
    pid_t producer = fork();
    if (producer == 0)
    {
        for (int i = 1; i <= 5; i++)
        {
            sem_wait(put);
            std::ofstream(BUF) << "message " << i << '\n';
            std::cout << "producer: message " << i << '\n';
            sem_post(get);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        return 0;
    }
    pid_t consumer = fork();
    if (consumer == 0)
    {
        for (int i = 1; i <= 5; i++)
        {
            sem_wait(get);
            std::ifstream in(BUF);
            std::string s;
            std::getline(in, s);
            std::cout << "consumer: " << s << '\n';
            sem_post(put);
        }
        return 0;
    }
    int st;
    waitpid(producer, &st, 0);
    waitpid(consumer, &st, 0);
    sem_close(put);
    sem_close(get);
    sem_unlink(SEM_PUT);
    sem_unlink(SEM_GET);
}
