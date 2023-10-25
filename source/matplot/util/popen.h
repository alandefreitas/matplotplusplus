//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_POPEN_H
#define MATPLOTPLUSPLUS_POPEN_H

#ifdef _WIN32
#include <stdio.h> //needed for FILE type
#include <windows.h>

namespace matplot::detail {
    // in a GUI application, _popen pops up a window. This version does not.
    FILE *hiddenPopen(const char *command, const char *mode);

    // close the handle opened by hiddenPopen
    int hiddenPclose(FILE *file);
} // namespace matplot::detail

#define PCLOSE ::matplot::detail::hiddenPclose
#define POPEN ::matplot::detail::hiddenPopen
#define FILENO _fileno
#else
#define PCLOSE pclose
#define POPEN popen
#define FILENO fileno
#endif

#endif // MATPLOTPLUSPLUS_POPEN_H
