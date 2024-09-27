#ifdef _WIN32
#include <ncurses/ncurses.h>
#include <ncursesw/curses.h>
#include <windows.h>
#else
#include <curses.h>
#include <ncursesw/curses.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>

#include "cpu.h"
#include "./panels/window.cpp"

// Define global windows
WINDOW *memory, *registers, *program, *terminal, *info;

void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000); // usleep takes microseconds
#endif
}

void init_colors()
{
    if (has_colors() == FALSE)
    {
        endwin();
        std::cerr << "Your terminal does not support color." << std::endl;
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Define color pair 1 as green text on a black background
}

void printREGISTORS(CPU core)
{
    print(registers, 1, 1, "Registers:");
    print8(registers, 2, 2, "A8: ", core.getREG8(0));
    print8(registers, 3, 2, "B8: ", core.getREG8(1));
    print8(registers, 4, 2, "C8: ", core.getREG8(2));
    print8(registers, 5, 2, "D8: ", core.getREG8(3));
    print8(registers, 6, 2, "PC: ", core.getPC());
    print16(registers, 2, 18, "A16: ", core.getREG16(0));
    print16(registers, 3, 18, "B16: ", core.getREG16(1));
    printff(registers, 4, 18, "AF:  ", core.getREGF(0));
    printff(registers, 5, 18, "BF:  ", core.getREGF(1));
    print8(registers, 6, 18, "SP:  ", core.getSP());
}

int main()
{
    CPU core;

    // Initialize ncurses
    initscr();

    resize_term(40, 200);

    noecho();
    cbreak();
    keypad(stdscr, TRUE); // Enable special keys like arrows
    curs_set(0);

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);    // Red text on black background
        init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Green text on black background
        init_pair(3, COLOR_YELLOW, COLOR_BLACK); // Yellow text on black background
        init_pair(4, COLOR_BLUE, COLOR_BLACK);   // Blue text on black background
        init_pair(5, COLOR_BLACK, COLOR_WHITE); // Black text on white background
    }

    refresh();

    /* memory = create_newPanel(LINES - 3, (COLS / 7) * 1, 0, 0, "Memory panel");
    terminal = create_newPanel(LINES - 3, (COLS / 7) * 3, 0, (COLS / 6) * 1, "Terminal panel");
    registers = create_newPanel((LINES / 5) * 2, (COLS / 8) * 2, 0, (COLS / 8) * 5, "Registors panel");
    program = create_newPanel((LINES / 5) * 3 - 2, (COLS / 8) * 2, (LINES / 5) * 2, (COLS / 8) * 5, "Stack panel");
    info = create_newPanel(3, COLS, LINES - 3, 0, "Info panel"); */

    memory = create_newPanel(LINES - 3, (COLS / 7) * 1, 0, 0, "Memory panel");
    terminal = create_newPanel(LINES - 3, (COLS / 7) * 4, 0, (COLS / 7) * 1, "Terminal panel");
    registers = create_newPanel((LINES / 5) * 1, ((COLS / 7) * 2) + 4, 0, (COLS / 7) * 5, "Registors panel");
    program = create_newPanel(((LINES / 5) * 4) - 3, ((COLS / 7) * 2) + 4, (LINES / 5) * 1, (COLS / 7) * 5, "Stack panel");
    info = create_newPanel(3, COLS, LINES - 3, 0, "Info panel");

    print(info, 1, 1, "Info: Press [F] to go step-by-step or [R] to run the program");

    core.load("load a8 1");
    core.load("push a8");
    core.load("load a8 2");
    core.load("push a8");
    core.load("load a8 5");
    core.load("pop a8");
    core.load("pop a8");
    core.load("halt");

    bool hasStoped = false;

    while (true)
    {
        printREGISTORS(core);
        displayMemory(memory, 138, core);
        displayStack(program, 106, core);
        int ch = getch(); // Get user input, non-blocking due to nodelay

        // Check for F1 key press
        if ((ch == 'f' || ch == 'F') && !hasStoped)
        {
            print(info, 1, 1, "                                                                                                    ");
            printColor(info, 2, 1, 1, "Info: Program running on step-by-step mode");
            // Execute some function, e.g., core.execute();
            int temp = core.execute(); // Your CPU's execute method

            if (temp == 1)
            {
                print(info, 1, 1, "                                                                                                    ");
                printColor(info, 3, 1, 1, "Info: Program has reached HALT!");
                hasStoped = true;
            }
        }

        // Check for 'q' or 'Q' key press to quit
        if (ch == 'q' || ch == 'Q')
        {
            break; // Exit the loop
        }

        if (ch == 'r' || ch == 'R')
        {
            print(info, 1, 1, "                                                                                                    ");
            printColor(info, 4, 1, 1, "Info: Program running on automatic mode");
            while (core.execute() != 1)
            {
                printREGISTORS(core);
                sleep_ms(200);
            }
            print(info, 1, 1, "                                                                                                    ");
            printColor(info, 5, 1, 1, "Info: Program has reached HALT!");
        }
    }
    endwin(); // close the window
    return 0;
}