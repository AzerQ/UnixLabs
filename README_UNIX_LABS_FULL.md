
# Подробный README по лабораторным работам №11–14 по Unix/Linux

Файл описывает, как скомпилировать, запустить, протестировать и отладить программы C++ для лабораторных работ №11–14 по дисциплине «Операционные системы семейства UNIX».

Исходники предполагают следующую структуру каталогов:

```text
unix_labs_cpp/
├── lab11/
│   ├── myshell_process.cpp
│   ├── myshell_pipe.cpp
│   └── myshell_fifo.cpp
├── lab12/
│   ├── sem_create.cpp
│   ├── sem_unlink.cpp
│   ├── sem_getvalue.cpp
│   ├── sem_wait.cpp
│   ├── sem_post.cpp
│   ├── producer_consumer_file.cpp
│   └── producer_consumer_ring.cpp
├── lab13/
│   └── myshell_threads.cpp
└── lab14/
    └── myshell_sem_pipe.cpp
```

---

## 1. Общая подготовка окружения

### 1.1. Требуемая система

Рекомендуемая среда выполнения:

- Linux: Ubuntu, Debian, Linux Mint, Fedora или другая POSIX-совместимая система;
- компилятор `g++`;
- отладчик `gdb`;
- стандартные утилиты shell: `ls`, `pwd`, `date`, `cat`, `echo`, `sleep`, `sort`.

На Debian/Ubuntu установите инструменты так:

```bash
sudo apt update
sudo apt install -y build-essential gdb make
```

Проверка:

```bash
g++ --version
gdb --version
```

### 1.2. Распаковка архива и переход в каталог

```bash
unzip unix_labs_cpp.zip
cd unix_labs_cpp
```

Если архив уже распакован, достаточно перейти в каталог проекта:

```bash
cd unix_labs_cpp
```

### 1.3. Общие флаги компиляции

Для большинства программ удобно использовать такие флаги:

```bash
-g -O0 -Wall -Wextra -std=c++17
```

Назначение флагов:

- `-g` — добавить отладочную информацию для `gdb`;
- `-O0` — отключить оптимизацию, чтобы пошаговая отладка была понятнее;
- `-Wall -Wextra` — включить предупреждения компилятора;
- `-std=c++17` — использовать стандарт C++17;
- `-pthread` — подключить поддержку потоков и корректную линковку pthread, требуется для программ с потоками и неименованными семафорами.

### 1.4. Общий шаблон отладки в gdb

Для любой программы можно использовать такую последовательность:

```bash
gdb ./program_name
```

Внутри `gdb`:

```gdb
break main
run
next
step
continue
quit
```

Полезные команды `gdb`:

```gdb
break function_name       # поставить точку останова на функцию
break file.cpp:42         # поставить точку останова на строку
run                       # запустить программу
run arg1 arg2             # запустить программу с аргументами
next                      # выполнить следующую строку без захода внутрь функции
step                      # выполнить следующую строку с заходом внутрь функции
continue                  # продолжить выполнение до следующей точки останова
print variable            # вывести значение переменной
info locals               # показать локальные переменные
backtrace                 # показать стек вызовов
info threads              # показать потоки
thread 2                  # переключиться на поток №2
set follow-fork-mode child # при fork() отлаживать дочерний процесс
set follow-fork-mode parent # при fork() отлаживать родительский процесс
quit                      # выйти из отладчика
```

---

# Лабораторная работа №11

## 2. ЛР11. Создание простейшей командной оболочки ядра Linux на базе процессов

### 2.1. Назначение программ

В лабораторной работе №11 реализованы три варианта оболочки:

1. `myshell_process.cpp` — простейшая оболочка на базе процессов: чтение команд, разделение по `;`, запуск через `fork()` и `execvp()`, поддержка перенаправлений `<`, `>`, `>>`, встроенные команды `cd`, `exit`, `showenv`.
2. `myshell_pipe.cpp` — версия с передачей команд из родительского процесса дочернему через неименованный канал `pipe()`.
3. `myshell_fifo.cpp` — версия с передачей команд через именованный канал `FIFO`, создаваемый через `mkfifo()`.

