#include <stdio.h>

int main(){

  int x = 0;
  // ne brise - ok
  for(int i=0; i< 5; i++){
      x++;
  }
  
  // ne brise - ok
  while(x < 10){
      x *= 5;
  }


  return 0;
}
