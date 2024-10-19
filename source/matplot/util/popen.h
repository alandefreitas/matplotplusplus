//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_POPEN_H
#define MATPLOTPLUSPLUS_POPEN_H

#include <cstdio> // FILE

#ifdef _WIN32

#include <windows.h> // HANDLE

/// Pipe to a child process for either input or output
struct ProcPipe
{
    HANDLE hProcess; ///< WIN32 process handle
    HANDLE hThread; ///< WIN32 thread handle
    FILE *file = nullptr; ///< C file handle for input or output (not both)
};

#elif defined(__linux) || defined(__APPLE__)

#include <unistd.h> // pid_t

/// Pipe to a child process for either input or output
struct ProcPipe
{
    pid_t pid; ///< POSIX process handle
    FILE *file = nullptr; ///< C file handle for input or output (not both)
};

#else
#error "ProcPipe is not implemented for this platform"
#endif // platform specific code

/** Runs native shell command similar to popen(3), except:
 * 1) _popen pops a cmd.exe window on Windows. This version does not.
 * 2) includes process handle for extra signaling process data keeping.
 * 3) outputs results to a pointer and returns errors.
 * @param command to be passed to a shell
 * @param mode 'r' for reading output, 'w' for writing input
 * @param pipe the piped file and child process handle
 * @returns 0 upon success and system error number upon failure
 */
int popen2(const char *command, const char *mode, ProcPipe *pipe);

/** Closes the pipe opened by popen2 and waits for the process to terminate.
 * @param pipe the handle to close and wait for exit
 * @param exit_code to store the process exit code
 * @returns 0 upon success and system error number (errno) upon failure
 */
int pclose2(ProcPipe *pipe, int *exit_code = nullptr);

#ifdef __cplusplus

#include <string>
#include <string_view>
#include <system_error>

/// Writes data into the process pipe without flushing
inline void proc_write(ProcPipe *p, std::string_view data)
{
    constexpr auto CSIZE = sizeof(std::string_view::value_type);
    if (auto sz = fwrite(data.data(), CSIZE, data.size(), p->file); sz != data.size()) {
        if (auto e = ferror(p->file); e != 0)
            throw std::runtime_error{"fwrite failed after "+std::to_string(sz)};
        if (auto e = feof(p->file); e != 0)
            throw std::runtime_error{"fwrite reached end-of-file after "+std::to_string(sz)};
    }
    if (auto e = fflush(p->file); e != 0)
        throw std::system_error{errno, std::system_category(), "fflush"};
}

/// Writes data into process pipe followed by a flush
inline void proc_flush(ProcPipe *p, std::string_view data = {})
{
    if (!data.empty())
        proc_write(p, data);
    if (auto e = fflush(p->file); e != 0)
        throw std::system_error{errno, std::system_category(), "fflush"};
}

/// Launches a shell command, writes data to its input stream and waits for completion.
void shell_write(const std::string &cmd, std::string_view data = {});

/// Launches a shell command, reads data from its output stream and waits for completion.
std::string shell_read(const std::string &cmd);

#endif // __cplusplus

#endif // MATPLOTPLUSPLUS_POPEN_H
