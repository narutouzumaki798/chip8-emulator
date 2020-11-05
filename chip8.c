
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

int V[16]; // register file

int delay_timer; // delay timer
int sound_timer; // sound timer

#define debug 1
#define SUPER 0 // whether emulating super-chip

void load_rom(char* rom_file)
{
    FILE* fp = fopen(rom_file, "rb");
    unsigned char buffer[2000];
    int i = 0;
    while(fread(buffer+i, 1, 1, fp)) // read rom into buffer
	i++;
    int N = i; // number of bytes in rom

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
    int i = 0x100;
    mem[i] = 0xF0;   mem[i+1] = 0x90;   mem[i+2] = 0x90;   mem[i+3] = 0x90;   mem[i+4] = 0xF0; i += 5; // 0
    mem[i] = 0x20;   mem[i+1] = 0x60;   mem[i+2] = 0x20;   mem[i+3] = 0x20;   mem[i+4] = 0x70; i += 5; // 1
    mem[i] = 0xF0;   mem[i+1] = 0x10;   mem[i+2] = 0xF0;   mem[i+3] = 0x80;   mem[i+4] = 0xF0; i += 5; // 2
    mem[i] = 0xF0;   mem[i+1] = 0x10;   mem[i+2] = 0xF0;   mem[i+3] = 0x10;   mem[i+4] = 0xF0; i += 5; // 3
    mem[i] = 0x90;   mem[i+1] = 0x90;   mem[i+2] = 0xF0;   mem[i+3] = 0x10;   mem[i+4] = 0x10; i += 5; // 4
    mem[i] = 0xF0;   mem[i+1] = 0x80;   mem[i+2] = 0xF0;   mem[i+3] = 0x10;   mem[i+4] = 0xF0; i += 5; // 5
    mem[i] = 0xF0;   mem[i+1] = 0x80;   mem[i+2] = 0xF0;   mem[i+3] = 0x90;   mem[i+4] = 0xF0; i += 5; // 6
    mem[i] = 0xF0;   mem[i+1] = 0x10;   mem[i+2] = 0x20;   mem[i+3] = 0x40;   mem[i+4] = 0x40; i += 5; // 7
    mem[i] = 0xF0;   mem[i+1] = 0x90;   mem[i+2] = 0xF0;   mem[i+3] = 0x90;   mem[i+4] = 0xF0; i += 5; // 8
    mem[i] = 0xF0;   mem[i+1] = 0x90;   mem[i+2] = 0xF0;   mem[i+3] = 0x10;   mem[i+4] = 0xF0; i += 5; // 9
    mem[i] = 0xF0;   mem[i+1] = 0x90;   mem[i+2] = 0xF0;   mem[i+3] = 0x90;   mem[i+4] = 0x90; i += 5; // A
    mem[i] = 0xE0;   mem[i+1] = 0x90;   mem[i+2] = 0xE0;   mem[i+3] = 0x90;   mem[i+4] = 0xE0; i += 5; // B
    mem[i] = 0xF0;   mem[i+1] = 0x80;   mem[i+2] = 0x80;   mem[i+3] = 0x80;   mem[i+4] = 0xF0; i += 5; // C
    mem[i] = 0xE0;   mem[i+1] = 0x90;   mem[i+2] = 0x90;   mem[i+3] = 0x90;   mem[i+4] = 0xE0; i += 5; // D
    mem[i] = 0xF0;   mem[i+1] = 0x80;   mem[i+2] = 0xF0;   mem[i+3] = 0x80;   mem[i+4] = 0xF0; i += 5; // E
    mem[i] = 0xF0;   mem[i+1] = 0x80;   mem[i+2] = 0xF0;   mem[i+3] = 0x80;   mem[i+4] = 0x80; i += 5; // F
}

void draw_sprite(int X, int Y, int N)
{
    int star_X = X;
    for(int i = I; i<I+N; i++)
    {
	X = star_X;
	int j = (1<<7);
	while(j)
	{
	    if( ((mem[i]&j) != 0) && (X < 64) && (Y < 32))
	    {
		screen_buffer[Y][X] ^= 1;
		if(screen_buffer[Y][X] == 0) V[0xF] = 1; // set VF 1 if pixel was turned off
	    }
	    j = j>>1;
	    X++;
	}
	Y++;
    }
}


void load_manual_test()
{
    int i = 0x200; 
    mem[i] = 0xA1; i++;
    mem[i] = 0x00; i++;  // 0x200: 0xA100 

    mem[i] = 0x60; i++;
    mem[i] = 0x10; i++;  // 0x202: 0x6010 

    mem[i] = 0x61; i++;
    mem[i] = 0x02; i++;  // 0x204: 0x6102 

    mem[i] = 0xD0; i++;
    mem[i] = 0x15; i++;  // 0x206: 0xD015 

    mem[i] = 0x12; i++;
    mem[i] = 0x08; i++;  // 0x208: 0x1208 
}

