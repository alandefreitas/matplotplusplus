#include <matplot/util/popen.h>

#ifdef _WIN32
#include <vector>
#include <memory>
#include <mutex>
#include <io.h>

namespace matplot::detail {
    /// Stores file and process handles between hiddenPopen and hiddenPclose
    struct WinProcess
    {
        FILE *file{nullptr};
        PROCESS_INFORMATION pi{}; // contains process handle to be closed in pclose
        static void store(std::unique_ptr<WinProcess> proc) {
            std::scoped_lock lock{mtx};
            procs.push_back(std::move(proc));
        }
        static std::unique_ptr<WinProcess> retrieve(FILE* file) {
            auto res = std::unique_ptr<WinProcess>{};
            auto lock = std::scoped_lock{mtx};
			for (auto it = procs.begin(); it != procs.end(); ++it) {
				if ((*it)->file == file) {
					res = std::move(*it);
					procs.erase(it);
					break;
				}
			}
            return res;
        }
    private:
        static std::vector<std::unique_ptr<WinProcess>> procs;
        static std::mutex mtx;
    };
    std::vector<std::unique_ptr<WinProcess>> WinProcess::procs{};
    std::mutex WinProcess::mtx{};

    // Function to create a new process and return a FILE pointer for
    // input/output. Mimics _popen behaviour, but does not open console windows
    // in GUI apps
    FILE *hiddenPopen(const char *command, const char *mode) {
        HANDLE hChildStdinRd, hChildStdinWr, hStdin, hStdout;
        SECURITY_ATTRIBUTES saAttr{};
        // Set up security attributes for inheritable handles
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        // Create a pipe for the child process's input
        if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
            errno = GetLastError(); // emulate POSIX behavior
            return nullptr;
        }

        // Ensure the write handle to the pipe is not inherited by child
        // processes
        if (!SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0)) {
            auto err = GetLastError(); // emulate POSIX behavior
            CloseHandle(hChildStdinRd);
            errno = err;
            return nullptr;
        }

        // Create a pipe for the child process's output
        if (!CreatePipe(&hStdin, &hStdout, &saAttr, 0)) {
            auto err = GetLastError(); // emulate POSIX behavior
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            errno = err;
            return nullptr;
        }

        // Ensure the read handle to the output pipe is not inherited by child
        // processes
        if (!SetHandleInformation(hStdout, HANDLE_FLAG_INHERIT, 0)) {
            auto err = GetLastError(); // emulate POSIX behavior
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            errno = err;
            return nullptr;
        }

        // Configure STARTUPINFO structure for the new process
        STARTUPINFO si{};
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = hStdout;
        si.hStdOutput = hStdout;
        si.hStdInput = hChildStdinRd;
        si.dwFlags |= STARTF_USESTDHANDLES;
        auto p = std::make_unique<WinProcess>();

        // Create the child process, while hiding the window
        if (!CreateProcess(NULL, const_cast<char *>(command), NULL, NULL, TRUE,
                           CREATE_NO_WINDOW, NULL, NULL, &si, &p->pi)) {
            auto err = GetLastError(); // emulate POSIX behavior
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            CloseHandle(hStdout);
            errno = err;
            return nullptr;
        }

        // Close unnecessary handles
        CloseHandle(hChildStdinRd);
        CloseHandle(hStdout);

        // Create a FILE pointer from the write handle to the child process's
        // input
        FILE *file = _fdopen(_open_osfhandle((intptr_t)hChildStdinWr, 0), mode);

        if (file == nullptr) {
            auto err = GetLastError();
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            CloseHandle(p->pi.hProcess);
            CloseHandle(p->pi.hThread);
            errno = err; // emulate POSIX behavior
            return nullptr;
        }
        p->file = file;
        WinProcess::store(std::move(p));
        return file;
    }

    // Function to close the process and retrieve its exit code
    int hiddenPclose(FILE *file) {
        // The following does not work for GetExitCodeProcess:
        // HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
        if (file == nullptr) {
            errno = ECHILD;
            return -1;
        }
        // Close the input to child:
        fclose(file);
        // Retrieve the child process info:
        auto p = WinProcess::retrieve(file);
        if (p == nullptr) {
            errno = ECHILD;
            return -1;
        }
        HANDLE hThread = p->pi.hThread;
        HANDLE hProc = p->pi.hProcess;
        // Wait for the process to finish
        if (auto r = WaitForSingleObject(hProc, INFINITE); r != WAIT_OBJECT_0) {
            CloseHandle(hThread);
            CloseHandle(hProc);
            errno = ECHILD; // emulate POSIX behavior
            return -1;
        }
        // Retrieve the exit code
        DWORD exitCode;
        if (BOOL r = GetExitCodeProcess(hProc, &exitCode); !r) {
            auto err = GetLastError();
            CloseHandle(hThread);
            CloseHandle(hProc);
            errno = err; // emulate POSIX behavior
            return -1;
        }
        CloseHandle(hThread);
        CloseHandle(hProc);
        return exitCode;
    }
} // namespace matplot::detail
#endif // _WIN32
