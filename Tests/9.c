#include <stdio.h>

int main() {

    int x = 0;
    int y = 3;

    // ne brise - OK
    do {
      if(0)
        x++;
      y += 3;
    } while(x < 10);

    // brise - OK
    do {
        if(0)
            x++;
    } while(x < 10);

    return 0;
}
