char show_byte(int a)
{
    int x = (int)a&15;
    int y = a>>4;

    if(y < 10) printf("%d", y);
    else printf("%c", 'A'+(y-10));

    if(x < 10) printf("%d", x);
    else printf("%c", 'A'+(x-10));
}

char* to_hex2(int a) // 2 bytes
{
    char* ans = (char*)malloc(7*sizeof(char));
    int d;
    ans[0] = '0'; ans[1] = 'x';
    ans[6] = '\0';

    d = (a & 0xF000)>>12;
    ans[2] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = (a & 0x0F00)>>8;
    ans[3] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = (a & 0x00F0)>>4;
    ans[4] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = a & 0x000F;
    ans[5] = (d < 10)? ('0' + d) : ('A' + (d - 10));

    return ans;
}

char* to_hex1(int a) // 1 byte
{
    char* ans = (char*)malloc(5*sizeof(char));
    int d;
    ans[0] = '0'; ans[1] = 'x';
    ans[4] = '\0';

    d = (a & 0x00F0)>>4;
    ans[2] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = a & 0x000F;
    ans[3] = (d < 10)? ('0' + d) : ('A' + (d - 10));

    return ans;
}

void append1(char** a, char* b)
{
    char* ta = (*a);
    int n = 0;
    if(ta != NULL)
    {
	while(ta[n] != '\0') n++;
    }
    int m = 0;
    if(b != NULL)
    {
	while(b[m] != '\0') m++;
    }
    char* tmp = (char*)malloc((m+n+1)*sizeof(char));
    for(int i=0; i<n; i++) tmp[i] = ta[i];
    // free(ta);
    for(int i=0; i<m; i++) tmp[n+i] = b[i];
    tmp[n+m] = '\0';
    (*a) = tmp;
}

void append2(char** a, char b)
{
    char* ta = (*a);
    int n = 0;
    if(ta != NULL)
    {
	while(ta[n] != '\0') n++;
    }
    char* tmp = (char*)malloc((n+2)*sizeof(char));
    for(int i=0; i<n; i++) tmp[i] = ta[i];
    tmp[n] = b;
    tmp[n+1] = '\0';
    (*a) = tmp;
}



