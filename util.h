char show_byte(int a)
{
    int x = (int)a&15;
    int y = a>>4;

    if(y < 10) printf("%d", y);
    else printf("%c", 'A'+(y-10));

    if(x < 10) printf("%d", x);
    else printf("%c", 'A'+(x-10));
}

char* to_hex(int a)
{
    char* ans = (char*)malloc(5*sizeof(char));
    int d;
    ans[4] = '\0';

    d = (a & 0xF000)>>12;
    ans[0] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = (a & 0x0F00)>>8;
    ans[1] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = (a & 0x00F0)>>4;
    ans[2] = (d < 10)? ('0' + d) : ('A' + (d - 10));
    d = a & 0x000F;
    ans[3] = (d < 10)? ('0' + d) : ('A' + (d - 10));

    return ans;
}


