/*(c)Qianwen (Tiffany) Zheng                                                                                           
 * 11/17/17                                                                                                            
 * This program simulates a game of Solitaire Chess and prints the solution as a sequence of captures to output, one 
 * per line, after reading a sequence of n positioned pieces from input. It is played on a 4x4 board whose positions
 * are indicated by column (a to d) and row (1 to 4). It starts with n >= 1 chess pieces on the board. Rules are: pawns
 * capture diagonally forward, knights capture by jumping opposite on a 3x2 rectangle, bishops capture by sliding along
 * a diagonal, rooks capture by sliding along a row or column, queens capture like bishops or rooks and kings capture
 * any adjacent piece. This program finds a sequence of n-1 captures that leaves a lone piece. If there is a king among
 * the starting pieces, the king must not be captured.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Board {
  char pos[4][4];    //the 2-d array
  int numPieces;     //number of pieces on board
  char* moves[16];   //last move made on board
  int numMoves;      //number of moves made to achieve the board
}board;

//Get the moves for a bishop piece: THEY MOVE BY JUMPING NOT SLIDING
int getBishopMoves(int count, int r, int cl, char** capt, struct Board *original)
{
  char c;
  int size = 0;
  for(int i = 1; i <4; i++){
    //Conditions to be a valid space on the board
    int cond1 = r-i>-1;
    int cond2 = cl-i>-1;
    int cond3 = r+i<4;
    int cond4 = cl+i<4;

    if(cond1){
    //main diagonal going upwards
    c = original->pos[r-i][cl-i];
    if(c!=0&&cond2&&c!='k'){capt[count][0]=c; capt[count][1]=cl-i; capt[count][2]=r-i; size++;count++;}
    //minor diagonal going up
    c = original->pos[r-i][cl+i];
    if(c!=0&&cond4&&c!='k'){capt[count][0]=c; capt[count][1]=cl+i; capt[count][2]=r-i; size++;count++;}
    }

    if(cond3){
    //main diagonal going down
    c = original->pos[r+i][cl+i];
    if(c!=0&&cond4&&c!='k'){capt[count][0]=c; capt[count][1]=cl+i; capt[count][2]=r+i; size++;count++;}
    //minor diagonal going down
    c = original->pos[r+i][cl-i];
    if(c!=0&&cond2&&c!='k'){capt[count][0]=c; capt[count][1]=cl-i; capt[count][2]=r+i; size++;}
    }
  }
  return size;
}

//Get the moves for a rook piece: THEY MOVE BY JUMPING NOT SLIDING
int getRookMoves(int count, int r, int cl, char** capt, struct Board *original)
{
  char c;
  int size = 0;
  for(int i = 1; i <4; i++){
    //Conditions to be a valid space on the board
    int cond1 =r-i>-1;
    int cond2 =cl-i>-1;
    int cond3 =r+i<4;
    int cond4 =cl+i<4;

    //vertical going upwards
    c = original->pos[r-i][cl];
    if(c!=0&&cond1&&c!='k'){capt[count][0]=c; capt[count][1]=cl; capt[count][2]=r-i; size++;count++;}
    //vertical going down
    c = original->pos[r+i][cl];
    if(c!=0&&cond3&&c!='k'){capt[count][0]=c; capt[count][1]=cl; capt[count][2]=r+i; size++;count++;}

    //horizontal going left
    c = original->pos[r][cl-i];
    if(c!=0&&cond2&&c!='k'){capt[count][0]=c; capt[count][1]=cl-i; capt[count][2]=r; size++;count++;}
    //horizontal going right
    c = original->pos[r][cl+i];
    if(c!=0&&cond4&&c!='k'){capt[count][0]=c; capt[count][1]=cl+i; capt[count][2]=r; size++;}
  }
  return size;
}

/*
 * Fills up array of possible pieces to be captured by a given piece and returns the number of possible moves.
 * Accesses each possible captured piece directly from the 2-D array
 */
