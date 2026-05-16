// ЛР11. Вариант 2: передача команд от родителя к дочернему процессу через pipe().
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

static std::string trim(std::string s)
{
    auto ns = [](unsigned char c)
    { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), ns));
    s.erase(std::find_if(s.rbegin(), s.rend(), ns).base(), s.end());
    return s;
}
static std::vector<std::string> split(const std::string &line)
{
    std::vector<std::string> r;
    std::string c;
    for (char ch : line)
    {
        if (ch == ';')
        {
            if (!trim(c).empty())
                r.push_back(trim(c));
            c.clear();
        }
        else
            c.push_back(ch);
    }
    if (!trim(c).empty())
        r.push_back(trim(c));
    return r;
}
static std::vector<std::string> tok(const std::string &cmd)
{
    std::istringstream ss(cmd);
    std::vector<std::string> a;
    std::string x;
    while (ss >> x)
        a.push_back(x);
    return a;
}
static void run_cmd(const std::string &cmd)
{
    auto args = tok(cmd);
    if (args.empty())
        return;
    if (args[0] == "exit")
        _exit(0);
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
int main()
{
    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("pipe");
        return 1;
    }
    pid_t worker = fork();
    if (worker < 0)
    {
        perror("fork");
        return 1;
    }
    if (worker == 0)
    {
        close(fd[1]);
        FILE *in = fdopen(fd[0], "r");
        char *line = nullptr;
        size_t n = 0;
        while (getline(&line, &n, in) != -1)
        {
            std::string cmd(line);
            if (!cmd.empty() && cmd.back() == '\n')
                cmd.pop_back();
            run_cmd(cmd);
        }
        free(line);
        return 0;
    }
    close(fd[0]);
    FILE *out = fdopen(fd[1], "w");
    std::string line;
    while (std::cout << "$> " << std::flush, std::getline(std::cin, line))
    {
        for (auto &c : split(line))
            fprintf(out, "%s\n", c.c_str());
        fflush(out);
        if (trim(line) == "exit")
            break;
    }
    fclose(out);
    int st;
    waitpid(worker, &st, 0);
}
