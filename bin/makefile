# Compiler settings
CC = g++  # Default to g++
CFLAGS = -std=c++17
LDFLAGS = -lncurses

# Output executable name
OUT = ASM

# Source files
SRC = ../src/main.cpp

# OS detection
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXE = .exe
    CFLAGS += -I/C:/msys64/mingw64/include
    LDFLAGS += -L/C:/msys64/mingw64/lib -lncursesw
else
    RM = rm -f
    EXE =
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -I/usr/local/include
        LDFLAGS += -L/usr/local/lib -lncurses
        CC = clang++
    endif
endif

# All target
all: $(OUT)

# Build target
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT)$(EXE) $(CFLAGS) $(LDFLAGS)

# Clean up
clean:
	$(RM) $(OUT)$(EXE)