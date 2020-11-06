char screen[100][100];
void debugger_addstr(int x, int y, char** str)
{
    int i = 0;
    while((*str)[i] != '\0')
    {
       screen[y][x] = (*str)[i];
       i++; x++;
    }
    free(*str); (*str) = NULL;
}
void debugger_reset()
{
    for(int i=0; i<100; i++)
	for(int j=0; j<100; j++)
	    screen[i][j] = ' ';
}
void debugger_display()
{
    printf("\n\n");
    for(int i=0; i<22; i++)
    {
	for(int j=0; j<50; j++)
	{
	    printf("%c", screen[i][j]);
	}
	printf("\n");
    }
    int raise = 20;
    while(raise--)
    printf("\n");
}

void debugger_update()
{
    debugger_reset();
    int x, y;
    char* tmp = NULL;

    // registers
    x = 3; y = 1;
    append1(&tmp, "Registers:");
    debugger_addstr(x, y, &tmp); y++; int register_y = y;
    for(int i=0; i<8; i++)
    {
	char r = (i < 10) ? ('0'+i) : ('A' + (i-10));
	append1(&tmp, "V");
	append2(&tmp, r);
	append1(&tmp, " : ");
	append1(&tmp, to_hex1(V[i]));
	// printf("tmp: %s\n", tmp);
	debugger_addstr(x, y, &tmp);
	y++;
    }
    x += 12; y = register_y;
    for(int i=8; i<16; i++)
    {
	char r = (i < 10) ? ('0'+i) : ('A' + (i-10));
	append1(&tmp, "V");
	append2(&tmp, r);
	append1(&tmp, " : ");
	append1(&tmp, to_hex1(V[i]));
	debugger_addstr(x, y, &tmp);
	y++;
    }
    x += 12; y = register_y;
    append1(&tmp, "PC : ");
    append1(&tmp, to_hex2(PC));
    debugger_addstr(x, y, &tmp);
    y++;
    append1(&tmp, "I  : ");
    append1(&tmp, to_hex2(I));
    debugger_addstr(x, y, &tmp);
    y++;

    // memory
    x = 3; y = 11;
    append1(&tmp, "Memory:");
    debugger_addstr(x, y, &tmp);  y++; int memory_y = y;
    int mem_idx = PC - 14;
    for(int i = 0; i<10; i++) // column 1
    {
	append1(&tmp, to_hex2(mem_idx));
	append1(&tmp, ": ");
	append1(&tmp, to_hex1(mem[mem_idx]));
	debugger_addstr(x, y, &tmp);
	y++; mem_idx++;
    }
    x += 14; y = memory_y;
    for(int i = 0; i<10; i++) // column 2
    {
	if(mem_idx == PC)
	    append1(&tmp, ">> ");
	else
	    append1(&tmp, "   ");

	append1(&tmp, to_hex2(mem_idx));
	append1(&tmp, ": ");
	append1(&tmp, to_hex1(mem[mem_idx]));
	debugger_addstr(x, y, &tmp);
	y++; mem_idx++;
    }
    x += 17; y = memory_y;
    for(int i = 0; i<10; i++) // column 3
    {
	append1(&tmp, "   ");
	append1(&tmp, to_hex2(mem_idx));
	append1(&tmp, ": ");
	append1(&tmp, to_hex1(mem[mem_idx]));
	debugger_addstr(x, y, &tmp);
	y++; mem_idx++;
    }


    debugger_display();
}


