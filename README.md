# Исходный код C++ для лабораторных работ Unix №11–14

Папки:
- `lab11` — процессы, `pipe()`, FIFO.
- `lab12` — POSIX-семафоры и модели «производитель–потребитель».
- `lab13` — оболочка на процессах и потоках, `MAXSERVERS == MAXQUEUE`.
- `lab14` — оболочка на процессах, `pipe()` и POSIX-семафорах `sem0end`/`sem1end`.

## Подготовка Linux-среды
```bash
sudo apt update
sudo apt install -y build-essential gdb
```

## Общая проверка
Перейдите в нужную папку и выполните команды из локального `README.md`.

## Примечание
Код написан на C++17 и рассчитан на Linux. На Windows напрямую не запустится, так как используются POSIX-вызовы: `fork`, `execvp`, `pipe`, `mkfifo`, `sem_open`, `sem_wait`, `sem_post`.
