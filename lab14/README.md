# Лабораторная работа №14

## Файл
- `myshell_sem_pipe.cpp` — оболочка с двумя серверными процессами, каналом `pipe()` и POSIX-семафорами. Вместо общего семафора завершения используется два семафора: `sem0end` и `sem1end`.

## Компиляция
```bash
g++ -std=c++17 -Wall -Wextra -g myshell_sem_pipe.cpp -o myshell_sem_pipe -pthread
```

## Запуск
```bash
./myshell_sem_pipe
$> date; pwd; (ls &); echo ok; history
$> exit
```

## Отладка через gdb
```bash
gdb ./myshell_sem_pipe
(gdb) set follow-fork-mode child
(gdb) break server_loop
(gdb) run
(gdb) continue
```

## Экспериментальная оценка
Для грубой оценки можно выполнить серию коротких команд и измерить время:
```bash
time bash -c 'for i in {1..20}; do echo "echo test"; done | ./myshell_sem_pipe'
```
Сравните результат с версией, где один общий `sem01end`; ожидаемая разница обычно мала, но отдельные семафоры убирают неоднозначность того, какой сервер завершил команду.
