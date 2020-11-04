#include <stdio.h>
#include "util.h"

int mem[2000];
int PC;
int idx;

void test()
{
    int flag[10]; for(int i=0; i<10; i++) flag[i] = 0;
    while(PC < idx)
    {
	// fetch
	int inst;
	inst = mem[PC]; PC++; inst = inst << 8;
	inst = inst | mem[PC]; PC++;

	// printf("inst: %x\n", inst);
	// decode & execute
	switch((int)inst>>12)
	{
	case 0x0: // ------------------------
	    switch(inst)
	    {
	    case 0x00E0:
		// printf(">> clear the screen\n");
		break;
	    case 0x00EE:
		// printf(">> return from a subroutine\n");
		break;
	    default:
		// printf("execute machine language instruction\n");
		break;
	    }
	    break;
	case 0x1: // ------------------------
	    // printf(">> jump to address\n");
	    break;
	case 0x2: // ------------------------
	    // printf(">> execute subroutine starting from address\n");
	    break;
	case 0x3: // ------------------------
	    // printf(">> skip the following instruction if VX equals\n");
	    break;
	case 0x4: // ------------------------
	    // printf(">> skip the following instruction if VX does not equal\n");
	    break;
	case 0x5: // ------------------------
	    // printf(">> skip the following instruction if VX = VY\n");
	    break;
	case 0x6: // ------------------------
	    // printf(">> store NN in VX\n");
	    break;
	case 0x7: // ------------------------
	    // printf(">> add NN to VX\n");
	    break;
	case 0x8: // ------------------------
	    switch(inst&0x000F)
	    {
	    case 0x0000:
		// printf(">> VX = VY\n");
		break;
	    case 0x0001:
		// printf(">> VX = VX | VY\n");
		break;
	    case 0x0002:
		// printf(">> VX = VX & VY\n");
		break;
	    case 0x0003:
		// printf(">> VX = VX ^ VY\n");
		break;
	    case 0x0004:
		// printf(">> VX = VX + VY; carry -> VF\n");
		break;
	    case 0x0005:
		// printf(">> VX = VX - VY; borrow -> VF\n");
		break;
	    case 0x0006:
		// printf(">> VX = VY >> 1; lsb -> VF\n");
		break;
	    case 0x0007:
		// printf(">> VX = VY - VX; borrow -> VF\n");
		break;
	    case 0x000E:
		// printf(">> VX = VY << 1; msb -> VF\n");
		break;
	    default:
		// printf("invalid instruction\n");
		flag[1] += 1;
		break;
	    }
	    break;
	case 0x9: // ------------------------
	    switch(inst&0x000F)
	    {
	    case 0x0000:
		// printf(">> skip if VX != VY\n");
		break;
	    default:
		// printf("invalid instruction\n");
		flag[2] += 1;
		break;
	    }
	    break;
	case 0xA: // ------------------------
	    // printf(">> I = NNN\n");
	    break;
	case 0xB: // ------------------------
	    // printf(">> jump to NNN + V0\n");
	    break;
	case 0xC: // ------------------------
	    // printf(">> VX to random number with a mask of NN\n");
	    break;
	case 0xD: // ------------------------
	    // printf(">> draw sprite\n");
	    break;
	case 0xE: // ------------------------
	    switch(0x00FF)
	    {
	    case 0x009E:
		// printf(">> skip if key pressed\n");
		break;
	    case 0x00A1:
		// printf(">> skip if key not pressed\n");
		break;
	    default:
		// printf("invalid instruction\n");
		flag[3] += 1;
		break;
	    }
	    break;
	case 0xF: // ------------------------
	    switch(inst&0x00FF)
	    {
	    case 0x0007:
		// printf(">> VX = delay timer\n");
		break;
	    case 0x000A:
		// printf(">> VX = input\n");
		break;
	    case 0x0015:
		// printf(">> delay timer = VX\n");
		break;
	    case 0x0018:
		// printf(">> sound timer = VX\n");
		break;
	    case 0x001E:
		// printf(">> I = I + VX\n");
		break;
	    case 0x0029:
		// printf(">> I = font sprite data address\n");
		break;
	    case 0x0033:
		// printf(">> binary coded decimal diye kichu ekta\n");
		break;
	    case 0x0055:
		// printf(">> store values of registers in memory\n");
		break;
	    case 0x0065:
		// printf(">> get values of registers from memory\n");
		break;
	    default:
		// printf("invalid instruction");
		flag[4] += 1;
		break;
	    }
	    break;
	default: // ------------------------
	    // printf("invalid instruction\n");
	    flag[5] += 1;
	    break;
	}   
    }
    for(int i=1; i<=5; i++)
	printf("flag[%d] = %d  ", i, flag[i]);
}


int main()
{
    int a = 0x370F;
    int b = (a&0x0F00)>>8;
    int c = (a&0x00FF);
    printf("b = %d c = %d\n\n", b, c);

    FILE* fp = fopen("space_invaders.ch8", "rb");
    unsigned char buffer[2000];
    int i = 0;
    while(fread(buffer+i, 1, 1, fp))
	i++;
    int N = i;
    printf("%d bytes\n\n", N);

    idx = 0x200;
    for(i=0; i<N; i++)
	mem[idx++] = (int)buffer[i];
    printf("idx = %d\n\n", idx);


    for(int i=0; i<10; i++)
    {
	printf("%d: ", i);
	PC = 0x200 + i;
	test();
	printf("\n");
	// break;
    }
    printf(" ttttt \n\n");

    // i = 0x200;
    // while(i < 0x200+N)
    // {
    // 	show_byte(mem[i]);
    // 	printf(" ");
    // 	i++;
    // 	if(i%8 == 0) printf(" ");
    // 	if(i%16 == 0) printf("\n");
    // }
    // printf("\n");
    return 0;
}
