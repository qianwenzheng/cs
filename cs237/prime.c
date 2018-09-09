/* (c)Qianwen (Tiffany) Zheng
 * This program reads an integer from the input and prints out the prime 
 * number/s, other than itself if it is prime, that is/are closest to it (in 
 * both the negative and positive directions on the number line, if applicable).
 * This program will search for primes in both directions of the given integer
 * and print whichever prime is closer, or will print the two primes if they 
 * are equidistant from the integer. If the user enters a negative number, he
 * is prompted to enter a positive one.
 * 9/17/17  
 */
#include <stdio.h>

/*
 *PRE: The function will only be called on odd numbers with the exception of 
 *two. The given integer n must be odd since even numbers other than two are 
 *not prime.
 *POST: Returns 1 if n is prime and prints n to output.
 */
int isPrime(int n)
{
  if(n==1)
    return 0;
  
  //Divides n by increasing odd numbers
  for(int i = 3; i*i<=n; i+=2) {
    if(n%i == 0)
      return 0;
  }
  printf("%d\n",n);
  return 1;
}

/*
 * Prints the closest prime number/s to the given integer n to output.
 */
void getPrimes(int n)
{
  //Determines the closest odd numbers smaller and larger than n
  int small; 
  int big;
  if(n%2 == 0){
    small = n-1;
    big = n+1;
  }else{
    small = n-2;
    big = n+2;
  }

  //Values that hold 1 or 0 depending on if small/large is prime
  int smIsPrime;
  int bigIsPrime;
  
  //Iterates through odd numbers in both directions until a prime is found
  do
    {
      smIsPrime = isPrime(small);
      bigIsPrime = isPrime(big);
      small-=2;
      big+=2;
    } while((small>2 && smIsPrime==0)&&bigIsPrime==0);
}

//Reads in an integer from input and prints its closest primes
int main()
{
  int n;  //integer read from input
  scanf("%d",&n);
  
  //Special case when n is negative
  if(n<0){
    printf("\n%s\n", "Error: negative numbers not valid input!");
    return -1;
  }
  //Special case when n is two
  else if(n==2)
    printf("%d\n",3);
  //Special case when the closest prime is two (not an odd number)
  else if(n<4)
    printf("%d\n", 2);
  //All other cases
  else
    getPrimes(n);
}
