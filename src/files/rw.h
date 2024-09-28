#ifndef RW_H
#define RW_H

#include <iostream>
#include <vector>
#include <cstring> // For strerror
#include <errno.h> // For error handling

#ifdef _WIN32
#include <windows.h> // Windows API
#else
#include <fcntl.h>   // For open()
#include <unistd.h>  // For read() and close()
#endif

const size_t BUFFER_SIZE = 4096; // Buffer size for reading data

intptr_t open_file(const char* filename);

// Function to close the file
void close_file(intptr_t fd);

// Function to read the file line by line
std::vector<std::string> read_lines(const char* filename);

#endif // RW_H