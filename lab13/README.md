# Лабораторная работа №13

## Файл
- `myshell_threads.cpp` — оболочка с интерактивными и неинтерактивными командами. Команды с `&` выполняются в потоках. В коде `MAXSERVERS == MAXQUEUE`.

## Компиляция
```bash
g++ -std=c++17 -Wall -Wextra -g myshell_threads.cpp -o myshell_threads -pthread
```

## Запуск
```bash
./myshell_threads
$> date; pwd; sleep 1 &; echo done
$> ls -l &; uname -a &; whoami
$> exit
```

## Отладка через gdb
```bash
gdb ./myshell_threads
(gdb) break exec_command
(gdb) run
(gdb) info threads
(gdb) thread apply all bt
(gdb) continue
```
