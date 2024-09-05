#include <stdio.h>

/*
    primer koji proverava da li se brisu petlje koje imaju samo store
    i ne brisu se one koje imaju artitmeticke operacije
*/

int main()
{
    // brise je - ok
    int y;
    for(int i = 0; i < 10; i++)
    {
        int tmp = 12;
        y = 11;
    }

    // ne brise je - ok
    for(int i = 0; i < 5; i++)
    {
        int tmp = 12;
        y++;
        int x = i+1;
    }

    // ne brise - OK
    int x = 10;
    while(y < 15)
    {
        x *= 12;
        y++;
    }

    // brise je - ok
    int z = 0;
    do {
        int s = 12;
        int tmp = 1;
    } while(z < 10);

    return 0;
}