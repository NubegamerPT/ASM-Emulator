#ifdef WIN32 
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

    WINDOW *memory;
    WINDOW *registors;
    WINDOW *program;
    WINDOW *terminal;
    WINDOW *info;

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE); // Enable special keys like arrows
    curs_set(0);
    
    refresh();

    memory = create_newPanel(LINES - 3, (COLS / 7) * 1, 0, 0, "Memory panel");
    terminal = create_newPanel(LINES - 3, (COLS / 7) * 4, 0, (COLS / 7) * 1, "Terminal panel");
    registors = create_newPanel((LINES / 5) * 2, (COLS / 7) * 2, 0, (COLS / 7) * 5, "Registors panel");
    program = create_newPanel((LINES / 5) * 3 - 2, (COLS / 7) * 2, (LINES / 5) * 2, (COLS / 7) * 5, "Stack panel");
    info = create_newPanel(3, COLS, LINES - 3, 0, "Info panel");

    print(info, 1, 1, "Info: Press [F] to go step-by-step or [R] to run the program");

    core.load("addi a8 a8 2");
    core.load("addi b8 a8 4");
    core.load("subb c8 b8 2");
    core.load("halt");

    for (int i = 0; i < 162; i += 4)
    {
        print(memory, i/4 + 1, 1, core.dump(i));
    }

    for (int i = 0; i < 92; i += 4)
    {
        print(program, i/4 + 1, 1, core.getStack(i));
    }

    core.run();

    print(registors, 1, 1, "Registors:");
    print8(registors, 2, 2, "A8: ", core.getREG8(0));
    print8(registors, 3, 2, "B8: ", core.getREG8(1));
    print8(registors, 4, 2, "C8: ", core.getREG8(2));
    print8(registors, 5, 2, "D8: ", core.getREG8(3));
    print16(registors, 2, 18, "A16: ", core.getREG16(0));
    print16(registors, 3, 18, "B16: ", core.getREG16(1));
    printff(registors, 4, 18, "AF:  ", core.getREGF(0));
    printff(registors, 5, 18, "BF:  ", core.getREGF(1));

    getch();  // wait for user input
    endwin(); // close the window
    return 0;
}