void stop()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(1);
}


// init
void emu_start()
{
    for(int i=0; i<SCREEN_HEIGHT; i++)
    {
	for(int j=0; j<SCREEN_WIDTH; j++)
	{
	    screen_buffer[i][j] = 0; 
	}
    }
    load_rom("ibm.ch8");
    // load_manual_test();
    load_font();
    PC = 0x200;
    SB = 0x07FF;
    SP = SB;
    delay_timer = 0;
    sound_timer = 0;
}

// update
void emu_update()
{
    if(PC >= idx)
    {
	// printf("\n\n  hoe geche PC > rom size\n\n");
	// exit(1);
    }

    // fetch
    int inst;
    inst = mem[PC]; PC++; inst = inst << 8;
    inst = inst | mem[PC]; PC++;
    int X, Y, N, NN, NNN, mem_idx;

    if(debug) printf("PC = %x :   ", PC - 2);

    // decode & execute
    switch((int)inst>>12)
    {
    case 0x0: // ------------------------
	switch(inst)
	{
	case 0x00E0: // clear screen  inst:00E0
	    for(int i=0; i<SCREEN_HEIGHT; i++)
	    {
		for(int j=0; j<SCREEN_WIDTH; j++)
		{
		    screen_buffer[i][j] = 0;
		}
	    }
	    if(debug) printf("00E0: clear screen - %x\n", inst);
	    break;
	case 0x00EE: // return from a subroutine  inst:00EE
	    if(SP == SB) { printf("stack already empty\n"); exit(1); }
	    long ret = mem[++SP];
	    PC = ret;
	    if(debug) printf("00EE: return from subroutine - %x\n", inst);
	    break;
	default: // machine language routine  inst:0NNN
	    printf("execute machine language routine\n");
	    break;
	}
	break;
    case 0x1: // ------------------------ // jump to address  inst:1NNN
	PC = inst & 0x0FFF;
	if(debug) printf("1NNN: jump to address - %x\n", inst);
	break;
    case 0x2: // ------------------------ // call subroutine  inst:2NNN
	mem[SP--] = PC;
	PC = inst & 0x0FFF;
	if(debug) printf("2NNN: call subroutine - %x\n", inst);
	break;
    case 0x3: // ------------------------ // skip if VX == NN  inst:3XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	if(V[X] == NN) PC += 2;
	if(debug) printf("3XNN: skip - %x\n", inst);
	break;
    case 0x4: // ------------------------ // skip if VX != NN  inst:4XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	if(V[X] != NN) PC += 2;
	if(debug) printf("4XNN: skip - %x\n", inst);
	break;
    case 0x5: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // skip if VX == VY  inst:5XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] == V[Y]) PC += 2;
	    if(debug) printf("5XYN: skip - %x\n", inst);
	    break;
	default:
	    printf("invalid instruction: %x\n", inst);
	    break;
	}
	break;
    case 0x6: // ------------------------ VX = NN  inst:6XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] = NN;
	if(debug) printf("6XNN: VX = NN - %x\n", inst);
	break;
    case 0x7: // ------------------------ VX = VX + NN  inst:7XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] += NN;
	if(debug) printf("6XYN: VX = VX + NN - %x\n", inst);
	break;
    case 0x8: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // VX = VY  inst:8XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[Y];
	    if(debug) printf("8XY0: VX = VY - %x\n", inst);
	    break;
	case 0x0001: // VX = VX | VY  inst:8XY1
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] | V[Y];
	    if(debug) printf("8XY1: VX = VX | VY - %x\n", inst);
	    break;
	case 0x0002: // VX = VX & VY  inst:8XY2
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] & V[Y];
	    if(debug) printf("8XY2: VX = VX & VY - %x\n", inst);
	    break;
	case 0x0003: // VX = VX ^ VY  inst:8XY3
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] ^ V[Y];
	    if(debug) printf("8XY3: VX = VX ^ VY - %x\n", inst);
	    break;
	case 0x0004: // VX = VX + VY  inst:8XY4
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] + V[Y];
	    if(V[X] & 0x0100) V[0xF] = 0x01; else V[0xF] = 0x00; // check for carry 
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) printf("8XY4: VX = VX + VY - %x\n", inst);
	    break;
	case 0x0005: // VX = VX - VY  inst:8XY5
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] > V[Y]) V[0xF] = 0x01; else V[0xF] = 0x00; // check for borrow
	    V[X] = V[X] - V[Y];
	    if(debug) printf("8XY5: VX = VX - VY - %x\n", inst);
	    break;
	case 0x0006: // VX = VY >> 1  inst:8XY6
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(!SUPER) V[X] = V[Y];
	    V[0xF] = V[X] & 0x0001;
	    V[X] = V[X] >> 1;
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) printf("8XY6: VX = VY >> 1 - %x\n", inst);
	    break;
	case 0x0007: // VX = VY - VX  inst:8XY7
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[Y] > V[X]) V[0xF] = 0x01; else V[0xF] = 0x00; // check for borrow
	    V[X] = V[Y] - V[X];
	    if(debug) printf("8XY7: VX = VY - VX - %x\n", inst);
	    break;
	case 0x000E: // VX = VY << 1  inst:8XYE
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(!SUPER) V[X] = V[Y];
	    V[0xF] = V[X] & 0x0080;
	    V[X] = V[X] << 1;
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) printf("8XYE: VX = VY << 1 - %x\n", inst);
	    break;
	default:
	    printf("invalid instruction\n");
	    break;
	}
	break;
    case 0x9: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // skip if VX != VY  inst:0x9XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] != V[Y]) PC += 2;
	    if(debug) printf("9XY0: skip - %x\n", inst);
	    break;
	default:
	    printf("invalid instruction\n");
	    break;
	}
	break;
    case 0xA: // ------------------------ I = NNN  inst:0xANNN
	I = (inst & 0x0FFF);
	if(debug) printf("ANNN: I = NNN - %x\n", inst);
	break;
    case 0xB: // ------------------------ jump to NNN + V0  inst:0xBNNN
	PC = V[0] + (inst & 0x0FFF);
	if(debug) printf("BNNN: jump to NNN + V0 - %x\n", inst);
	break;
    case 0xC: // ------------------------ VX = rand() & NN  inst:CXNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] = rand() & NN;
	if(debug) printf("CXNN: VX = rand() & NN - %x\n", inst);
	break;
    case 0xD: // ------------------------ draw sprite  inst:0xDXYN
	X = (inst & 0x0F00)>>8; X = X&64;
	Y = (inst & 0x00F0)>>4; Y = Y%32;
	N = inst & 0x000F;
	draw_sprite(V[X], V[Y], N);
	if(debug) printf("DXYN: draw sprite - %x\n", inst);
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
	switch(inst & 0x00FF)
	{
	case 0x0007: // VX = delay timer  inst:0xFX07
	    X = (inst & 0x0F00)>>8;
	    V[X] = delay_timer;
	    if(debug) printf("FX07: VX = delay_timer - %x\n", inst);
	    break;
	case 0x000A: // VX  = input  inst:0xFX0A
	    if(debug) printf("FX0A: VX = input - %x\n", inst);
	    break;
	case 0x0015: // delay_timer = VX  inst:0xFX15
	    X = (inst & 0x0F00)>>8;
	    delay_timer = V[X];
	    if(debug) printf("FX15: delay_timer = VX - %x\n", inst);
	    break;
	case 0x0018: // sound_timer = VX  inst:0xFX18
	    X = (inst & 0x0F00)>>8;
	    sound_timer = V[X];
	    if(debug) printf("FX18: sound_timer = VX - %x\n", inst);
	    break;
	case 0x001E: // I = I + VX  inst:0xFX1E
	    X = (inst & 0x0F00)>>8;
	    I = I + V[X];
	    if(debug) printf("FX1E: I = I + VX - %x\n", inst);
	    break;
	case 0x0029: // get font addres in I  inst:0xFX29
	    X = (inst & 0x0F00)>>8;
	    int f = V[X] & 0x0F;
	    I = 0x100 + f*5;
	    if(debug) printf("FX29: get font address in I - %x\n", inst);
	    break;
	case 0x0033: // binary coded decimal  inst:0xFX33
	    X = (inst & 0x0F00)>>8;
	    mem[I] = V[X]/100;
	    mem[I+1] = (V[X]/10)%10;
	    mem[I+2] = V[X]%10;
	    if(debug) printf("FX33: binary coded decimal - %x\n", inst);
	    break;
	case 0x0055: // store registers in memory  inst:0xFX55
	    X = (inst & 0x0F00)>>8;
	    mem_idx = I;
	    for(int i=0; i<=X; i++)
	    {
		mem[mem_idx] = V[i];
		mem_idx++;
	    }
	    if(debug) printf("FX55: store registers - %x\n", inst);
	    break;
	case 0x0065: // get registers from memory  inst:0xFX65
	    X = (inst & 0x0F00)>>8;
	    mem_idx = I;
	    for(int i=0; i<=X; i++)
	    {
		V[i] = mem[mem_idx];
		mem_idx++;
	    }
	    if(debug) printf("FX65: get registers - %x\n", inst);
	    break;
	}
	break;
    default: // ------------------------
	printf("invalid instruction\n");
	break;
    }
    if(debug)
    {
	int ch = getchar();
	if(ch == 97) stop();
    }
}


