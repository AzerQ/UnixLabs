# Лабораторная работа №11

## Файлы
- `myshell_process.cpp` — простейшая оболочка на базе `fork/execvp`, поддерживает `;`, `<`, `>`, `>>`, `cd`, `exit`, `showenv`.
- `myshell_pipe.cpp` — передача команд дочернему процессу через неименованный канал `pipe()`.
- `myshell_fifo.cpp` — передача команд дочернему процессу через именованный канал `FIFO`.

## Компиляция
```bash
g++ -std=c++17 -Wall -Wextra -g myshell_process.cpp -o myshell_process
g++ -std=c++17 -Wall -Wextra -g myshell_pipe.cpp -o myshell_pipe
g++ -std=c++17 -Wall -Wextra -g myshell_fifo.cpp -o myshell_fifo
```

## Запуск и проверка
```bash
./myshell_process
$> date; pwd; ls
$> echo hello > out.txt; cat out.txt
$> showenv
$> exit

./myshell_pipe
$> date; pwd; echo hello
$> exit

./myshell_fifo
$> date; pwd; echo hello
$> exit
```

## Отладка через gdb
```bash
gdb ./myshell_process
(gdb) break main
(gdb) run
(gdb) next
(gdb) continue
```
