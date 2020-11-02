char show_byte(unsigned char a)
{
    int x = (int)a&15;
    int y = a>>4;

    if(y < 10) printf("%d", y);
    else printf("%c", 'A'+(y-10));

    if(x < 10) printf("%d", x);
    else printf("%c", 'A'+(x-10));
}
