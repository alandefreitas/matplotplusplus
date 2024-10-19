#include <matplot/util/popen.h>
#include <array>

#ifdef _WIN32

#include <io.h>

int pipe_open(ProcPipe *pipe, const char *cmd, const char *mode) 
{
    if (cmd == nullptr || mode == nullptr || (mode[0] != 'r' && mode[0] != 'w') ||
        pipe == nullptr)
        return EINVAL;
    HANDLE hChildStdinRd, hChildStdinWr, hStdin, hStdout;
    SECURITY_ATTRIBUTES saAttr{};
    // Set up security attributes for inheritable handles
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's input
    if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
        return GetLastError();

    // Ensure the write handle to the pipe is not inherited by child
    // processes
    if (!SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        return err;
    }

    // Create a pipe for the child process's output
    if (!CreatePipe(&hStdin, &hStdout, &saAttr, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        return err;
    }

    // Ensure the read handle to the output pipe is not inherited by child
    // processes
    if (!SetHandleInformation(hStdout, HANDLE_FLAG_INHERIT, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        return err;
    }

    // Configure STARTUPINFO structure for the new process
    STARTUPINFO si{};
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = hStdout;
    si.hStdOutput = hStdout;
    si.hStdInput = hChildStdinRd;
    si.dwFlags |= STARTF_USESTDHANDLES;
    PROCESS_INFORMATION pi;

    // Create the child process, while hiding the window
    if (!CreateProcess(NULL, const_cast<char *>(cmd), NULL, NULL, TRUE,
                       CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        CloseHandle(hStdout);
        return err;
    }
    pipe->hProcess = pi.hProcess;
    pipe->hThread = pi.hThread;

    // Close unnecessary handles
    CloseHandle(hChildStdinRd);
    CloseHandle(hStdout);

    // Create a FILE pointer from the write handle to the child process's
    // input
    pipe->file = _fdopen(_open_osfhandle((intptr_t)hChildStdinWr, 0), mode);
    if (pipe->file == nullptr) {
        auto err = GetLastError();
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return err;
    }
    return 0;
}

int pipe_close(ProcPipe *pipe, int *exit_code)
{
    // The following does not work for GetExitCodeProcess:
    // HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
    if (!pipe_is_valid(pipe))
        return EINVAL;
    // Close the pipe to process:
    fclose(pipe->file);
    pipe->file = nullptr;
    // Wait for the process to finish
    if (auto r = WaitForSingleObject(pipe->hProcess, INFINITE); r != WAIT_OBJECT_0) {
        CloseHandle(pipe->hThread);
        CloseHandle(pipe->hProcess);
        return ECHILD;
    }
    // Retrieve the exit code
    if (exit_code != nullptr) {
        if (BOOL r = GetExitCodeProcess(pipe->hProcess, (LPDWORD)exit_code); !r) {
            auto err = GetLastError();
            CloseHandle(pipe->hThread);
            CloseHandle(pipe->hProcess);
            return err;
        }
    }
    CloseHandle(pipe->hThread);
    CloseHandle(pipe->hProcess);
    return 0;
}
#endif // _WIN32 implementtion

#if defined(__linux) || defined(__APPLE__)

#include <cerrno>
#include <sys/wait.h> // waitpid

int pipe_open(ProcPipe *p, const char *cmd, const char *mode) 
{
    constexpr auto READ = 0u;
    constexpr auto WRITE = 1u;
    if (cmd == nullptr || mode == nullptr || (mode[0] != 'r' && mode[0] != 'w') || p == nullptr)
        return EINVAL;

    int fd[2];
    if (pipe(fd) == -1)
        return errno;
    if ((p->pid = fork()) == -1)
        return errno;

    if (p->pid == 0) { // child process
        if (mode[0] == 'r') {
            close(fd[READ]);    // Close the READ end of the pipe
            dup2(fd[WRITE], 1); // Redirect stdout to pipe
        } else {                // (mode[0] == 'w')
            close(fd[WRITE]);   // Close the WRITE end of the pipe
            dup2(fd[READ], 0);  // Redirect stdin to pipe
        }
        setpgid(p->pid, p->pid); // Needed so negative PIDs can kill children of /bin/sh
        execl("/bin/sh", "/bin/sh", "-c", cmd, nullptr); // returns only upon errors
        return errno;
    } else {
        if (mode[0] == 'r') {
            close(fd[WRITE]); // Close the WRITE end of the pipe since parent's
                              // fd is read-only
        } else {              // (mode[0] == 'w')
            close(fd[READ]); // Close the READ end of the pipe since parent's fd
                             // is write-only
        }
    }
    p->file = (mode[0] == 'r') ? fdopen(fd[READ], "r") : fdopen(fd[WRITE], "w");
    return 0;
}

/// Closes the pipe opened by popen2 and waits for termination
int pipe_close(ProcPipe *pipe, int *exit_code)
{
    if (!proc_is_good(pipe))
        return EINVAL;
    fclose(pipe->file);
    while (waitpid(pipe->pid, exit_code, 0) == -1) {
        if (errno != EINTR) {
            *exit_code = -1;
            break;
        }
    }
    pipe->file = nullptr;
    return 0;
}

#endif // POSIX implementation

int pipe_write(ProcPipe *p, std::string_view data) 
{
    constexpr auto CSIZE = sizeof(std::string_view::value_type);
    if (!pipe_is_valid(p))
        return EINVAL;
    if (auto sz = std::fwrite(data.data(), CSIZE, data.length(), p->file);
        sz != data.size()) {
        if (auto err = std::ferror(p->file); err != 0)
            return EIO;
        if (auto err = std::feof(p->file); err != 0)
            return EIO;
    }
    if (auto res = std::fflush(p->file); res != 0)
        return errno;
    return 0;
}

int pipe_flush(ProcPipe *p, std::string_view data) 
{
    if (!pipe_is_valid(p))
        return EINVAL;
    if (!data.empty())
        if (auto err = pipe_write(p, data); err != 0)
            return err;
    if (auto res = std::fflush(p->file); res != 0)
        return errno;
    return 0;
}

int shell_write(const std::string &cmd, std::string_view data)
{
    auto pipe = ProcPipe{};
    if (auto err = pipe_open(&pipe, cmd.c_str(), "w"); err != 0)
        return err;
    if (!data.empty())
        if (auto err = pipe_flush(&pipe, data); err != 0)
            return err;
    if (auto err = pipe_close(&pipe); err != 0)
        return err;
    return 0;
}

int shell_read(const std::string &cmd, std::string &data)
{
    auto pipe = ProcPipe{};
    if (auto err = pipe_open(&pipe, cmd.c_str(), "r"); err != 0)
        return err;
    data.clear();
    auto buffer = std::array<char, 128>{};
    while (!std::feof(pipe.file) && !std::ferror(pipe.file)) {
        auto count = std::fread(buffer.data(), sizeof(char), buffer.size(), pipe.file);
        if (count > 0)
            data.append(buffer.data(), count);
    }
    if (auto err = pipe_close(&pipe); err != 0)
        return err;
    return 0;
}
