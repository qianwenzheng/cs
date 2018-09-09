/*(c)Qianwen (Tiffany) Zheng
 * This program reads input and writes to the output strings of letters (word)
 * that appear in the input, with each word on its own line. It assumes that a 
 * "word" refers to any string of letters separated by other characters.
 * 9/17/17
 */
#include <stdio.h>
#include <ctype.h>

int main()
{
  //current character read in from input
  int ch = fgetc(stdin);

  //the previous character read from input, initially set to current char
  int chPrev = ch; 

  //reads in input until the end of file
  while(EOF != ch) 
    {
      //prints character to output if it is alphabetic
      if(isalpha(ch) != 0)  
	fputc(ch,stdout);
      
      //Handles multiple non-alphabetic characters; adds a single new line
      //in between each printed word if they have one/more non-alphabetic
      //characters between them
      else if(isalpha(chPrev) != 0)
	fputc('\n',stdout);  
      chPrev = ch;
      ch = fgetc(stdin);
    }
}
