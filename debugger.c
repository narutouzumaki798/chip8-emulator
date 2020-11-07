char register_buffer[100][100];
char memory_buffer[100][100];
int key_flags[16];
WINDOW* register_screen;
WINDOW* memory_screen;
WINDOW* keys_screen;
void* create_shared_memory(size_t size) {
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, -1, 0);
}
void debugger_addstr(char ptr[][100], int x, int y, char** str)
{
    int i = 0;
    while((*str)[i] != '\0')
    {
	ptr[y][x] = (*str)[i];
	i++; x++;
    }
    free(*str); (*str) = NULL;
}
void debugger_reset()
{
    for(int i=0; i<100; i++)
    {
	for(int j=0; j<100; j++)
	{
	    register_buffer[i][j] = ' ';
	    memory_buffer[i][j] = ' ';
	}
    }
}
void debugger_display()
{
    // register
    for(int i=0; i<12; i++)
	for(int j=0; j<57; j++)
	    mvwaddch(register_screen, i+1, j+2, register_buffer[i][j]);
    // memory
    for(int i=0; i<12; i++)
	for(int j=0; j<57; j++)
	    mvwaddch(memory_screen, i+1, j+2, memory_buffer[i][j]);

    // keys
    mvwaddstr(keys_screen, 1, 1, "Keys:");
    int x = 2, y = 3, z;

    // row 1
    z = 0x1;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x2;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x3;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0xC;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x=2; y++;


    // row 2
    z = 0x4;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x5;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x6;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0xD;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x=2; y++;

    // row 3
    z = 0x7;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x8;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x9;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0xE;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x=2; y++;

    // row 4
    z = 0xA;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0x0;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0xB;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x+=5;

    z = 0xF;
    if(key_flags[z]) wattron(keys_screen, A_REVERSE);
    mvwaddstr(keys_screen, y, x, to_hex1(z));
    if(key_flags[z]) wattroff(keys_screen, A_REVERSE);
    x=2; y++;




    wrefresh(register_screen);
    wrefresh(memory_screen);
    wrefresh(keys_screen);
}

void debugger_update()
{
    debugger_reset();
    int x, y;
    char* tmp = NULL;

    // registers
    x = 0; y = 0;
    append1(&tmp, "Registers:");
    debugger_addstr(register_buffer, x, y, &tmp); y+=2;
    for(int i=0; i<8; i++)
    {
	char r = (i < 10) ? ('0'+i) : ('A' + (i-10));
	append1(&tmp, "V");
	append2(&tmp, r);
	append1(&tmp, ": ");
	append1(&tmp, to_hex1(V[i]));
	// printf("tmp: %s\n", tmp);
	debugger_addstr(register_buffer, x, y, &tmp);
	y++;
    }
    x += 12; y = 2;
    for(int i=8; i<16; i++)
    {
	char r = (i < 10) ? ('0'+i) : ('A' + (i-10));
	append1(&tmp, "V");
	append2(&tmp, r);
	append1(&tmp, ": ");
	append1(&tmp, to_hex1(V[i]));
	debugger_addstr(register_buffer, x, y, &tmp);
	y++;
    }
    x += 12; y = 2;
    append1(&tmp, "PC : ");
    append1(&tmp, to_hex2((*PC)));
    debugger_addstr(register_buffer, x, y, &tmp);
    y++;
    append1(&tmp, "I  : ");
    append1(&tmp, to_hex2((*I)));
    debugger_addstr(register_buffer, x, y, &tmp);
    y++;

    // memory
    x = 0; y = 0;
    append1(&tmp, "Memory:");
    debugger_addstr(memory_buffer, x, y, &tmp);  y+=2;
    int mem_idx = (*PC) - 14;
    for(int j=0; j<3; j++)
    {
	for(int i=0; i<10; i++) // column 1
	{
	    if(mem_idx == (*PC))
		append1(&tmp, ">> ");
	    else
		append1(&tmp, "   ");

	    append1(&tmp, to_hex2(mem_idx));
	    append1(&tmp, ": ");
	    append1(&tmp, to_hex1(mem[mem_idx]));
	    debugger_addstr(memory_buffer, x, y, &tmp);
	    y++; mem_idx++;
	}
	x += 17; y = 2;
    }

    // keys
    for(int i=0; i<16; i++)
	key_flags[i] = keys[i];
}
void curses_init()
{
  initscr();
  noecho();
  curs_set(0);
  erase();
  refresh();
}
void curses_end()
{
    endwin();
}

void debugger()
{
    curses_init();
    register_screen = newwin(15, 60, 1, 5);
    memory_screen = newwin(15, 60, 16, 5);
    keys_screen = newwin(21, 24, 1, 65);
    box(register_screen, 0, 0);
    box(memory_screen, 0, 0);
    box(keys_screen, 0, 0);
    wrefresh(register_screen);
    wrefresh(memory_screen);
    wrefresh(keys_screen);

    debugger_reset();
    while(1)
    {
	sem_wait(semaphore1);
	if(debug) fprintf(err_fp, "debugger: PC = %x pid = %d\n",
		(*PC), pid); fflush(err_fp);
	if((*PC) == 0) // exit if PC == 0
	{
	    sem_post(semaphore1);
	    curses_end();
	    exit(1);
	}
	debugger_update();
	sem_post(semaphore1);
	debugger_display();
	usleep(16000); // around 60 Hz
    }
    curses_end();
    exit(1);
}


