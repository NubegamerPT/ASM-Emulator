#ifndef WIN32 
#include <ncurses/ncurses.h>
#else
#include <curses.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "./panels/window.cpp"
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

int main()
{
    CPU core;

    WINDOW *terminal;
    WINDOW *registers;
    WINDOW *program;

    initscr();     // initialize the screen
    init_colors(); // Initialize colors

    noecho(); // Don't echo user input

    keypad(stdscr, TRUE); // Enable special keys like arrows

    refresh();
    curs_set(0);

    terminal = create_newPanel(LINES, (COLS / 5) * 3, 0, 0, "Terminal panel");
    registers = create_newPanel((LINES / 5) * 2, (COLS / 5) * 2, 0, (COLS / 5) * 3, "Registers panel");
    program = create_newPanel((LINES / 5) * 3, (COLS / 5) * 2, (LINES / 5) * 2, (COLS / 5) * 3, "Program panel");

    core.load("addi b8 c8 5");
    core.load("subb b8 d8 5");
    core.load("load a8 9");

    for (int i = 0; i < 48; i += 4)
    {
        print(terminal, i/4 + 1, 1, core.dump(i));
    }

    int temp = core.execute();

    print(terminal, 13, 1, "Executed: ", temp);

    print(registers, 1, 1, "Registers:");
    print(registers, 2, 2, "A8: ", core.getREG8(0));
    print(registers, 3, 2, "B8: ", core.getREG8(1));
    print(registers, 4, 2, "C8: ", core.getREG8(2));
    print(registers, 5, 2, "D8: ", core.getREG8(3));

    getch();  // wait for user input
    endwin(); // close the window
    return 0;
}