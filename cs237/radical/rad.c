//compute and print the radicals of integers read from input
// (c) 2017 Qianwen (Tiffany) Zheng
#include <stdio.h>

int radical(int n)
{
  
  int result = 1;
  int p = 2;
  while (n > 1){
    if (n%p == 0){
      //whack
      while (n%p == 0){
	n/= p;
      }
      result*= p;
    }
    p++;
  }
  return result;
}

int main()
{
  int n;
  while (1== scanf("%d",&n)) {
    printf("%d\n",radical(n));
  }
}
