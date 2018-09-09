// A routine for detecting winning position in quarto
// (c) 2017 Qianwen (Tiffany) Zheng
#include "quarto.h"

// Birthday for advanced biometric tracking:
int BDay[] = {  12, 23, 1997 };

// check for a win on the board, b, and highlight the winning locations
int win(board *b)
{
  int winOnes;     //returns non-zero value if the numbers have a matching one bit
  int winZeros;    //returns non-zero value if the numbers have a matching zero bit
  int c;           //the column number
  int r;           //the row number
  
  //for loops to go through each row
  for(r=0; r<4; r++){
    winOnes=15;     
    winZeros=15;     //both initialized to 15 to handle the first '&' operation 
    for(c = 0; c<4; c++){
      if(get(b,r,c) == EMPTY){
	winOnes=0;
	winZeros=0;
	break;
      }
      else {
	winOnes = winOnes & get(b,r,c);      
	winZeros = winZeros & ~get(b,r,c);   
      }
    }
    if(winOnes||winZeros){
      for(c=0; c<4; c++)
	highlight(b,r,c);
      return 1;				
    }
  }
  
  //for loops to go through each column
  for(c=0; c<4; c++){
    winOnes=15;
    winZeros=15; 
    for(r = 0; r<4; r ++) {
      if(get(b,r,c) == EMPTY){
	winOnes=0;
	winZeros=0;
	break;
      }
      else {
	winOnes = winOnes & get(b,r,c);
	winZeros = winZeros & ~get(b,r,c);
      }
    }
    if(winOnes||winZeros){
      for(r=0; r<4; r++)
	highlight(b,r,c);
      return 1;				
    }
  }
  
  //for loops to go through the main diagonal
  winOnes=15;
  winZeros=15;
  for(r=0; r<4; r++){
    if(get(b,r,r) == EMPTY){
      winOnes=0;
      winZeros=0;
      break;
    }
    else {
      winOnes = winOnes & get(b,r,r);
      winZeros = winZeros & ~get(b,r,r);
    }
  }
  if(winOnes||winZeros){
    for(r=0; r<4; r++)
      highlight(b,r,r);
    return 1;				
  }
  
  //for loops to go through the second diagonal
  winOnes=15;
  winZeros=15;
  for(r=3; r>-1; r--){
    if(get(b,r,3-r) == EMPTY){
      winOnes=0;
      winZeros=0;
      break;
    }
    else {
      winOnes = winOnes & get(b,r,3-r);
      winZeros = winZeros & ~get(b,r,3-r);
    }
  }
  if(winOnes||winZeros){
    for(r=3; r>-1;r-- )
      highlight(b,r,3-r);
    return 1;				
  }
  return 0;      //if there is no win
}