Перейдите в каталог лабораторной:

```bash
cd unix_labs_cpp/lab11
```

---

## 2.2. Компиляция ЛР11

### 2.2.1. Компиляция оболочки на базе процессов

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_process.cpp -o myshell_process
```

### 2.2.2. Компиляция оболочки с pipe()

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_pipe.cpp -o myshell_pipe
```

### 2.2.3. Компиляция оболочки с FIFO

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_fifo.cpp -o myshell_fifo
```

### 2.2.4. Компиляция всех программ ЛР11 одной командой

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_process.cpp -o myshell_process && \
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_pipe.cpp -o myshell_pipe && \
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_fifo.cpp -o myshell_fifo
```

Проверить, что исполняемые файлы созданы:

```bash
ls -l myshell_process myshell_pipe myshell_fifo
```

---

## 2.3. Тестовые запуски ЛР11

### 2.3.1. Тестирование `myshell_process`

Запуск:

```bash
./myshell_process
```

Внутри оболочки выполните:

```text
date; pwd; ls
```

Ожидаемый смысл результата:

- `date` выводит текущую дату и время;
- `pwd` выводит текущий каталог;
- `ls` выводит список файлов текущего каталога.

Пример:

```text
$> date; pwd; ls
Sat May 16 12:00:00 UTC 2026
/home/user/unix_labs_cpp/lab11
myshell_fifo.cpp  myshell_pipe.cpp  myshell_process.cpp
```

Проверка перенаправления вывода:

```text
echo Hello > result.txt; cat result.txt
```

Ожидаемый результат:

```text
Hello
```

Проверка добавления в файл:

```text
echo First > result.txt; echo Second >> result.txt; cat result.txt
```

Ожидаемый результат:

```text
First
Second
```

Проверка перенаправления ввода:

```text
cat < result.txt
```

Ожидаемый результат:

```text
First
Second
```

Проверка встроенной команды `cd`:

```text
pwd; cd ..; pwd
```

Выход:

```text
exit
```

---

### 2.3.2. Тестирование `myshell_pipe`

Запуск:

```bash
./myshell_pipe
```

Проверка передачи нескольких команд через `pipe()`:

```text
date; pwd; echo pipe_ok
```

Ожидаемый результат:

```text
Sat May 16 12:00:00 UTC 2026
/home/user/unix_labs_cpp/lab11
pipe_ok
```

Проверка последовательного выполнения команд:

```text
echo one; echo two; echo three
```

Ожидаемый результат:

```text
one
two
three
```

Выход:

```text
exit
```

Особенность этой версии: родительский процесс читает строку пользователя и пишет команды в канал, а дочерний процесс читает команды из канала и запускает их через `execvp()`.

---

### 2.3.3. Тестирование `myshell_fifo`

Запуск:

```bash
./myshell_fifo
```

Проверка передачи команд через FIFO:

```text
pwd; echo fifo_ok; ls
```

Ожидаемый результат:

```text
/home/user/unix_labs_cpp/lab11
fifo_ok
myshell_fifo  myshell_fifo.cpp  myshell_pipe  myshell_pipe.cpp  myshell_process  myshell_process.cpp
```

Выход:

```text
exit
```

Проверка, что FIFO создаётся в `/tmp`:

В отдельном терминале во время работы программы можно выполнить:

```bash
ls -l /tmp/lab11_cmd_fifo
```

Ожидаемый результат — файл специального типа `p`:

```text
prw-r--r-- 1 user user 0 May 16 12:00 /tmp/lab11_cmd_fifo
```

После завершения программы FIFO удаляется вызовом `unlink()`.

---

## 2.4. Отладка ЛР11 в gdb

### 2.4.1. Отладка `myshell_process`

```bash
gdb ./myshell_process
```

Команды внутри `gdb`:

```gdb
break main
run
next
next
```

После появления приглашения оболочки введите тестовую команду:

```text
echo debug_test
```

