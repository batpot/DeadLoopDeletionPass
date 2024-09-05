#include <stdio.h>

int main() {

    int x = 0;
    
    // brise - OK 
    for(int i=0; i< 10; i++){
    	x *= 1;
    }
     
     
    // brise - OK
    while(x < 3) {
    	x = x*1;
    }
    
    
    // brise - OK
    do {
        x = x * 1;
    } while(x < 10);
    
     
    return 0;
}

