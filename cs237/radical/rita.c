// A program to report radicals of positive integers found on input.
// (c) 2017 Rita Decoda

// The radical of a number is the product of unique primes that appear
// in its prime factorization.  The radical is defined for positive values
// only.  I reduce n by successively large integers that we can assume are prime.
// (Assume one, p, was composite.  p is the product of primes, smaller than
//  p, but all those factors have been removed earlier in the process, so
//  p will not divide the current value of n.)

// N.B. Much of this code was borrowed from my pfactor program, the prints the
// prime factorization of integers found on the input.

#include <stdio.h>
#include <stdlib.h>

// compute the radical of positive n; returns 0 on error.
int radical(int n)
{
  int rad = 1;
  int pfactor = 2;
  if (n <= 0) return 0;
  while (n > 1) {
    if (n%pfactor == 0) {	// found prime factor; composites accounted for
      rad *= pfactor;		// accumulate the prime
      do {
	n /= pfactor;		// remove all instances of this factor
      } while (n%pfactor == 0);
    }
    pfactor++;			// next (possible) prime factor
  }
  return rad;
}

int main()
{
  int n;
  // assuming input consists of integers, read and process each one
  while (1 == scanf("%d",&n)) {
    // could process errors....but don't
    printf("%d\n",radical(n));
  }
  return 0;
}