Для отладки запуска дочернего процесса полезно задать режим следования за дочерним процессом:

```gdb
set follow-fork-mode child
break execute
run
```

После остановки на функции `execute` можно смотреть аргументы:

```gdb
next
info locals
print args
```

Рекомендуемые точки останова:

```gdb
break split_commands
break tokenize
break execute
break apply_redirections
```

Что показать в отчёте:

- остановку на `main`;
- разбор строки команд в `split_commands`;
- формирование аргументов в `tokenize`;
- запуск дочернего процесса через `fork()`;
- переход к `execvp()`;
- обработку перенаправления через `dup2()`.

---

### 2.4.2. Отладка `myshell_pipe`

```bash
gdb ./myshell_pipe
```

Рекомендуемый режим:

```gdb
set follow-fork-mode parent
break main
run
```

Для анализа дочернего процесса:

```gdb
set follow-fork-mode child
run
```

Рекомендуемые точки останова:

```gdb
break main
break run_cmd
```

Что показать в отчёте:

- создание канала `pipe(fd)`;
- `fork()` дочернего процесса;
- закрытие ненужных концов канала: `close(fd[0])`, `close(fd[1])`;
- запись команды родителем через `fprintf(out, ...)`;
- чтение команды дочерним процессом через `getline()`;
- запуск команды через `execvp()`.

---

### 2.4.3. Отладка `myshell_fifo`

```bash
gdb ./myshell_fifo
```

Рекомендуемые точки останова:

```gdb
break main
break run_cmd
run
```

Что показать в отчёте:

- создание FIFO вызовом `mkfifo(FIFO, 0666)`;
- открытие FIFO на чтение в дочернем процессе;
- открытие FIFO на запись в родительском процессе;
- передачу команд через именованный канал;
- удаление FIFO после завершения программы.

---

# Лабораторная работа №12

## 3. ЛР12. Семафоры стандарта POSIX для синхронизации процессов и потоков

### 3.1. Назначение программ

В лабораторной работе №12 используются следующие программы:

1. `sem_create.cpp` — создание или открытие именованного POSIX-семафора.
2. `sem_unlink.cpp` — удаление именованного POSIX-семафора.
3. `sem_getvalue.cpp` — вывод текущего значения семафора.
4. `sem_wait.cpp` — выполнение операции `sem_wait()`.
5. `sem_post.cpp` — выполнение операции `sem_post()`.
6. `producer_consumer_file.cpp` — модель «производитель–потребитель» с буфером в виде файла и именованными семафорами.
7. `producer_consumer_ring.cpp` — модель «производитель–потребитель» с несколькими буферами в памяти и неименованными семафорами.

Перейдите в каталог лабораторной:

```bash
cd unix_labs_cpp/lab12
```

---

## 3.2. Компиляция ЛР12

### 3.2.1. Компиляция программ командного интерфейса семафоров

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 sem_create.cpp -o sem_create -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_unlink.cpp -o sem_unlink -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_getvalue.cpp -o sem_getvalue -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_wait.cpp -o sem_wait -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_post.cpp -o sem_post -pthread
```

### 3.2.2. Компиляция моделей «производитель–потребитель»

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_file.cpp -o producer_consumer_file -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_ring.cpp -o producer_consumer_ring -pthread
```

