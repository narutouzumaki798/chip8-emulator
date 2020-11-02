#include <stdio.h>
#include "util.h"

int main()
{
    // char* a[100];
    // a[0] = "abcd";
    // a[1] = "bbcd";
    // a[2] = "cbcd";
    // a[3] = "dbcd";
    // printf("%s\n\n", a[1]);

    int a = 0x10;
    printf("a = %d\n\n", a);

    FILE* fp = fopen("space_invaders.ch8", "rb");
    unsigned char buffer[2000];
    int i = 0;
    while(fread(buffer+i, 1, 1, fp))
	i++;

    int n = i;
    printf("%d bytes\n\n", n);
    i = 0;
    while(i < n)
    {
	    show_byte(buffer[i]);
	    printf(" ");
	    i++;
		if(i%8 == 0) printf(" ");
		if(i%16 == 0) printf("\n");
	}
	printf("\n");
    return 0;
}
