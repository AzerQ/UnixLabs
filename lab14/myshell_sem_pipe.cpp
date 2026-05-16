// ЛР14: оболочка на базе процессов, программных каналов и POSIX-семафоров.
// Вариант с двумя отдельными семафорами завершения: /lab14_sem0end и /lab14_sem1end.
#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
constexpr size_t CMD_SIZE = 512;
static const char *SEM0GET = "/lab14_sem0get";
static const char *SEM1GET = "/lab14_sem1get";
static const char *SEM0END = "/lab14_sem0end";
static const char *SEM1END = "/lab14_sem1end";
static std::string trim(std::string s)
{
    auto ns = [](unsigned char c)
    { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), ns));
    s.erase(std::find_if(s.rbegin(), s.rend(), ns).base(), s.end());
    return s;
}
struct Command
{
    std::string text;
    bool noninteractive = false;
};
static std::vector<Command> parse(const std::string &line)
{
    std::vector<Command> r;
    std::string cur;
    for (char ch : line)
    {
        if (ch == ';')
        {
            auto t = trim(cur);
            if (!t.empty())
            {
                bool bg = false;
                if (t.front() == '(' && t.back() == ')')
                    t = trim(t.substr(1, t.size() - 2));
                if (!t.empty() && t.back() == '&')
                {
                    bg = true;
                    t = trim(t.substr(0, t.size() - 1));
                }
                r.push_back({t, bg});
            }
            cur.clear();
        }
        else
            cur.push_back(ch);
    }
    auto t = trim(cur);
    if (!t.empty())
    {
        bool bg = false;
        if (t.front() == '(' && t.back() == ')')
            t = trim(t.substr(1, t.size() - 2));
        if (!t.empty() && t.back() == '&')
        {
            bg = true;
            t = trim(t.substr(0, t.size() - 1));
        }
        r.push_back({t, bg});
    }
    return r;
}
static std::vector<std::string> tokenize(const std::string &cmd)
{
    std::istringstream ss(cmd);
    std::vector<std::string> a;
    std::string s;
    while (ss >> s)
        a.push_back(s);
    return a;
}
static void execute(const std::string &cmd)
{
    auto args = tokenize(cmd);
    if (args.empty())
        return;
    pid_t p = fork();
    if (p == 0)
    {
        std::vector<char *> av;
        for (auto &s : args)
            av.push_back(s.data());
        av.push_back(nullptr);
        execvp(av[0], av.data());
        perror("execvp");
        _exit(127);
    }
    int st;
    waitpid(p, &st, 0);
}
static void server_loop(int read_fd, sem_t *sem_get, sem_t *sem_end, const char *name)
{
    while (true)
    {
        sem_wait(sem_get);
        char buf[CMD_SIZE]{};
        ssize_t n = read(read_fd, buf, sizeof(buf));
        if (n <= 0)
            continue;
        std::string cmd(buf);
        std::cout << '[' << name << "] " << cmd << '\n';
        execute(cmd);
        sem_post(sem_end);
    }
}
static void sem_cleanup()
{
    sem_unlink(SEM0GET);
    sem_unlink(SEM1GET);
    sem_unlink(SEM0END);
    sem_unlink(SEM1END);
}
int main()
{
    int cmdpipe[2];
    if (pipe(cmdpipe) < 0)
    {
        perror("pipe");
        return 1;
    }
    sem_cleanup();
    sem_t *sem0get = sem_open(SEM0GET, O_CREAT | O_RDWR, 0644, 0);
    sem_t *sem1get = sem_open(SEM1GET, O_CREAT | O_RDWR, 0644, 0);
    sem_t *sem0end = sem_open(SEM0END, O_CREAT | O_RDWR, 0644, 0);
    sem_t *sem1end = sem_open(SEM1END, O_CREAT | O_RDWR, 0644, 0);
    if (sem0get == SEM_FAILED || sem1get == SEM_FAILED || sem0end == SEM_FAILED || sem1end == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }
    pid_t s0 = fork();
    if (s0 == 0)
    {
        close(cmdpipe[1]);
        server_loop(cmdpipe[0], sem0get, sem0end, "interactive-server");
    }
    pid_t s1 = fork();
    if (s1 == 0)
    {
        close(cmdpipe[1]);
        server_loop(cmdpipe[0], sem1get, sem1end, "background-server");
    }
    close(cmdpipe[0]);
    std::vector<std::string> history;
    std::string line;
    while (std::cout << "$> " << std::flush, std::getline(std::cin, line))
    {
        if (trim(line) == "exit")
            break;
        if (trim(line) == "history")
        {
            for (size_t i = 0; i < history.size(); ++i)
                std::cout << i + 1 << ": " << history[i] << '\n';
            continue;
        }
        for (const auto &c : parse(line))
        {
            history.push_back(c.text + (c.noninteractive ? " &" : ""));
            char packet[CMD_SIZE]{};
            std::snprintf(packet, sizeof(packet), "%s", c.text.c_str());
            if (write(cmdpipe[1], packet, sizeof(packet)) != (ssize_t)sizeof(packet))
                perror("write");
            if (c.noninteractive)
            {
                sem_post(sem1get);
                sem_wait(sem1end);
            }
            else
            {
                sem_post(sem0get);
                sem_wait(sem0end);
            }
        }
    }
    kill(s0, SIGTERM);
    kill(s1, SIGTERM);
    int st;
    waitpid(s0, &st, 0);
    waitpid(s1, &st, 0);
    close(cmdpipe[1]);
    sem_cleanup();
}
