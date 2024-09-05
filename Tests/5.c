#include <stdio.h>

int main() {

    int x = 0;
    // ne brise - OK
    do {
      x++;
    } while(x < 10);
    
    
    // ne brise - OK
    int y = 5;
    do {
      x = y * 1;
    } while(x < 10);
    
    
    // brise - OK
    y = 5;
    do {
      x = x * 1;
    } while(x < 10);
    
    
    
}
