# Makefile

# Compiler settings
CC = gcc  # Default to g++
CFLAGS = -I/C:/msys64/mingw64/include -L/C:/msys64/mingw64/lib -std=c++17
LDFLAGS = -lncurses

# Output executable name
OUT = ASM

# Source files
SRC = ../src/main.cpp

# All target
all: $(OUT)

# Windows build target
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT).exe $(CFLAGS) $(LDFLAGS)

# Mac build target
mac: $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Clean up
clean:
	del /Q *.o $(OUT).exe