//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_POPEN_H
#define MATPLOTPLUSPLUS_POPEN_H

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#include <windows.h>

// in a GUI application, _popen pops up a window. This version does not.
FILE *hiddenPopen(const char *command, const char *mode);

// close the handle opened by hiddenPopen
int hiddenPclose(FILE *file);

#define PCLOSE hiddenPclose
#define POPEN hiddenPopen
#define FILENO _fileno
#else
#define PCLOSE pclose
#define POPEN popen
#define FILENO fileno
#endif

#endif // MATPLOTPLUSPLUS_POPEN_H
