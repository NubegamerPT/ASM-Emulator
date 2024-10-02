#ifndef RW_H
#define RW_H

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream> // Add this line´

#ifdef _WIN32
#include <windows.h>
#else
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

const size_t BUFFER_SIZE = 4096; // Buffer size for reading data

intptr_t open_file(const char* filename);

// Function to close the file
void close_file(intptr_t fd);

// Function to read the file line by line
std::vector<std::string> read_lines(const char* filename);

#endif // RW_H