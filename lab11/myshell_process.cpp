// ЛР11. Вариант 1: простейшая командная оболочка на базе процессов.
// Поддержка: команды через ';', запуск через fork/execvp, перенаправления <, >, >>, встроенные cd/exit/showenv.
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

static std::string trim(std::string s){
    auto not_space=[](unsigned char c){return !std::isspace(c);};
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
    return s;
}

static std::vector<std::string> split_commands(const std::string& line){
    std::vector<std::string> result; std::string cur; bool quote=false;
    for(char c: line){
        if(c=='"') quote=!quote;
        if(c==';' && !quote){ if(!trim(cur).empty()) result.push_back(trim(cur)); cur.clear(); }
        else cur.push_back(c);
    }
    if(!trim(cur).empty()) result.push_back(trim(cur));
    return result;
}

static std::vector<std::string> tokenize(const std::string& cmd){
    std::vector<std::string> args; std::string cur; bool quote=false;
    for(size_t i=0;i<cmd.size();++i){
        char c=cmd[i];
        if(c=='"'){ quote=!quote; continue; }
        if(!quote && std::isspace((unsigned char)c)){ if(!cur.empty()){args.push_back(cur); cur.clear();} continue; }
        if(!quote && (c=='<' || c=='>')){
            if(!cur.empty()){args.push_back(cur); cur.clear();}
            if(c=='>' && i+1<cmd.size() && cmd[i+1]=='>'){ args.push_back(">>"); ++i; }
            else args.push_back(std::string(1,c));
            continue;
        }
        cur.push_back(c);
    }
    if(!cur.empty()) args.push_back(cur);
    return args;
}

static int apply_redirections(std::vector<std::string>& args){
    std::vector<std::string> clean;
    for(size_t i=0;i<args.size();++i){
        if(args[i]=="<" || args[i]==">" || args[i]==">>"){
            if(i+1>=args.size()){ std::cerr << "myshell: missing file after " << args[i] << "\n"; return -1; }
            const std::string file=args[++i]; int fd=-1;
            if(args[i-1]=="<") fd=open(file.c_str(), O_RDONLY);
            else fd=open(file.c_str(), O_WRONLY|O_CREAT|(args[i-1]==">>"?O_APPEND:O_TRUNC), 0644);
            if(fd<0){ std::cerr << "myshell: " << file << ": " << strerror(errno) << "\n"; return -1; }
            dup2(fd, args[i-1]=="<" ? STDIN_FILENO : STDOUT_FILENO); close(fd);
        } else clean.push_back(args[i]);
    }
    args.swap(clean); return 0;
}

static void execute(std::vector<std::string> args){
    if(args.empty()) return;
    if(args[0]=="cd") { if(args.size()>1 && chdir(args[1].c_str())<0) perror("cd"); return; }
    if(args[0]=="showenv") { extern char **environ; for(char **e=environ; *e; ++e) std::cout << *e << '\n'; return; }
    pid_t pid=fork();
    if(pid==0){
        if(apply_redirections(args)<0) _exit(2);
        std::vector<char*> argv; for(auto& a: args) argv.push_back(a.data()); argv.push_back(nullptr);
        execvp(argv[0], argv.data()); perror("execvp"); _exit(127);
    } else if(pid>0){ int st; waitpid(pid,&st,0); }
    else perror("fork");
}

int main(){
    std::string line;
    while(std::cout << "$> " << std::flush, std::getline(std::cin,line)){
        if(trim(line)=="exit") break;
        for(const auto& cmd: split_commands(line)) execute(tokenize(cmd));
    }
}
