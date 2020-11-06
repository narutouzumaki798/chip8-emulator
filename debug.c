#include <ncurses.h>

// debug
WINDOW* debug_reg;
WINDOW* debug_mem;
void curses_init()
{
  initscr();
  noecho();
  curs_set(0);
  erase();
  refresh();

  debug_reg = newwin(10, 10, 2, 2);
  debug_mem = newwin(10, 10, 15, 2);
}
void curses_end()
{
  endwin();
}
void debug_update()
{
    box(debug_reg, 0, 0);
    box(debug_mem, 0, 0);
    wrefresh(debug_reg);
    wrefresh(debug_mem);
}

