#include "shell.hh"

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

std::string load_prompt(const char *filename) {
    int fd = syscall(SYS_open, filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t n = syscall(SYS_read, fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    syscall(SYS_close, fd);
    return std::string(buffer);
}

void new_file() {
    std::string filename;
    std::string content;
    std::string line;

    std::cout << "Enter filename: ";
    std::getline(std::cin, filename);
    std::cout << "Enter file content (end with <EOF>):\n";
    
    while (std::getline(std::cin, line)) {
        if (line == "<EOF>") break;
        content += line + "\n";
    }

    int fd = syscall(SYS_creat, filename.c_str(), 0644);
    if (fd < 0) {
        perror("creat");
        return;
    }

    if (syscall(SYS_write, fd, content.c_str(), content.size()) < 0) {
        perror("write");
    }

    syscall(SYS_close, fd);
}

void list() {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("ls", "ls", "-la", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
    }
}

void find() {
    std::string search_string;
    std::cout << "Enter search string: ";
    std::getline(std::cin, search_string);

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("find", "find", ".", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("grep", "grep", search_string.c_str(), NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}

void seekTest() {
    int fd_seek = syscall(SYS_creat, "seek", 0644);
    int fd_loop = syscall(SYS_creat, "loop", 0644);

    if (fd_seek < 0 || fd_loop < 0) {
        perror("creat");
        return;
    }

    syscall(SYS_write, fd_seek, "x", 1);
    syscall(SYS_lseek, fd_seek, 5 * 1024 * 1024, SEEK_SET);
    syscall(SYS_write, fd_seek, "x", 1);
    syscall(SYS_close, fd_seek);

    syscall(SYS_write, fd_loop, "x", 1);
    for (int i = 0; i < 5 * 1024 * 1024; ++i) {
        syscall(SYS_write, fd_loop, "\0", 1);
    }
    syscall(SYS_write, fd_loop, "x", 1);
    syscall(SYS_close, fd_loop);

    list();
}

int main() {
    std::string input;
    std::string prompt = load_prompt("prompt.txt");

    while(true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input == "new_file") new_file();
        else if (input == "ls") list();
        else if (input == "find") find();
        else if (input == "seekTest") seekTest();
        else if (input == "exit" || input == "quit") return 0;
        else std::cout << "Unknown command: " << input << std::endl;

        if (std::cin.eof()) return 0;
    }
}
