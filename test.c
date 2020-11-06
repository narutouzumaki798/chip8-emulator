#include <stdio.h>
#include <stdlib.h>


#include "util.h"

int mem[2000];
int PC;
int idx;


int test()
{
    int a = 0x37FF;
    int b = (a&0x0F00)>>8;
    int c = (a&0x00F0)>>4;
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

int main()
{
    char tmp = NULL;
    append1(&tmp, "registers");
    printf("tmp: %s\n", tmp);
    free(tmp); tmp = NULL;
    append1(&tmp, "V");
    printf("tmp: %s\n", tmp);
    return 0;
}

