# Compiler settings
CC = gcc  # Default to g++ or clang
CFLAGS = -std=c++17
LDFLAGS = -lncurses

# Output executable name
OUT = ASM

# Source files
SRC = ../src/main.cpp

# OS detection (Windows or Mac)
UNAME_S := $(shell uname -s)

# All target
all: $(OUT)

# Windows build target
ifeq ($(UNAME_S), Windows_NT)
CFLAGS += -I/C:/msys64/mingw64/include -L/C:/msys64/mingw64/lib
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT).exe $(CFLAGS) $(LDFLAGS)
else
# Mac build target
ifeq ($(UNAME_S), Darwin)
CFLAGS += -I/usr/local/include -L/usr/local/lib  # Adjust for macOS if needed
$(OUT): $(SRC)
	clang++ $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
endif
endif

# Clean up
clean:
ifeq ($(UNAME_S), Windows_NT)
	del /Q $(OUT).exe
else
	rm -f $(OUT)
endif
