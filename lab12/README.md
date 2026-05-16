# Лабораторная работа №12

## Файлы
- `sem_create.cpp`, `sem_unlink.cpp`, `sem_getvalue.cpp`, `sem_wait.cpp`, `sem_post.cpp` — командный интерфейс для именованных POSIX-семафоров.
- `producer_consumer_file.cpp` — модель «производитель–потребитель» с буфером-файлом.
- `producer_consumer_ring.cpp` — модель «производитель–потребитель» с несколькими буферами.

## Компиляция
```bash
g++ -std=c++17 -Wall -Wextra -g sem_create.cpp -o sem_create -pthread
g++ -std=c++17 -Wall -Wextra -g sem_unlink.cpp -o sem_unlink -pthread
g++ -std=c++17 -Wall -Wextra -g sem_getvalue.cpp -o sem_getvalue -pthread
g++ -std=c++17 -Wall -Wextra -g sem_wait.cpp -o sem_wait -pthread
g++ -std=c++17 -Wall -Wextra -g sem_post.cpp -o sem_post -pthread
g++ -std=c++17 -Wall -Wextra -g producer_consumer_file.cpp -o producer_consumer_file -pthread
g++ -std=c++17 -Wall -Wextra -g producer_consumer_ring.cpp -o producer_consumer_ring -pthread
```

## Проверка семафоров
```bash
./sem_create -i 1 /testsem
./sem_getvalue /testsem
./sem_wait /testsem
./sem_getvalue /testsem
./sem_post /testsem
./sem_getvalue /testsem
./sem_unlink /testsem
```

## Проверка моделей
```bash
./producer_consumer_file
./producer_consumer_ring
```

## Отладка через gdb
```bash
gdb ./producer_consumer_ring
(gdb) break producer
(gdb) run
(gdb) info threads
(gdb) continue
```
