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
int pipe_open(ProcPipe *pipe, const char *command, const char *mode);

/// Return true if pipe represents a valid handle
inline bool pipe_is_valid(ProcPipe *pipe)
{
    return pipe != nullptr && pipe->file != nullptr;
}

/** Closes the pipe opened by popen2 and waits for the process to terminate.
 * @param pipe the handle to close and wait for exit
 * @param exit_code to store the process exit code
 * @returns 0 upon success and system error number (errno) upon failure
 */
int pipe_close(ProcPipe *pipe, int *exit_code = nullptr);

#ifdef __cplusplus

#include <string>
#include <string_view>

/// Return true if pipe represents a valid handle
inline bool pipe_is_valid(ProcPipe &pipe) 
{
    return pipe.file != nullptr;
}

/// Writes data into the process pipe without flushing, returns errno if failed
int pipe_write(ProcPipe *pipe, std::string_view data);

/// Writes data into the process pipe followed by a flush, returns errno if failed
int pipe_flush(ProcPipe *pipe, std::string_view data = {});

/// Launches the shell command, writes data to input stream and waits for completion, returns errno if failed
int shell_write(const std::string &command, std::string_view data = {});

/// Launches the shell command, reads data from output stream and waits for completion, returns errno if failed
int shell_read(const std::string &command, std::string& data);

#endif // __cplusplus

#endif // MATPLOTPLUSPLUS_POPEN_H
