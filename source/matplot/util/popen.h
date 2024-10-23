//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_POPEN_H
#define MATPLOTPLUSPLUS_POPEN_H

#include <string>
#include <string_view>
#include <cstdio> // FILE

#ifdef _WIN32

#include <windows.h> // HANDLE

/// State of a child process pipe
class proc_pipe 
{
protected:
    proc_pipe() = default;
    HANDLE hProcess = 0; ///< WIN32 process handle
    HANDLE hThread = 0; ///< WIN32 thread handle
    FILE *file_ = nullptr; ///< C file handle for I/O (not both)
};

#elif defined(__linux) || defined(__APPLE__)

#include <unistd.h> // pid_t

/// State of a child process pipe
class proc_pipe
{
protected:
    proc_pipe() = default;
    pid_t pid = 0;         ///< POSIX process identifier
    FILE *file_ = nullptr; ///< C file handle for input or output (not both)
};

#else
#error "proc_pipe is not implemented for this platform"
#endif // platform specific code

/// Common operations for a child process pipe
class common_pipe : public proc_pipe 
{
public:
    FILE *file() const { return file_; }
    int close(int *exit_code = nullptr);
    bool opened() const { return file_ != nullptr; }
    const std::string& error() const { return error_; }
    bool exceptions() const { return exceptions_; }
    void exceptions(bool exc) { exceptions_ = exc; }

protected:
    common_pipe() = default;
    common_pipe(const common_pipe&) = delete;
    common_pipe& operator=(const common_pipe&) = delete;
    common_pipe(common_pipe&&) noexcept = default;
    common_pipe& operator=(common_pipe&&) noexcept = default;
    ~common_pipe() noexcept {
        if (opened())
            close();
    }
    bool exceptions_ = false;
    std::string error_{};
    int report(int err, const std::string& what);
    int open(const std::string &command, char mode);
};

/// Pipe to read from process output
class ipipe : public common_pipe 
{
public:
    ipipe();
    int open(const std::string &cmd) { return common_pipe::open(cmd, 'r'); }
    int read(std::string &data);
};

/// Pipe to write into process input
class opipe : public common_pipe 
{
public:
    opipe();
    int open(const std::string &cmd) { return common_pipe::open(cmd, 'w'); }
    int write(std::string_view data);
    int flush(std::string_view data = {});
};

/// Runs the shell command, writes data to its input and waits for completion, returns errno if failed
int shell_write(const std::string &command, std::string_view data = {});

/// Runs the shell command, reads data from its output and waits for completion, returns errno if failed
int shell_read(const std::string &command, std::string& data);

#endif // MATPLOTPLUSPLUS_POPEN_H
