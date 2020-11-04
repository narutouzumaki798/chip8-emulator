
// i will use long 16 bit registers/locations
// and int for 8 bit registers/locations just for convinience

// original COSMAC VIP base system had 2K ram
int mem[2048];
// each element of the array will store 1 byte
int idx; // memory idx

long PC; // program counter
long I; // index register
long SP; // stack pointer
long SB; // stack base

int N; // number of bytes in the rom

int V[16]; // register file

int delay_timer; // delay timer
int sound_timer; // sound timer

void load_rom(char* rom_file)
{
    FILE* fp = fopen(rom_file, "rb");
    unsigned char buffer[2000];
    int i = 0;
    while(fread(buffer+i, 1, 1, fp)) // read rom into buffer
	i++;
    N = i;

    // load in memory starting from address 0x200 as per orignal COSMAC VIP convention
    // in order to play old games
    // FILE* err_fp = fopen("debug.txt", "w");
    idx = 0x200;
    for(i=0; i<N; i++)
    {
	mem[idx++] = (int)buffer[i];
	// fprintf(err_fp, "%x: %x\n", idx, mem[idx-1]);
    }
    // fclose(err_fp);
}   

void load_font()
{
    
}


FILE* err_fp;
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
    PC = 0x200;
    SB = 0x07FF;
    SP = SB;
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

    if(PC >= idx)
    {
	printf("\n\n  hoe geche\n\n");
	exit(1);
    }

    // fetch
    int inst;
    inst = mem[PC]; PC++; inst = inst << 8;
    inst = inst | mem[PC]; PC++;
    int X, Y, NN, NNN;

    // decode & execute
    switch((int)inst>>12)
    {
    case 0x0: // ------------------------
	switch(inst)
	{
	case 0x00E0: // clear screen
	    for(int i=0; i<SCREEN_HEIGHT; i++)
	    {
		for(int j=0; j<SCREEN_WIDTH; j++)
		{
		    screen_buffer[i][j].red = 0;
		    screen_buffer[i][j].blue = 0;
		    screen_buffer[i][j].green = 0;
		}
	    }
	    break;
	case 0x00EE: // return from a subroutine
	    if(SP == SB) { printf("stack already empty\n"); exit(1); }
	    int ret = stack[++SP];
	    PC = ret;
	    break;
	default:
	    printf("execute machine language instruction\n");
	    break;
	}
	break;
    case 0x1: // ------------------------ // jump to address
	PC = inst&0x0FFF;
	break;
    case 0x2: // ------------------------ // call subroutine
	stack[SP--] = PC;
	PC = inst&0x0FFF;
	break;
    case 0x3: // ------------------------ // skip if VX == NN
	X = (inst&0x0F00)>>8;
	NN = (inst&0x00FF);
	if(V[X] == NN) PC += 2;
	break;
    case 0x4: // ------------------------ // skip if VX != NN
	X = (inst&0x0F00)>>8;
	NN = (inst&0x00FF);
	if(V[X] != NN) PC += 2;
	break;
    case 0x5: // ------------------------
	switch(inst0x000F)
	{
	case 0x0000: // skip if VX == VY
	    X = (inst&0x0F00)>>8;
	    Y = (inst&0x00F0)>>4;
	    if(V[X] == V[Y]) PC += 2;
	    break;
	default:
	    printf("invalid instruction: %x\n", inst);
	    break;
	}
	break;
    case 0x6: // ------------------------ VX = NN
	X = (inst&0x0F00)>>8;
	NN = (inst&0x00FF);
	V[X] = NN;
	break;
    case 0x7: // ------------------------ VX = VX + NN
	X = (inst&0x0F00)>>8;
	NN = (inst&0x00FF);
	V[X] += NN;
	break;
    case 0x8: // ------------------------
	switch(inst&0x000F)
	{
	case 0x0000: // VX = VY
	    X = (inst&0x0F00)>>8;
	    Y = (inst&0x00F0)>>4;
	    V[X] = V[Y];
	    break;
	case 0x0001:
	    printf(">> VX = VX | VY\n");
	    break;
	case 0x0002:
	    printf(">> VX = VX & VY\n");
	    break;
	case 0x0003:
	    printf(">> VX = VX ^ VY\n");
	    break;
	case 0x0004:
	    printf(">> VX = VX + VY; carry -> VF\n");
	    break;
	case 0x0005:
	    printf(">> VX = VX - VY; borrow -> VF\n");
	    break;
	case 0x0006:
	    printf(">> VX = VY >> 1; lsb -> VF\n");
	    break;
	case 0x0007:
	    printf(">> VX = VY - VX; borrow -> VF\n");
	    break;
	case 0x000E:
	    printf(">> VX = VY << 1; msb -> VF\n");
	    break;
	default:
	    printf("invalid instruction\n");
	    break;
	}
	break;
    case 0x9: // ------------------------
	switch(inst&0x000F)
	{
	case 0x0000: // skip if VX != VY
	    X = (inst&0x0F00)>>8;
	    Y = (inst&0x00F0)>>4;
	    if(V[X] != V[Y]) PC += 2;
	    break;
	default:
	    printf("invalid instruction\n");
	    break;
	}
	break;
    case 0xA: // ------------------------
	printf(">> I = NNN\n");
	break;
    case 0xB: // ------------------------
	printf(">> jump to NNN + V0\n");
	break;
    case 0xC: // ------------------------
	printf(">> VX to random number with a mask of NN\n");
	break;
    case 0xD: // ------------------------
	printf(">> draw sprite\n");
	break;
    case 0xE: // ------------------------
	switch(0x00FF)
	{
	case 0x009E:
	    printf(">> skip if key pressed\n");
	    break;
	case 0x00A1:
	    printf(">> skip if key not pressed\n");
	    break;
	default:
	    printf("invalid instruction\n");
	    break;
	}
	break;
    case 0xF: // ------------------------
	switch(inst&0x00FF)
	{
	case 0x0007:
	    printf(">> VX = delay timer\n");
	    break;
	case 0x000A:
	    printf(">> VX = input\n");
	    break;
	case 0x0015:
	    printf(">> delay timer = VX\n");
	    break;
	case 0x0018:
	    printf(">> sound timer = VX\n");
	    break;
	case 0x001E:
	    printf(">> I = I + VX\n");
	    break;
	case 0x0029:
	    printf(">> I = font sprite data address\n");
	    break;
	case 0x0033:
	    printf(">> binary coded decimal diye kichu ekta\n");
	    break;
	case 0x0055:
	    printf(">> store values of registers in memory\n");
	    break;
	case 0x0065:
	    printf(">> get values of registers from memory\n");
	    break;
	}
	break;
    default: // ------------------------
	printf("invalid instruction\n");
	break;
    }
}