### 3.2.3. Компиляция всех программ ЛР12 одной командой

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 sem_create.cpp -o sem_create -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 sem_unlink.cpp -o sem_unlink -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 sem_getvalue.cpp -o sem_getvalue -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 sem_wait.cpp -o sem_wait -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 sem_post.cpp -o sem_post -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_file.cpp -o producer_consumer_file -pthread && \
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_ring.cpp -o producer_consumer_ring -pthread
```

Проверка:

```bash
ls -l sem_create sem_unlink sem_getvalue sem_wait sem_post producer_consumer_file producer_consumer_ring
```

---

## 3.3. Тестовые запуски ЛР12

### 3.3.1. Проверка создания и удаления именованного семафора

Сначала удалим семафор, если он остался от предыдущих запусков:

```bash
./sem_unlink /lab12_test
```

Если семафора нет, может появиться сообщение об ошибке — это не критично при предварительной очистке.

Создание семафора со значением `2`:

```bash
./sem_create -i 2 /lab12_test
```

Ожидаемый результат:

```text
created/opened semaphore /lab12_test value=2
```

Проверка текущего значения:

```bash
./sem_getvalue /lab12_test
```

Ожидаемый результат:

```text
/lab12_test value=2
```

Выполнение `sem_wait()`:

```bash
./sem_wait /lab12_test
```

Ожидаемый результат:

```text
waiting...
locked
```

После `sem_wait()` значение должно уменьшиться:

```bash
./sem_getvalue /lab12_test
```

Ожидаемый результат:

```text
/lab12_test value=1
```

Выполнение `sem_post()`:

```bash
./sem_post /lab12_test
```

Ожидаемый результат:

```text
posted
```

Проверка значения:

```bash
./sem_getvalue /lab12_test
```

Ожидаемый результат:

```text
/lab12_test value=2
```

Удаление семафора:

```bash
./sem_unlink /lab12_test
```

Ожидаемый результат:

```text
unlinked /lab12_test
```

---

### 3.3.2. Проверка блокировки `sem_wait()`

Создайте семафор с начальным значением `0`:

```bash
./sem_create -i 0 /lab12_block
```

В первом терминале запустите:

```bash
./sem_wait /lab12_block
```

Программа выведет:

```text
waiting...
```

и заблокируется, так как значение семафора равно `0`.

Во втором терминале выполните:

```bash
cd unix_labs_cpp/lab12
./sem_post /lab12_block
```

После этого в первом терминале программа продолжит выполнение и выведет:

```text
locked
```

Очистка:

```bash
./sem_unlink /lab12_block
```

---

### 3.3.3. Тестирование модели с файловым буфером

Запуск:

```bash
./producer_consumer_file
```

Пример ожидаемого вывода:

```text
producer: message 1
consumer: message 1
producer: message 2
consumer: message 2
producer: message 3
consumer: message 3
producer: message 4
consumer: message 4
producer: message 5
consumer: message 5
```

Смысл результата:

- производитель записывает сообщение в файл `buffer.txt`;
- потребитель читает сообщение из этого файла;
- семафор `/lab12_put` разрешает производителю записывать очередное сообщение;
- семафор `/lab12_get` разрешает потребителю читать сообщение только после записи.

После запуска можно проверить файл:

```bash
cat buffer.txt
```

---

### 3.3.4. Тестирование модели с несколькими буферами

Запуск:

```bash
./producer_consumer_ring
```

Пример ожидаемого вывода:

```text
put 1 into slot 0
put 2 into slot 1
put 3 into slot 2
get 1 from slot 0
get 2 from slot 1
get 3 from slot 2
put 4 into slot 0
...
```

Смысл результата:

- используется кольцевой буфер из трёх ячеек;
- семафор `empty_slots` хранит количество свободных ячеек;
- семафор `full_slots` хранит количество заполненных ячеек;
- мьютекс защищает индексы `put_i` и `get_i`.

---

## 3.4. Отладка ЛР12 в gdb

### 3.4.1. Отладка `sem_create`

```bash
gdb ./sem_create
```

Внутри `gdb`:

```gdb
break main
run -i 2 /lab12_debug
next
info locals
continue
quit
```

Что показать в отчёте:

- разбор аргументов `-i 2`;
- формирование флагов `O_CREAT | O_RDWR`;
- вызов `sem_open()`;
- закрытие семафора через `sem_close()`.

Очистка после отладки:

```bash
./sem_unlink /lab12_debug
```

---

### 3.4.2. Отладка `sem_wait` и `sem_post`

Подготовка:

```bash
./sem_create -i 1 /lab12_debug_wait
```

Запуск отладки:

```bash
gdb ./sem_wait
```

Внутри `gdb`:

```gdb
break main
run /lab12_debug_wait
next
next
info locals
continue
quit
```

Проверить значение после `sem_wait()`:

```bash
./sem_getvalue /lab12_debug_wait
```

Затем выполнить `sem_post()`:

```bash
gdb ./sem_post
```

Внутри `gdb`:

```gdb
break main
run /lab12_debug_wait
next
continue
quit
```

Очистка:

```bash
./sem_unlink /lab12_debug_wait
```

---

### 3.4.3. Отладка модели `producer_consumer_file`

```bash
gdb ./producer_consumer_file
```

Рекомендуемые команды:

```gdb
set follow-fork-mode child
break main
run
next
continue
```

Для наблюдения за родителем:

```gdb
set follow-fork-mode parent
run
```

Что показать в отчёте:

- создание двух именованных семафоров `/lab12_put` и `/lab12_get`;
- создание процесса производителя через `fork()`;
- создание процесса потребителя через `fork()`;
- блокировку/разблокировку через `sem_wait()` и `sem_post()`;
- запись в файл `buffer.txt`;
- чтение из файла `buffer.txt`.

---

### 3.4.4. Отладка модели `producer_consumer_ring`

```bash
gdb ./producer_consumer_ring
```

Рекомендуемые команды:

```gdb
break main
break producer
break consumer
run
info threads
next
continue
```

Так как программа использует потоки, полезно:

```gdb
info threads
thread 2
backtrace
thread 3
backtrace
```

Что показать в отчёте:

- создание потоков `producer` и `consumer`;
- инициализацию `sem_init(&empty_slots, 0, N)`;
- инициализацию `sem_init(&full_slots, 0, 0)`;
- переход элементов по кольцевому буферу;
- синхронизацию через `sem_wait()` и `sem_post()`.

---

# Лабораторная работа №13

## 4. ЛР13. Создание командной оболочки ядра Linux на базе процессов и программных потоков

### 4.1. Назначение программы

Файл:

```text
lab13/myshell_threads.cpp
```

Назначение:

- оболочка читает командную строку;
- разделяет команды по символу `;`;
- интерактивные команды выполняются последовательно;
- команды с символом `&` считаются неинтерактивными и запускаются через серверы-потоки;
- в программе выполнено условие `MAXSERVERS == MAXQUEUE`.

Перейдите в каталог лабораторной:

```bash
cd unix_labs_cpp/lab13
```

---

## 4.2. Компиляция ЛР13

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_threads.cpp -o myshell_threads -pthread
```

