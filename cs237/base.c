/*(c)Qianwen (Tiffany) Zheng
 * 10/15/17 
 * This program takes in a single command-line argument b, an integer between
 * 2 and 36, and converts unsigned integers from input into their base b 
 * representation and outputs the converted values on separate lines.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*                                                                                                         
 * Takes in an integer and a specific base b and returns a string of the base b representation             
 * of the integer                                                                                          
 */
void baseRep(char* result,int n, int b)
{
  char rem;                          //remainder when curr/b
  char *temp;                        //temporary array of chars to hold the backwards string 
  //char *result;                      //final string representing converted value to be returned
  int max = 4;                       //size of temp 
  int currSize = 0;                  //number of elements in temp currently
  temp = (char*)malloc(max);
  
  do{
    if(n%b < 10) rem = n%b + 48;     //mapping the decimal integer to a numeric char value
    else rem = n%b + 55;             //mapping the decimal integer to an alphabetic char value
    n = n/b;
    temp[currSize]=rem;              //concatenates the previously calculated digits to the current digit
    ++currSize;
    if(currSize + 1 == max) {        //allocates more space in temp if currSize increases to a certain extent
      max = max*2;
      temp = (char*)realloc(temp,max);
    }
  } while (n != 0);                  //builds up the converted value

  //reverse the string
  //char result[currSize+1];
  //result = (char*)malloc(currSize+1);

  for(int i=0; i < currSize; i++){
    result[i] = temp[currSize-i-1];
  }
  free(temp);
  result[currSize] = '\0';
}


int main(int argc, char* argv[])
{
  int b = atoi(argv[1]);            //the base b
  if(b<2 || b>36){
    printf("\n Integer must be between 2 and 36!");
    return -1;
  }

  int n;                           //the integer to be represented in base b
  while(n != EOF){
      scanf("%d", &n);
      char *result =(char*)malloc(17);
      baseRep(result,n,b);
      printf("%s\n",result);      //prints the converted value to output
      free(result);
  }
}


