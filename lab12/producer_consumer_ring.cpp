// Модель производитель-потребитель с несколькими буферами в памяти.
#include <array>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <thread>
constexpr int N = 3;
std::array<int, N> buffer{};
int put_i = 0, get_i = 0;
std::mutex m;
sem_t empty_slots, full_slots;
void producer()
{
    for (int x = 1; x <= 10; x++)
    {
        sem_wait(&empty_slots);
        {
            std::lock_guard<std::mutex> lock(m);
            buffer[put_i] = x;
            std::cout << "put " << x << " into slot " << put_i << "\n";
            put_i = (put_i + 1) % N;
        }
        sem_post(&full_slots);
    }
}
void consumer()
{
    for (int k = 1; k <= 10; k++)
    {
        sem_wait(&full_slots);
        int x;
        {
            std::lock_guard<std::mutex> lock(m);
            x = buffer[get_i];
            std::cout << "get " << x << " from slot " << get_i << "\n";
            get_i = (get_i + 1) % N;
        }
        sem_post(&empty_slots);
    }
}
int main()
{
    sem_init(&empty_slots, 0, N);
    sem_init(&full_slots, 0, 0);
    std::thread p(producer), c(consumer);
    p.join();
    c.join();
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
}