Проверка:

```bash
ls -l myshell_threads
```

---

## 4.3. Тестовые запуски ЛР13

### 4.3.1. Простой запуск

```bash
./myshell_threads
```

Внутри оболочки:

```text
date; pwd; echo hello
```

Ожидаемый результат:

```text
Sat May 16 12:00:00 UTC 2026
/home/user/unix_labs_cpp/lab13
hello
```

Выход:

```text
exit
```

---

### 4.3.2. Проверка фоновых команд

Запуск:

```bash
./myshell_threads
```

Ввод:

```text
sleep 1 &; echo interactive_command; date
```

Ожидаемый смысл результата:

- команда `sleep 1 &` будет запущена в отдельном потоке;
- `echo interactive_command` и `date` выполнятся как обычные команды;
- оболочка дождётся завершения серверных потоков перед следующей строкой ввода.

Пример вывода:

```text
interactive_command
Sat May 16 12:00:00 UTC 2026
```

---

### 4.3.3. Проверка нескольких фоновых команд

```text
sleep 1 &; sleep 1 &; echo after_background
```

Ожидаемый результат:

```text
after_background
```

Смысл теста: две фоновые команды должны быть переданы в отдельные серверы-потоки. После этого программа дождётся завершения потоков.

---

## 4.4. Отладка ЛР13 в gdb

Запуск:

```bash
gdb ./myshell_threads
```

Рекомендуемые команды:

```gdb
break main
break parse
break exec_command
run
```

После запуска введите:

```text
sleep 1 &; echo debug
```

Для анализа потоков:

```gdb
info threads
thread 2
backtrace
thread 3
backtrace
```

