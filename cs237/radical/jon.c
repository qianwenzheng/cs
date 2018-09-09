// Print radicals.
#include <stdio.h>
#include <stdlib.h>

int main()
{
  char buffer[20];
  while (buffer == fgets(buffer,20,stdin)) {
    int n = atoi(buffer);
    int r = 1;
    int p = 2;
    while (n > 1) {
      int first = 1;	// true only the first time we go around this loop
      while (n % p == 0) {
	n = n/p;
	if (first) {
	  r = r*p;
	  first = 0;
	}
      }
      p += 1;
    }
    printf("%d",r);
    putchar('\n');
  }
}
