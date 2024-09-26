#pragma once
#include <cstring>

#ifdef WIN32
#include <ncurses/ncurses.h>
#else
#include <curses.h>
#endif

#include <iostream>
#include <string>

#define SCREEN_WIDTH 92
#define SCREEN_HEIGHT 28