Для пошагового анализа:

```gdb
next
step
print created
print commands.size()
continue
```

Что показать в отчёте:

- разбор командной строки в функции `parse()`;
- определение признака фоновой команды `background`;
- создание объекта `std::thread`;
- запуск команды внутри `exec_command()`;
- создание процесса через `fork()` внутри потока;
- ожидание завершения всех потоков через `join()`.

---

# Лабораторная работа №14

## 5. ЛР14. Создание командной оболочки ядра Linux на базе процессов, программных каналов и семафоров POSIX

### 5.1. Назначение программы

Файл:

```text
lab14/myshell_sem_pipe.cpp
```

Программа реализует оболочку, в которой:

- родительский процесс читает команды пользователя;
- команды передаются серверным процессам через неименованный канал `pipe()`;
- интерактивные команды обрабатываются сервером `interactive-server`;
- неинтерактивные команды с `&` обрабатываются сервером `background-server`;
- для сигнализации о поступлении команды используются семафоры `/lab14_sem0get` и `/lab14_sem1get`;
- для сигнализации о завершении используются отдельные семафоры `/lab14_sem0end` и `/lab14_sem1end`;
- поддерживается простая команда `history`, которая выводит историю введённых команд.

Перейдите в каталог лабораторной:

```bash
cd unix_labs_cpp/lab14
```

---

## 5.2. Компиляция ЛР14

```bash
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_sem_pipe.cpp -o myshell_sem_pipe -pthread
```

Проверка:

```bash
ls -l myshell_sem_pipe
```

---

## 5.3. Тестовые запуски ЛР14

### 5.3.1. Простой запуск

```bash
./myshell_sem_pipe
```

Внутри оболочки:

```text
date; pwd; echo lab14
```

Пример вывода:

```text
[interactive-server] date
Sat May 16 12:00:00 UTC 2026
[interactive-server] pwd
/home/user/unix_labs_cpp/lab14
[interactive-server] echo lab14
lab14
```

Выход:

```text
exit
```

---

### 5.3.2. Проверка интерактивных и неинтерактивных команд

Запуск:

```bash
./myshell_sem_pipe
```

Ввод:

```text
echo interactive; (echo background &); date
```

Ожидаемый смысл результата:

- `echo interactive` попадёт на интерактивный сервер;
- `(echo background &)` попадёт на неинтерактивный сервер;
- `date` снова попадёт на интерактивный сервер.

Пример вывода:

```text
[interactive-server] echo interactive
interactive
[background-server] echo background
background
[interactive-server] date
Sat May 16 12:00:00 UTC 2026
```

---

### 5.3.3. Проверка истории команд

Ввод:

```text
echo one; echo two; history
```

Пример результата:

```text
[interactive-server] echo one
one
[interactive-server] echo two
two
1: echo one
2: echo two
```

---

### 5.3.4. Проверка семафоров в системе

Во время работы программы в отдельном терминале можно посмотреть POSIX-семафоры.

На Linux POSIX-семафоры часто отображаются в `/dev/shm` как файлы вида `sem.*`:

```bash
ls -l /dev/shm | grep lab14
```

Ожидаемый пример:

```text
sem.lab14_sem0get
sem.lab14_sem1get
sem.lab14_sem0end
sem.lab14_sem1end
```

После завершения программы `exit` семафоры удаляются через `sem_unlink()`.

---

## 5.4. Отладка ЛР14 в gdb

### 5.4.1. Отладка родительского процесса

```bash
gdb ./myshell_sem_pipe
```

Внутри `gdb`:

```gdb
set follow-fork-mode parent
break main
break parse
run
```

После запуска введите:

```text
echo debug; (echo bg &); history
```

Полезные команды:

```gdb
next
print line
print history.size()
continue
```

Что показать в отчёте:

- создание канала `pipe(cmdpipe)`;
- создание семафоров через `sem_open()`;
- создание серверов через `fork()`;
- разбор строки в `parse()`;
- запись команды в канал через `write()`;
- выбор семафора `sem0get` или `sem1get`;
- ожидание завершения через `sem0end` или `sem1end`.

