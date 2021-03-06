

// i will use long 16 bit registers/locations
// and int for 8 bit registers/locations just for convinience

// original COSMAC VIP base system had 2K ram
int* mem; // size = 2048
// each element of the array will store 1 byte
int idx; // memory idx

long* PC; // program counter
long* I; // index register
long* SP; // stack pointer
long* SB; // stack base

int* V; // register file; size = 16

int* delay_timer; // delay timer
int* sound_timer; // sound timer

int* keys; // keypad flags; size = 16


sem_t* semaphore1; // for synchronization
int pid;

#define debug 0
// debug mode:
// 0 = no
// 1 = print instructions
// 2 = step through instructions

// debug output
FILE* err_fp;


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
    idx = 0x200;
    for(i=0; i<N; i++)
	mem[idx++] = (int)buffer[i];

    // for(i=0x200; i<0x200+N; i++)
    // 	if(i%2 == 1)
    // 	    fprintf(err_fp, "%s: %s\n", to_hex(i-1), to_hex((mem[i-1]<<8)|mem[i]) );
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

void load_manual_test() // 0 print kore atke jabe
{
    int i = 0x200; 
    mem[i] = 0x00; i++;
    mem[i] = 0xE0; i++;  // 0x200: 0x00E0

    mem[i] = 0x60; i++;
    mem[i] = 0x10; i++;  // 0x202: 0x6010

    mem[i] = 0x61; i++;
    mem[i] = 0x05; i++;  // 0x204: 0x6105

    mem[i] = 0x62; i++;
    mem[i] = 0x00; i++;  // 0x206: 0x6200
	 
    mem[i] = 0xA1; i++;
    mem[i] = 0x00; i++;  // 0x208: 0xA100

    mem[i] = 0xE2; i++;
    mem[i] = 0xA1; i++;  // 0x20A: 0xE2A1

    mem[i] = 0xA1; i++;
    mem[i] = 0x05; i++;  // 0x20C: 0xA105

    mem[i] = 0xD0; i++;
    mem[i] = 0x15; i++;  // 0x20E: 0xD015

    mem[i] = 0x12; i++;
    mem[i] = 0x00; i++;  // 0x210: 0x1200

    idx = i;
}

