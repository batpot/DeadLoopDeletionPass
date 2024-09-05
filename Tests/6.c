#include <stdio.h>

// brisanje petlji koje se nikada ne izvrsavaju jer im je brojac 0

int main()
{
    // brise je - OK
    for(int i = 0; i < 0; i++)
    {

    }

    // brise je - OK
    for(int i = 0; i < 0; i++)
    {
        int c = 12 + i;
        i += 10;
    }

    int x = 0;
    // brise je - OK
    while(x < 0)
    {
        printf("brise");
    }



    return 0;
}