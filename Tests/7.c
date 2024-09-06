#include <stdio.h>

// primer za ugnjezdene petlje

int main()
{

    // brise while, ne brise for - OK
    for(int i = 0; i < 10; i++)
    {
        printf("nece obrisati ovu");

        while(i < 5)
        {
            // brise ovu
        }
    }

    return 0;
}