// A small program to compute the bits of the root of an 8 bit value, n.
// Compile with -DUSE_C to use the C source for root.
// (c) 2013, 2017 Duane A. Bailey
#include <stdio.h>
#include <stdlib.h>

// prepare for possible definition outside this procedure
extern void root(int, char []);

#ifdef USE_C
// Compute the square root of n as a string of digits.
void root(int n, char bits[])
{
  int m = 0;		   // the remainder,
  int r = 0;		   // the root,
  int v;                   // the intermediate value
  int i;		   // loop counter
  for (i=0; i<4; i++) {    // compute a 4 bit answer
    v = (m<<2) + (n>>6);   // drop down 2 highest bits
    n = (n<<2) & 0xff;     // shift n left, remove high bits
    m = v-((r<<2)+1);      // compute temporary remainder
    if (m < 0) {
      bits[i] = '0';
      m = v;               // remainder was negative, r[i] = 0
      r <<= 1;
    } else {
      bits[i] = '1';
      r = (r<<1)+1;        // remainder not negative, r[i] = 1
    }
  }
  bits[i] = 0;             // terminate string
}
#endif

int main(int argc, char **argv)
{
  char bits[5];
  int n = (argc > 1) ? atoi(argv[1]):81; // compute root of arg, or 81
  root(n,bits);
  printf("Square root of %d is %s (binary).\n",n,bits);
  return 0;
}
