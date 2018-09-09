// Print prime radicals of numbers.
// (c) Wendy Rhodes
#include <stdio.h>
#include <stdlib.h>

int rad(int wow)
{
  int prod = 1;
  int baz = 2;
  int before;
  while (wow > 1) {
    before = wow;
    while (wow % baz == 0) {
      wow = wow/baz;	     // factor!
    }
    if (wow < before) prod = prod * baz;  // product!
    baz += 1;
  }
  return prod;
}

int main()
{
  int n;
  int rd;
  int count;
  count = fscanf(stdin,"%d",&n);
  while (count == 1) {
    rd = rad(n);  // do it!
    printf("%d\n",rd);
    count = fscanf(stdin,"%d",&n);
  }
}
