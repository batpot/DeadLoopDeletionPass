#include <stdio.h>

int main() {

    int x = 0;
    // ne brise - OK
    for(int j = 0; j < x; j++){
        x = x*1;
    	printf("ne brise seee");
    }
    
    // brise - OK
    for(int j = 0; j < x; j++){
        x = x*1;
    }
    
}


