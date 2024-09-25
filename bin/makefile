# Makefile

# Compiler settings
CC = g++  # Default to g++
CFLAGS = -I./lib/ncurses/include -std=c++17
LDFLAGS = -lncurses

# Output executable name
OUT = ASM

# Source files
SRC = ../src/main.cpp

# All target
all: $(OUT)

# Windows build target
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT).exe $(LDFLAGS) $(CFLAGS)

mac: $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LDFLAGS) $(CFLAGS)

# Clean up
clean:
	rm -f *.o $(OUT)
