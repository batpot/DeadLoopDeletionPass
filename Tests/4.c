#include <stdio.h>

int main() {

    int x = 0;
    // ne brise - OK
    while(x < 20){
      x++;
    }
    
    // brise - OK
    while(x < 30) {
    
    }

    // brise - OK 
    do {
    
    } while(x < 30);

}
