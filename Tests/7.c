#include <stdio.h>

// primer za udnjezdene petlje

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

    // ne brise while, ne brise for - OK
    for(int i = 0; i < 10; i++)
    {
        printf("nece obrisati ovu");

        while(i < 5)
        {
            int y = i + 1;
            printf("%d\n", y);
        }

    }

    // brise spoljasnju, unutrasnju ispisuje da nije obrisana ali je brise u IR
    for(int j = 0; j < 0; j++)
    {
        printf("");

        for(int i = j + 1; i < 10; i++)
        {
            printf("ll");
        }
    }

    return 0;
}