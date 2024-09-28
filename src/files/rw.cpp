#include "rw.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream> // Add this line´

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif

#define BUFFER_SIZE 1024

// Function to open the file
intptr_t open_file(const char* filename) {

#ifdef _WIN32
    HANDLE hFile = CreateFileA(
        filename,    // file name with .s extension
        GENERIC_READ,            // open for reading
        0,                       // do not share
        NULL,                    // default security
        OPEN_EXISTING,           // open only if the file exists
        FILE_ATTRIBUTE_NORMAL,   // normal file
        NULL);                   // no attr. template
    
    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error_code = GetLastError();
        return -1;
    }
    return (intptr_t)hFile;
#else
    int fd = open(filename, O_RDONLY); // Open file read-only
    if (fd == -1) {
        return -1;
    }
    return fd;
#endif
}

// Function to close the file
void close_file(intptr_t fd)
{
#ifdef _WIN32
    CloseHandle((HANDLE)fd);
#else
    close(fd);
#endif
}

// Function to read the file line by line
std::vector<std::string> read_lines(const char *filename)
{
    std::vector<std::string> lines;
    char buffer[BUFFER_SIZE];
    std::string current_line;

    intptr_t fd = open_file(filename);
    if (fd == -1)
    {
        throw std::runtime_error("Error: Unable to open file " + std::string(filename));
    }

#ifdef _WIN32
    DWORD bytes_read;
    while (ReadFile((HANDLE)fd, buffer, BUFFER_SIZE, &bytes_read, NULL))
    {
        if (bytes_read == 0)
        {
            break;
        }
        current_line.append(buffer, bytes_read);
        size_t pos;
        while ((pos = current_line.find('\n')) != std::string::npos)
        {
            lines.push_back(current_line.substr(0, pos));
            current_line.erase(0, pos + 1);
        }
    }
    if (GetLastError() != ERROR_SUCCESS && GetLastError() != ERROR_HANDLE_EOF)
    {
        close_file(fd);
        throw std::runtime_error("Error: Failed to read from file " + std::string(filename));
    }
#else
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        current_line.append(buffer, bytes_read);
        size_t pos;
        while ((pos = current_line.find('\n')) != std::string::npos)
        {
            lines.push_back(current_line.substr(0, pos));
            current_line.erase(0, pos + 1);
        }
    }
    if (bytes_read == -1)
    {
        close_file(fd);
        throw std::runtime_error("Error: Failed to read from file " + std::string(filename));
    }
#endif

    close_file(fd);

    if (lines.empty())
    {
        throw std::runtime_error("Error: File " + std::string(filename) + " is empty or contains no valid lines");
    }

    return lines;
}