---

### 5.4.2. Отладка дочернего серверного процесса

```bash
gdb ./myshell_sem_pipe
```

Внутри `gdb`:

```gdb
set follow-fork-mode child
break server_loop
break execute
run
```

После запуска введите:

```text
echo child_debug
```

Что анализировать:

```gdb
next
print name
print buf
print cmd
continue
```

Что показать в отчёте:

- сервер блокируется на `sem_wait(sem_get)`;
- после `sem_post()` со стороны родителя сервер выходит из ожидания;
- сервер читает команду из канала через `read()`;
- сервер выполняет команду через `execute()`;
- после выполнения сервер вызывает `sem_post(sem_end)`.

---

### 5.4.3. Проверка различия `sem0end` и `sem1end`

В программе используются два отдельных семафора завершения:

```cpp
static const char* SEM0END="/lab14_sem0end";
static const char* SEM1END="/lab14_sem1end";
```

Смысл:

- `sem0end` сообщает родителю, что интерактивный сервер завершил команду;
- `sem1end` сообщает родителю, что неинтерактивный сервер завершил команду.

Для демонстрации в отчёте можно выполнить:

```text
echo interactive; (sleep 1 &); echo after
```

В `gdb` удобно поставить точки останова:

```gdb
break server_loop
break execute
```

И показать, что для разных команд используется разный сервер.

---

# 6. Рекомендуемые скриншоты для отчёта

Для каждой лабораторной работы желательно сделать такие скриншоты:

## 6.1. ЛР11

1. Компиляция `myshell_process.cpp`.
2. Запуск `myshell_process` с командами `date; pwd; ls`.
3. Проверка перенаправления `echo First > result.txt; echo Second >> result.txt; cat result.txt`.
4. Компиляция и запуск `myshell_pipe`.
5. Компиляция и запуск `myshell_fifo`.
6. Отладка `myshell_process` в `gdb`: остановка на `main`, `split_commands`, `execute`.
7. Отладка `myshell_pipe`/`myshell_fifo`: создание канала, `fork()`, чтение команды.

## 6.2. ЛР12

1. Компиляция программ `sem_create`, `sem_wait`, `sem_post`, `sem_getvalue`, `sem_unlink`.
2. Создание семафора `./sem_create -i 2 /lab12_test`.
3. Проверка значения через `./sem_getvalue /lab12_test`.
4. Выполнение `sem_wait` и уменьшение значения.
5. Выполнение `sem_post` и увеличение значения.
6. Запуск `producer_consumer_file`.
7. Запуск `producer_consumer_ring`.
8. Отладка в `gdb` с остановкой на `sem_open`, `sem_wait`, `sem_post` или соответствующих строках.

## 6.3. ЛР13

1. Компиляция `myshell_threads.cpp`.
2. Запуск обычных команд `date; pwd; echo hello`.
3. Запуск фоновых команд `sleep 1 &; echo interactive_command; date`.
4. Отладка в `gdb`: `break parse`, `break exec_command`, `info threads`.
5. Скриншот значения `MAXSERVERS == MAXQUEUE` в коде.

## 6.4. ЛР14

1. Компиляция `myshell_sem_pipe.cpp`.
2. Запуск интерактивных команд `date; pwd; echo lab14`.
3. Запуск смешанных команд `echo interactive; (echo background &); date`.
4. Проверка `history`.
5. Проверка `/dev/shm | grep lab14` во время работы программы.
6. Отладка родительского процесса: `pipe()`, `sem_open()`, `fork()`, `write()`, `sem_post()`.
7. Отладка серверного процесса: `sem_wait()`, `read()`, `execute()`, `sem_post(sem_end)`.

---

# 7. Частые проблемы и решения

## 7.1. Ошибка `g++: command not found`

Установите компилятор:

```bash
sudo apt update
sudo apt install -y build-essential
```

## 7.2. Ошибка `gdb: command not found`

Установите отладчик:

```bash
sudo apt install -y gdb
```

## 7.3. Программа с семафорами сообщает `File exists`

Если семафор уже был создан ранее, удалите его:

```bash
./sem_unlink /lab12_test
```

Для ЛР14 можно очистить семафоры так:

```bash
rm -f /dev/shm/sem.lab14_sem0get /dev/shm/sem.lab14_sem1get /dev/shm/sem.lab14_sem0end /dev/shm/sem.lab14_sem1end
```

Обычно сама программа ЛР14 вызывает `sem_unlink()`, но ручная очистка полезна после аварийного завершения.

## 7.4. Программа зависла на `sem_wait()`

Это нормальное поведение, если значение семафора равно `0`. Чтобы разблокировать ожидание, выполните `sem_post()` из другого терминала:

```bash
./sem_post /имя_семафора
```

## 7.5. FIFO остался в `/tmp`

Если программа ЛР11 с FIFO была аварийно завершена, удалите файл вручную:

```bash
rm -f /tmp/lab11_cmd_fifo
```

## 7.6. В gdb неудобно отлаживать `fork()`

Используйте:

```gdb
set follow-fork-mode child
```

или:

```gdb
set follow-fork-mode parent
```

Для сохранения обоих процессов под контролем `gdb` можно использовать:

```gdb
set detach-on-fork off
info inferiors
inferior 2
```

---

# 8. Быстрый сценарий проверки всех лабораторных

Из корня проекта:

```bash
cd unix_labs_cpp
```

Компиляция ЛР11:

```bash
cd lab11
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_process.cpp -o myshell_process
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_pipe.cpp -o myshell_pipe
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_fifo.cpp -o myshell_fifo
cd ..
```

Компиляция ЛР12:

```bash
cd lab12
g++ -g -O0 -Wall -Wextra -std=c++17 sem_create.cpp -o sem_create -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_unlink.cpp -o sem_unlink -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_getvalue.cpp -o sem_getvalue -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_wait.cpp -o sem_wait -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 sem_post.cpp -o sem_post -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_file.cpp -o producer_consumer_file -pthread
g++ -g -O0 -Wall -Wextra -std=c++17 producer_consumer_ring.cpp -o producer_consumer_ring -pthread
cd ..
```

Компиляция ЛР13:

```bash
cd lab13
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_threads.cpp -o myshell_threads -pthread
cd ..
```

Компиляция ЛР14:

```bash
cd lab14
g++ -g -O0 -Wall -Wextra -std=c++17 myshell_sem_pipe.cpp -o myshell_sem_pipe -pthread
cd ..
```

Проверка моделей ЛР12:

```bash
cd lab12
./producer_consumer_file
./producer_consumer_ring
cd ..
```

---

# 9. Что можно написать в выводах по лабораторным

## ЛР11

В ходе работы была реализована простейшая командная оболочка Linux на базе процессов. Были изучены системные вызовы `fork()`, `execvp()`, `waitpid()`, `dup2()`, `pipe()` и `mkfifo()`. Реализована передача команд между процессами через неименованный и именованный программные каналы.

## ЛР12

Были изучены POSIX-семафоры, операции `sem_open()`, `sem_wait()`, `sem_post()`, `sem_getvalue()`, `sem_close()` и `sem_unlink()`. На практике проверена синхронизация процессов и потоков на примере модели «производитель–потребитель».

## ЛР13

Была реализована оболочка, использующая процессы и программные потоки. Неинтерактивные команды запускаются через серверы-потоки, что позволяет обрабатывать несколько команд параллельно. Условие `MAXSERVERS == MAXQUEUE` позволяет выделить отдельный поток на каждую неинтерактивную команду из очереди.

## ЛР14

Была реализована оболочка, объединяющая процессы, программные каналы и POSIX-семафоры. Команды передаются серверным процессам через `pipe()`, а синхронизация выполняется через отдельные семафоры поступления и завершения команд. Вариант с отдельными семафорами `sem0end` и `sem1end` позволяет явно различать завершение интерактивного и неинтерактивного серверов.
