#include <matplot/util/popen.h>

#ifdef _WIN32
#include <io.h>

namespace matplot::detail {
    // Function to create a new process and return a FILE pointer for
    // input/output. Mimics _popen behaviour, but does not open console windows
    // in GUI apps
    FILE *hiddenPopen(const char *command, const char *mode) {
        SECURITY_ATTRIBUTES saAttr;
        HANDLE hChildStdinRd, hChildStdinWr, hStdin, hStdout;
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // Set up security attributes for inheritable handles
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        // Create a pipe for the child process's input
        if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
            return nullptr;
        }

        // Ensure the write handle to the pipe is not inherited by child
        // processes
        if (!SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hChildStdinRd);
            return nullptr;
        }

        // Create a pipe for the child process's output
        if (!CreatePipe(&hStdin, &hStdout, &saAttr, 0)) {
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            return nullptr;
        }

        // Ensure the read handle to the output pipe is not inherited by child
        // processes
        if (!SetHandleInformation(hStdout, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            return nullptr;
        }

        // Configure STARTUPINFO structure for the new process
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = hStdout;
        si.hStdOutput = hStdout;
        si.hStdInput = hChildStdinRd;
        si.dwFlags |= STARTF_USESTDHANDLES;

        // Create the child process, while hiding the window
        if (!CreateProcess(NULL, const_cast<char *>(command), NULL, NULL, TRUE,
                           CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(hChildStdinRd);
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            CloseHandle(hStdout);
            return nullptr;
        }

        // Close unnecessary handles
        CloseHandle(hChildStdinRd);
        CloseHandle(hStdout);

        // Create a FILE pointer from the write handle to the child process's
        // input
        FILE *file = _fdopen(_open_osfhandle((intptr_t)hChildStdinWr, 0), mode);

        if (file == nullptr) {
            CloseHandle(hChildStdinWr);
            CloseHandle(hStdin);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return nullptr;
        }

        return file;
    }

    // Function to close the process and retrieve its exit code
    int hiddenPclose(FILE *file) {
        HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
        fclose(file);
        DWORD exitCode;

        // Wait for the process to finish and get its exit code
        if (WaitForSingleObject(hFile, INFINITE) == WAIT_OBJECT_0 &&
            GetExitCodeProcess(hFile, &exitCode)) {
            CloseHandle(hFile);
            return exitCode;
        } else {
            return -1; // Failed to get the exit code
        }
    }
} // namespace matplot::detail
#endif // _WIN32
