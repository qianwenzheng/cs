/* (c) Qianwen (Tiffany) Zheng
 * This program reads input and outputs the width (in characters) and the
 * height (in lines) of the input.The height is the number of lines in the 
 * input and the width is the number of characters in the longest line. We
 * assume that each line ends with a newline character. The width only counts 
 * printable characters. Therefore, newlines and other control characters are 
 * not included in the width. 
 * 9/17/17
 */
#include <stdio.h>
#include <ctype.h>

int main()
{
  int prevWidth = 0;      //the most recent width calculated
  int width = 0;          //the highest current width read from input
  int height = 0;         //the number of lines in the input
  int ch = fgetc(stdin);  //the current character being read in from input

  // reads in input until the end of file
  while(EOF != ch)
    {
      //Increments width by one if ch is printable
      if(isprint(ch) != 0) 
	  prevWidth++;
      //Calculates new width if ch is a tab character
      else if(ch=='\t')
	  prevWidth = (prevWidth/8+1)*8;
      //Increases height on each newline and updates width
      else if (ch == '\n')
	{
	  height++;
	  if(prevWidth>width)
	    width=prevWidth;
	  prevWidth = 0;	  
	}
      ch = fgetc(stdin);
    }

  // Prints the width and height of the input
  printf("Width = %d Height = %d \n", width, height);
}
