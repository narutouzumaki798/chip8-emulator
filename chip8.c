
// i will use long data type for 16 bit registers/locations
// and int for 8 bit registers/locations just for convinience

// original COSMAC VIP base system had 2K ram
int mem[2048];
// each element of the array will store 1 byte

long PC; // program counter
long I; // index register

int N; // number of bytes in the rom

int V[16]; // register file

int emu_timer; // sound/delay timer

void load_rom(char* rom_file)
{
    FILE* fp = fopen(rom_file, "rb");
    unsigned char buffer[2000];
    int i = 0x200;
    while(fread(mem+i, 1, 1, fp)) // read rom directly into memory
	i++;                      // starting from address 0x200 as per orignal COSMAC VIP convention
                                  // in order to play old games
    N = i;
}   

void load_font()
{
    
}


// init
void emu_start()
{
    for(int i=0; i<SCREEN_HEIGHT; i++)
    {
	for(int j=0; j<SCREEN_WIDTH; j++)
	{
	    screen_buffer[i][j].red = 0; 
	    screen_buffer[i][j].blue = 0; 
	    screen_buffer[i][j].green = 0; 
	}
    }
    load_rom("space_invaders.ch8");
    load_font();
}

// update
void emu_update()
{
    /* for(int i=0; i<SCREEN_HEIGHT; i++) */
    /* { */
    /* 	for(int j=0; j<SCREEN_WIDTH; j++) */
    /* 	{ */
    /* 	    int x = rand()%256; */
    /* 	    screen_buffer[i][j].red = x;  */
    /* 	    screen_buffer[i][j].blue = x;  */
    /* 	    screen_buffer[i][j].green = x;  */
    /* 	} */
    /* } */

    // fetch

    // decode & execute
    
}


