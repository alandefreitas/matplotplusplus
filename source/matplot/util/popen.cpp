#include <matplot/util/popen.h>
#include <array>

#ifdef _WIN32

#include <io.h>

int popen2(const char *cmd, const char* mode, ProcPipe *pipe)
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

int pclose2(ProcPipe *pipe, int *exit_code)
{
    // The following does not work for GetExitCodeProcess:
    // HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
    if (pipe == nullptr || pipe->file == nullptr)
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

int popen2(const char *cmd, const char *mode, ProcPipe *p)
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
int pclose2(ProcPipe *pipe, int *exit_code)
{
    if (pipe == nullptr || pipe->file == nullptr)
        return EINVAL;
    fclose(pipe->file);
    while (waitpid(pipe->pid, exit_code, 0) == -1) {
        if (errno != EINTR) {
            *exit_code = -1;
            break;
        }
    }
    return 0;
}

#endif // POSIX implementation

void shell_write(const std::string &cmd, std::string_view input)
{
    auto pipe = ProcPipe{};
    if (auto e = popen2(cmd.c_str(), "w", &pipe); e != 0)
        throw std::system_error{e, std::system_category(), "popen2"};
    if (!input.empty())
        proc_flush(&pipe, input);
    if (auto e = pclose2(&pipe); e != 0)
        throw std::system_error{e, std::system_category(), "pclose2"};
}

std::string shell_read(const std::string &cmd)
{
    auto pipe = ProcPipe{};
    if (auto e = popen2(cmd.c_str(), "r", &pipe); e != 0)
        throw std::system_error{e, std::system_category(), "popen2"};
    auto res = std::string{};
    auto buffer = std::array<char, 128>{};
    while (!feof(pipe.file) && !ferror(pipe.file)) {
        auto count = fread(buffer.data(), sizeof(char), buffer.size(), pipe.file);
        if (count > 0)
            res.append(buffer.data(), count);
    }
    if (auto e = pclose2(&pipe); e != 0)
        throw std::system_error{e, std::system_category(), "pclose2"};
    return res;
}
