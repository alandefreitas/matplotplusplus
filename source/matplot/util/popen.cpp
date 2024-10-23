#define __STDC_WANT_LIB_EXT1__ 1
#include <matplot/util/popen.h>
#include <array>
#include <system_error>
#include <cstring> // strerror

#ifdef _WIN32

#include <io.h>

int common_pipe::open(const std::string& cmd, char mode)
{
    if (opened())
        close(); // prevent resource leak
    HANDLE hChildStdinRd, hChildStdinWr, hStdin, hStdout;
    SECURITY_ATTRIBUTES saAttr{};
    // Set up security attributes for inheritable handles
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's input
    if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))
        return report(GetLastError(), "CreatePipe");

    // Ensure the write handle to the pipe is not inherited by child
    // processes
    if (!SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        return report(err, "SetHandleInformation(hChildStdinWr)");
    }

    // Create a pipe for the child process's output
    if (!CreatePipe(&hStdin, &hStdout, &saAttr, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        return report(err, "CreatePipe");
    }

    // Ensure the read handle to the output pipe is not inherited by child
    // processes
    if (!SetHandleInformation(hStdout, HANDLE_FLAG_INHERIT, 0)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        return report(err, "SetHandleInformation(hStdout)");
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
    if (!CreateProcess(NULL, const_cast<char *>(cmd.c_str()), NULL, NULL, TRUE,
                       CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        auto err = GetLastError();
        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        CloseHandle(hStdout);
        return report(err, "CreateProcess");
    }
    hProcess = pi.hProcess;
    hThread = pi.hThread;

    // Close unnecessary handles
    CloseHandle(hChildStdinRd);
    CloseHandle(hStdout);

    // Create a FILE pointer from the write handle to the child process's
    // input
    if (mode == 'r')
        file_ = _fdopen(_open_osfhandle((intptr_t)hChildStdinWr, 0), "r");
    else
        file_ = _fdopen(_open_osfhandle((intptr_t)hChildStdinWr, 0), "w");
    if (file_ == nullptr) {
        auto err = GetLastError();
        CloseHandle(hChildStdinWr);
        CloseHandle(hStdin);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return report(err, "_fdopen(_open_osfhandle)");
    }
    return 0;
}

int common_pipe::close(int *exit_code)
{
    if (!opened())
        return 0; // nothing to do
    // Close the pipe to process:
    fclose(file_);
    file_ = nullptr;
    // Wait for the process to finish
    if (auto r = WaitForSingleObject(hProcess, INFINITE); r != WAIT_OBJECT_0) {
        CloseHandle(hThread);
        CloseHandle(hProcess);
        return ECHILD;
    }
    // Retrieve the exit code
    if (exit_code != nullptr) {
        if (BOOL r = GetExitCodeProcess(hProcess, (LPDWORD)exit_code); !r) {
            auto err = GetLastError();
            CloseHandle(hThread);
            CloseHandle(hProcess);
            return err;
        }
    }
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return 0;
}
#endif // _WIN32 implementtion

#if defined(__linux) || defined(__APPLE__)

#include <cerrno>
#include <sys/wait.h> // waitpid

int common_pipe::open(const std::string &cmd, char mode)
{
    if (opened())
        close(); // prevent resource leak
    constexpr auto READ = 0u;
    constexpr auto WRITE = 1u;
    int fd[2];
    if (::pipe(fd) == -1)
        return report(errno, "pipe");
    if ((pid = ::fork()) == -1)
        return report(errno, "fork");

    if (pid == 0) { // child process
        if (mode == 'r') {
			::close(fd[READ]);    // Close the READ end of the pipe
            ::dup2(fd[WRITE], 1); // Redirect stdout to pipe
        } else {                // (mode == 'w')
			::close(fd[WRITE]);   // Close the WRITE end of the pipe
            dup2(fd[READ], 0);  // Redirect stdin to pipe
        }
        ::setpgid(pid, pid); // Needed so negative PIDs can kill children of /bin/sh
        ::execl("/bin/sh", "/bin/sh", "-c", cmd.c_str(), nullptr);
        // execl returns only upon error
        std::exit(EXIT_FAILURE);
    } else {
        if (mode == 'r') {
			::close(fd[WRITE]); // Close the WRITE end of the pipe since parent's
                              // fd is read-only
        } else {              // (mode == 'w')
			::close(fd[READ]); // Close the READ end of the pipe since parent's fd
                             // is write-only
        }
    }
    if (mode == 'r')
        file_ = ::fdopen(fd[READ], "r");
    else
        file_ = ::fdopen(fd[WRITE], "w");
    if (file_ == nullptr)
        return report(errno, "fdopen");
    return 0;
}

/// Closes the pipe opened by popen2 and waits for termination
int common_pipe::close(int *exit_code)
{
    if (!opened())
        return 0; // nothing to do
    ::fclose(file_);
    file_ = nullptr;
    while (::waitpid(pid, exit_code, 0) == -1) {
        if (errno != EINTR) {
            if (exit_code != nullptr)
                *exit_code = -1;
            break;
        }
    }
    return 0;
}

#endif // POSIX implementation

inline int common_pipe::report(int code, const std::string& what)
{
#if defined(__STDC_LIB_EXT1__) || defined(_MSC_VER)
    char buffer[128]{}; // MSVC has no strerrorlen_s
    strerror_s(buffer, 128, code); // MSVC rejects strerror
    error_ = what + ": " + buffer;
#else
    error_ = what + ": " + std::strerror(code); // GCC fixed strerror and has no strerror_s
#endif // __STDC_LIB_EXT1__ or MSVC
    if (exceptions_)
        throw std::system_error{code, std::generic_category(), what};
    return code;
}

ipipe::ipipe() = default; // starting with C++20 could be in-class

opipe::opipe() = default; // starting with C++20 could be in-class

int opipe::write(std::string_view data)
{
    constexpr auto CSIZE = sizeof(std::string_view::value_type);
    if (!opened())
        return report(EINVAL, "opipe::write");
    if (auto sz = std::fwrite(data.data(), CSIZE, data.length(), file_);
        sz != data.size()) {
        if (auto err = std::ferror(file_); err != 0)
            return report(EIO, "fwrite error");
        if (auto err = std::feof(file_); err != 0)
            return report(EIO, "fwrite eof");
    }
    return 0;
}

int opipe::flush(std::string_view data)
{
    if (!opened())
        return report(EINVAL, "opipe::flush");
    if (!data.empty())
        if (auto err = write(data); err != 0)
            return report(err, "opipe::write");
    if (auto res = std::fflush(file_); res != 0)
        return report(errno, "fflush");
    return 0;
}

int ipipe::read(std::string &data)
{
    if (!opened())
        return report(EINVAL, "ipipe::read");
    data.clear();
    auto buffer = std::array<char, 128>{};
    while (!std::feof(file_) && !std::ferror(file_)) {
        auto count =
            std::fread(buffer.data(), sizeof(char), buffer.size(), file_);
        if (count > 0)
            data.append(buffer.data(), count);
    }
    if (std::ferror(file_))
        return report(EIO, "fread");
    return 0;
}

int shell_write(const std::string &cmd, std::string &data)
{
    auto pipe = opipe{};
    if (auto err = pipe.open(cmd); err != 0)
        return err;
    return pipe.write(data);
}

int shell_read(const std::string &cmd, std::string &data)
{
    auto pipe = ipipe{};
    if (auto err = pipe.open(cmd); err != 0)
        return err;
    return pipe.read(data);
}