int getMoves(struct Board *original)
{
  int stop = 0;   //indicates when to end the search process for solutions
  
  //Allocate memory for the array
  char* capt[8];
  for(int i = 0; i<8; i++)
    capt[i] = strdup("pa3");
  
  //Go through every piece in the board
  for(int r = 0; r < 4; r++){     
    for(int cl = 0; cl < 4; cl++){
      char type = original->pos[r][cl];
      if (type != 0){
	char c;
	int count = 0;
	char piece[3];
	piece[0] = type;
	piece[1] = cl+'a';
	piece[2] =r+'1';
	
	//Conditions to check if valid space on the board
	int cond1 =r>0;
	int cond2 =cl>0;
	int cond3 = r<3;
	int cond4 =cl<3;
	
	if(type == 'p'&& cond1){            //pawn moves forward diagonally
	  c=original->pos[r-1][cl-1];
	  if(c!=0&&cond2&&c!='k'){capt[0][0]=c; capt[0][1]=cl-1; capt[0][2]=r-1; count++;}
	  c=original->pos[r-1][cl+1];
	  if(c!=0&&cond4&&c!='k'){capt[1][0]=c; capt[1][1]=cl+1; capt[1][2]=r-1; count++;}
	  
	} else if(type == 'n'){            //knights capture in L direction
	  int condi = cl<2;
	  int condj = r<2;
	  int condk = cl>1;
	  int condl = r>1;

	  if(condi){
	  //Rightwards L
	  c= original->pos[r-1][cl+2];
	  if(c!=0&&cond1&&c!='k'){capt[0][0]=c; capt[0][1]=cl+2; capt[0][2]=r-1; count++;}
	  c= original->pos[r+1][cl+2];
	  if(c!=0&&cond3&&c!='k'){capt[0][0]=c; capt[0][1]=cl+2; capt[0][2]=r+1; count++;}
	  }
	  if(condj){
	  //Bottom L
	  c =original->pos[r+2][cl+1];
	  if(c!=0&&cond4&&c!='k'){capt[3][0]=c; capt[3][1]=cl+1; capt[3][2]=r+2; count++;}
	  c=original->pos[r+2][cl-1];
	  if(c!=0&&cond2&&c!='k'){capt[2][0]=c; capt[2][1]=cl-1; capt[2][2]=r+2; count++;}
	  }
	  if(condl){
	  //Top L
	  c =original->pos[r-2][cl+1];
	  if(c!=0&&cond4&&c!='k'){capt[3][0]=c; capt[3][1]=cl+1; capt[3][2]=r-2; count++;}
	  c=original->pos[r-2][cl-1];
	  if(c!=0&&cond2&&c!='k'){capt[2][0]=c; capt[2][1]=cl-1; capt[2][2]=r-2; count++;}
	  }
	  if(condk){
	  //Leftwards L
	  c =original->pos[r-1][cl-2];
	  if(c!=0&&cond1&&c!='k'){capt[1][0]=c; capt[1][1]=cl-2; capt[1][2]=r-1; count++;}
	  c =original->pos[r+1][cl-2];
	  if(c!=0&&cond3&&c!='k'){capt[1][0]=c; capt[1][1]=cl-2; capt[1][2]=r+1; count++;}
	  }
	  
	} else if(type == 'b'){      //bishops capture first piece encountered on either side of each relevant diagonal
	  count = getBishopMoves(count,r,cl,capt,original);
	  
	} else if (type == 'r'){   //rooks capture first piece encountered on either side of the vertical and horizontal
	  count = getRookMoves(count,r,cl,capt,original);
	  
	} else if (type == 'q'){    //queens capture like rooks and bishops
	  int bishCount = getBishopMoves(count,r,cl,capt,original);
	  int rookCount = getRookMoves(rookCount,r,cl,capt,original);
	  count = bishCount + rookCount;
	  
	} else if (type == 'k'){
	  for(int i = -1; i <2; i++){
	    //top adjacent squares
	    if(cl+i>-1 && cl+i<4){
	      c = original->pos[r-1][cl+i];
	      if(c!=0&&cond1&&c!='k'){capt[count][0]=c; capt[count][1]=cl+i; capt[count][2]=r-1; count++;}
	      //bottom adjacent squares
	      c = original->pos[r+1][cl+i];
	      if(c!=0&&cond3&&c!='k'){capt[count][0]=c; capt[count][1]=cl+i; capt[count][2]=r+1; count++;}
	    }
	  }
	  c = original->pos[r][cl+1];  //piece to the right
	  if(c!=0&&cond4&&c!='k'){capt[count][0]=c; capt[count][1]=cl+1; capt[count][2]=r; count++;}
	  c = original->pos[r][cl-1];  //piece to the left
	  if(c!=0&&cond2&&c!='k'){capt[count][0]=c; capt[count][1]=cl-1; capt[count][2]=r; count++;} 
	}
	
	if(count!=0){
	  //For each captured piece in the array, get the new board state
	  for(int i = 0; i < count; i ++){
	    struct Board new[1];   
	    new->numPieces = original->numPieces - 1;
	    new->numMoves = original->numMoves +1;
	    //allocate the new moves array
	    for(int j=0; j<16; j++)   
	      new->moves[j] = original->moves[j];
	    char captCol = capt[i][1] + 97;  //gets character from integer value of column
	    char captRow = capt[i][2] + 49;  //gets character from integer value of row
	    char captured[3] = {capt[i][0],captCol,captRow};
	    sprintf(new->moves[new->numMoves-1],"%s captures %s", piece, captured);
	    //allocate the new position array
	    for(int j=0; j<4; j++){  
	      for(int k=0; k<4; k++)
		new->pos[j][k] = original->pos[j][k];
	    }
	    new->pos[r][cl] = 0;
	    int row = capt[i][2];
	    int col = capt[i][1];
	    new->pos[row][col] = piece[0];
	    
	    if(new->numPieces ==1){
	      //Print out the first sequence of moves to achieve winning state
	      for(int i = 0; i <new->numMoves; i++)
		printf("%s\n",new->moves[i]);
	      stop=1;
	    } else 
	      stop = getMoves(new);
	    if(stop)break;
	  }
	}	
      }
      if(stop)break;
    }
  if(stop)break;
  }
  return stop;
}

int main(int argc, char* argv[])
{
  //Get the pieces from input
  int count = 0;     //number of pieces on the board
  struct Board b[1]; 
  char piece[3];     //current piece being read in
  while(scanf("%s",piece)>0){
    b->pos[piece[2]-49][piece[1]-97]= piece[0];
    count++;
  }
  b->numPieces = count;
  b->numMoves = 0;
  for(int i = 0; i<16; i++)
    b->moves[i] = (char*)malloc(18);
  if(!getMoves(b)) printf("No valid solution found!\n");
  for(int i = 0; i<16; i++)
    free(b->moves[i]);
}