void draw_sprite(int X, int Y, int N)
{
    int star_X = X; V[0xF] = 0x0;
    for(int i = (*I); i<(*I)+N; i++)
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

int check_key(int X)
{
    int flag = 0;
    for(int i=0; i<16; i++)
    {
	if(keys[i])
	{
	    V[X] = i;
	    flag = 1;
	}
    }
    return flag;
}

void stop()
{
    sem_wait(semaphore1);
    (*PC) = 0;
    sem_post(semaphore1);
    SDL_DestroyWindow(window);
    SDL_Quit();
    fprintf(err_fp, "stop: wait start\n"); fflush(err_fp);
    wait(NULL);
    fprintf(err_fp, "stop: wait end\n"); fflush(err_fp);
    exit(1);
}

int _8bit_sub(int a, int b)
{
    a = a & 0x00FF;
    b = b & 0x00FF;

    b = b ^ 0xFF; b = b + 1; b = b & 0xFF; // 2's complement
    a = a + b;
    a = a & 0xFF;
    return a;
}


#include "internal_state.c"

// init
void emu_start()
{
    for(int i=0; i<SCREEN_HEIGHT; i++)
	for(int j=0; j<SCREEN_WIDTH; j++)
	    screen_buffer[i][j] = 0; 

    // init memory
    mem = (int*)create_shared_memory(2048*sizeof(int));
    for(int i=0; i<2048; i++) mem[i] = 0;
    // init registers
    V  = (int*)create_shared_memory(16*sizeof(int));
    PC = (long*)create_shared_memory(sizeof(long));
    I  = (long*)create_shared_memory(sizeof(long));
    SP = (long*)create_shared_memory(sizeof(long));
    SB = (long*)create_shared_memory(sizeof(long));
    // key flags
    keys = (int*)create_shared_memory(16*sizeof(int));
    for(int i=0; i<16; i++) keys[i] = 0;
    // timers
    delay_timer = (int*)create_shared_memory(sizeof(int));
    sound_timer = (int*)create_shared_memory(sizeof(int));

    err_fp = fopen("err.txt", "w");
    load_font();
    load_rom("space_invaders.ch8");
    // load_manual_test();

    for(int i=0; i<16; i++) keys[i] = 0;
    (*PC) = 0x200;
    (*SB) = 0x07FF;
    (*SP) = (*SB);
    (*delay_timer) = 0;
    (*sound_timer) = 0;


    semaphore1 = (sem_t*)create_shared_memory(sizeof(sem_t));
    sem_init(semaphore1, 1, 1);
    pid = fork();
    if(pid == 0) internal_state();
}

// update
void emu_update()
{
    sem_wait(semaphore1);
    if((*PC) >= idx)  // hoa uchit noe
    {
	fprintf(err_fp, "  rom er baere\n");
	// exit(1);
    }

    // fetch
    int inst;
    inst = mem[(*PC)]; (*PC) = (*PC)+1; inst = inst << 8;
    inst = inst | mem[(*PC)]; (*PC) = (*PC)+1;
    int X, Y, N, NN, NNN, mem_idx;

    if(debug) fprintf(err_fp, "PC = %x :   ", (*PC) - 2);

    // decode & execute
    switch((int)inst>>12)
    {
    case 0x0: // ------------------------
	switch(inst)
	{
	case 0x00E0: // clear screen  inst:00E0
	    for(int i=0; i<SCREEN_HEIGHT; i++)
		for(int j=0; j<SCREEN_WIDTH; j++)
		    screen_buffer[i][j] = 0;
	    if(debug) fprintf(err_fp, "00E0: clear screen - %x pid=%d\n", inst, pid);
	    break;
	case 0x00EE: // return from a subroutine  inst:00EE
	    if((*SP) == (*SB)) { fprintf(err_fp, "stack already empty pid=%d\n", pid); exit(1); }
	    (*SP) = (*SP)+1;
	    long ret = mem[(*SP)];
	    (*PC) = ret;
	    if(debug) fprintf(err_fp, "00EE: return from subroutine - %x pid=%d\n", inst, pid);
	    break;
	default: // machine language routine  inst:0NNN
	    fprintf(err_fp, "execute machine language routine\n");
	    break;
	}
	break;
    case 0x1: // ------------------------ // jump to address  inst:1NNN
	(*PC) = inst & 0x0FFF;
	if(debug) fprintf(err_fp, "1NNN: jump to address - %x pid=%d\n", inst, pid);
	break;
    case 0x2: // ------------------------ // call subroutine  inst:2NNN
	mem[(*SP)] = (*PC);
	(*SP) = (*SP)-1;
	(*PC) = inst & 0x0FFF;
	if(debug) fprintf(err_fp, "2NNN: call subroutine - %x pid=%d\n", inst, pid);
	break;
    case 0x3: // ------------------------ // skip if VX == NN  inst:3XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	if(V[X] == NN) (*PC) = (*PC)+2;
	if(debug) fprintf(err_fp, "3XNN: skip - %x pid=%d\n", inst, pid);
	break;
    case 0x4: // ------------------------ // skip if VX != NN  inst:4XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	if(V[X] != NN) (*PC) = (*PC)+2;
	if(debug) fprintf(err_fp, "4XNN: skip - %x pid=%d\n", inst, pid);
	break;
    case 0x5: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // skip if VX == VY  inst:5XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] == V[Y]) (*PC) = (*PC)+2;
	    if(debug) fprintf(err_fp, "5XYN: skip - %x pid=%d\n", inst, pid);
	    break;
	default:
	    fprintf(err_fp, "invalid instruction: %x pid=%d\n", inst, pid);
	    break;
	}
	break;
    case 0x6: // ------------------------ VX = NN  inst:6XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] = NN;
	if(debug) fprintf(err_fp, "6XNN: VX = NN - %x pid=%d\n", inst, pid);
	break;
    case 0x7: // ------------------------ VX = VX + NN  inst:7XNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] += NN;
	V[X] = V[X] & 0x00FF; // 8 bit register
	if(debug) fprintf(err_fp, "7XNN: VX = VX + NN - %x pid=%d\n", inst, pid);
	break;
    case 0x8: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // VX = VY  inst:8XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[Y];
	    if(debug) fprintf(err_fp, "8XY0: VX = VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0001: // VX = VX | VY  inst:8XY1
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] | V[Y];
	    if(debug) fprintf(err_fp, "8XY1: VX = VX | VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0002: // VX = VX & VY  inst:8XY2
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] & V[Y];
	    if(debug) fprintf(err_fp, "8XY2: VX = VX & VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0003: // VX = VX ^ VY  inst:8XY3
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] ^ V[Y];
	    if(debug) fprintf(err_fp, "8XY3: VX = VX ^ VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0004: // VX = VX + VY  inst:8XY4
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    V[X] = V[X] + V[Y];
	    if(V[X] & 0x0100) V[0xF] = 0x01; else V[0xF] = 0x00; // check for carry 
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) fprintf(err_fp, "8XY4: VX = VX + VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0005: // VX = VX - VY  inst:8XY5
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] > V[Y]) V[0xF] = 0x01; else V[0xF] = 0x00; // check for borrow
	    V[X] = _8bit_sub(V[X], V[Y]);
	    if(debug) fprintf(err_fp, "8XY5: VX = VX - VY - %x pid=%d\n", inst, pid);
	    break;
	case 0x0006: // VX = VY >> 1  inst:8XY6
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(!SUPER) V[X] = V[Y];
	    V[0xF] = V[X] & 0x0001;
	    V[X] = V[X] >> 1;
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) fprintf(err_fp, "8XY6: VX = VY >> 1 - %x pid=%d\n", inst, pid);
	    break;
	case 0x0007: // VX = VY - VX  inst:8XY7
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[Y] > V[X]) V[0xF] = 0x01; else V[0xF] = 0x00; // check for borrow
	    V[X] = _8bit_sub(V[Y], V[X]);
	    if(debug) fprintf(err_fp, "8XY7: VX = VY - VX - %x pid=%d\n", inst, pid);
	    break;
	case 0x000E: // VX = VY << 1  inst:8XYE
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(!SUPER) V[X] = V[Y];
	    V[0xF] = V[X] & 0x0080;
	    V[X] = V[X] << 1;
	    V[X] = V[X] & 0x00FF; // 8 bit register
	    if(debug) fprintf(err_fp, "8XYE: VX = VY << 1 - %x pid=%d\n", inst, pid);
	    break;
	default:
	    fprintf(err_fp, "invalid instruction\n");
	    break;
	}
	break;
    case 0x9: // ------------------------
	switch(inst & 0x000F)
	{
	case 0x0000: // skip if VX != VY  inst:0x9XY0
	    X = (inst & 0x0F00)>>8;
	    Y = (inst & 0x00F0)>>4;
	    if(V[X] != V[Y]) (*PC) = (*PC)+2;
	    if(debug) fprintf(err_fp, "9XY0: skip - %x pid=%d\n", inst, pid);
	    break;
	default:
	    fprintf(err_fp, "invalid instruction\n");
	    break;
	}
	break;
    case 0xA: // ------------------------ I = NNN  inst:0xANNN
	(*I) = (inst & 0x0FFF);
	if(debug) fprintf(err_fp, "ANNN: I = NNN - %x pid=%d\n", inst, pid);
	break;
    case 0xB: // ------------------------ jump to NNN + V0  inst:0xBNNN
	(*PC) = V[0] + (inst & 0x0FFF);
	if(debug) fprintf(err_fp, "BNNN: jump to NNN + V0 - %x pid=%d\n",
			  inst, pid);
	break;
    case 0xC: // ------------------------ VX = rand() & NN  inst:CXNN
	X = (inst & 0x0F00)>>8;
	NN = (inst & 0x00FF);
	V[X] = rand() & NN;
	if(debug) fprintf(err_fp, "CXNN: VX = rand() & NN - %x pid=%d\n", inst, pid);
	break;
    case 0xD: // ------------------------ draw sprite  inst:0xDXYN
	X = (inst & 0x0F00)>>8; X = X%64;
	Y = (inst & 0x00F0)>>4; Y = Y%32;
	N = inst & 0x000F;
	draw_sprite(V[X], V[Y], N);
	if(debug) fprintf(err_fp, "DXYN: draw sprite - %x pid=%d\n", inst, pid);
	break;
    case 0xE: // ------------------------
	switch(inst & 0x00FF)
	{
	case 0x009E: // skip if key pressed  inst:0xEX9E
	    X = (inst & 0x0F00)>>8;
	    if(keys[V[X]]) (*PC) = (*PC)+2;
	    break;
	case 0x00A1: // skip if key not pressed  inst:0xEXA1
	    X = (inst & 0x0F00)>>8;
	    if(!keys[V[X]]) (*PC) = (*PC)+2;
	    if(debug) fprintf(err_fp, "EXA1: skip if key not pressed - %x pid=%d\n", inst, pid);
	    break;
	default:
	    fprintf(err_fp, "invalid instruction\n");
	    break;
	}
	break;
    case 0xF: // ------------------------
	switch(inst & 0x00FF)
	{
	case 0x0007: // VX = delay timer  inst:0xFX07
	    X = (inst & 0x0F00)>>8;
	    V[X] = (*delay_timer);
	    if(debug) fprintf(err_fp, "FX07: VX = delay_timer - %x pid=%d\n", inst, pid);
	    break;
	case 0x000A: // VX  = input  inst:0xFX0A
	    X = (inst & 0x0F00)>>8;
	    if(!check_key(X)) (*PC) = (*PC)-2;
	    break;
	case 0x0015: // delay_timer = VX  inst:0xFX15
	    X = (inst & 0x0F00)>>8;
	    (*delay_timer) = V[X];
	    if(debug) fprintf(err_fp, "FX15: delay_timer = VX - %x pid=%d\n", inst, pid);
	    break;
	case 0x0018: // sound_timer = VX  inst:0xFX18
	    X = (inst & 0x0F00)>>8;
	    (*sound_timer) = V[X];
	    if(debug) fprintf(err_fp, "FX18: sound_timer = VX - %x pid=%d\n", inst, pid);
	    break;
	case 0x001E: // I = I + VX  inst:0xFX1E
	    X = (inst & 0x0F00)>>8;
	    (*I) = (*I) + V[X];
	    if(debug) fprintf(err_fp, "FX1E: I = I + VX - %x pid=%d\n", inst, pid);
	    break;
	case 0x0029: // get font addres in I  inst:0xFX29
	    X = (inst & 0x0F00)>>8;
	    int f = V[X] & 0x0F;
	    (*I) = 0x100 + f*5;
	    if(debug) fprintf(err_fp, "FX29: get font address in I - %x pid=%d\n", inst, pid);
	    break;
	case 0x0033: // binary coded decimal  inst:0xFX33
	    X = (inst & 0x0F00)>>8;
	    mem[(*I)] = V[X]/100;
	    mem[(*I)+1] = (V[X]/10)%10;
	    mem[(*I)+2] = V[X]%10;
	    if(debug) fprintf(err_fp, "FX33: binary coded decimal - %x pid=%d\n", inst, pid);
	    break;
	case 0x0055: // store registers in memory  inst:0xFX55
	    X = (inst & 0x0F00)>>8;
	    mem_idx = (*I);
	    for(int i=0; i<=X; i++)
	    {
		mem[mem_idx] = V[i];
		mem_idx++;
	    }
	    if(debug) fprintf(err_fp, "FX55: store registers - %x pid=%d\n", inst, pid);
	    break;
	case 0x0065: // get registers from memory  inst:0xFX65
	    X = (inst & 0x0F00)>>8;
	    mem_idx = (*I);
	    for(int i=0; i<=X; i++)
	    {
		V[i] = mem[mem_idx];
		mem_idx++;
	    }
	    if(debug) fprintf(err_fp, "FX65: get registers - %x pid=%d\n", inst, pid);
	    break;
	}
	break;
    default: // ------------------------
	fprintf(err_fp, "invalid instruction\n");
	break;
    }
    sem_post(semaphore1);

    if(debug)
	fflush(err_fp);

}


