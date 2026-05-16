// ЛР13: командная оболочка на базе процессов и программных потоков.
// MAXSERVERS равен MAXQUEUE: для каждой неинтерактивной команды создается отдельный поток.
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
constexpr int MAXQUEUE = 6;
constexpr int MAXSERVERS = MAXQUEUE;
struct Command
{
    std::string text;
    bool background = false;
};
static std::string trim(std::string s)
{
    auto ns = [](unsigned char c)
    { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), ns));
    s.erase(std::find_if(s.rbegin(), s.rend(), ns).base(), s.end());
    return s;
}
static std::vector<Command> parse(const std::string &line)
{
    std::vector<Command> r;
    std::string cur;
    for (char ch : line)
    {
        if (ch == ';')
        {
            std::string t = trim(cur);
            if (!t.empty())
            {
                bool bg = false;
                if (t.back() == '&')
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
    std::string t = trim(cur);
    if (!t.empty())
    {
        bool bg = false;
        if (t.back() == '&')
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
static void exec_command(std::string cmd)
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
    else if (p > 0)
    {
        int st;
        waitpid(p, &st, 0);
    }
    else
        perror("fork");
}
int main()
{
    std::string line;
    while (std::cout << "$> " << std::flush, std::getline(std::cin, line))
    {
        if (trim(line) == "exit")
            break;
        auto commands = parse(line);
        std::vector<std::thread> servers;
        int created = 0;
        for (const auto &c : commands)
        {
            if (c.background && created < MAXSERVERS)
            {
                servers.emplace_back(exec_command, c.text);
                created++;
            }
            else
                exec_command(c.text);
        }
        for (auto &t : servers)
            t.join();
    }
}
