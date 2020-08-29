//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_POPEN_H
#define MATPLOTPLUSPLUS_POPEN_H

#ifdef _WIN32
#include <windows.h>
#define PCLOSE _pclose
#define POPEN _popen
#define FILENO _fileno
#else
#define PCLOSE pclose
#define POPEN popen
#define FILENO fileno
#endif

#endif // MATPLOTPLUSPLUS_POPEN_H
