/*(c)Qianwen (Tiffany) Zheng
 * 10/15/17
 * This program computes the Scrabble score of each word on the input, calculated using the
 * sum of the point values of letters that appear in a word. The score is zero if any word
 * contains a non-lowercase letter or if the word exceeds the count of characters available.
 * For invalid inputs like these, the count of characters is restored to the previous count 
 * before the invalid word. It outputs the score and the word on a single line for each word
 *  with a non-zero score. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
  //store the point values and count of letters, including BLANKs
  int points[27] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,8,4,10,0};
  int counts[27] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1,2};
  
  //temportary array to modify during point value allocation
  int tempC[27];
  
  //word and score
  char* s = (char*)malloc(32);
  int score;

  int i = 0;

  while(scanf("%s",s)!=EOF){
    score = 0;                                     //sets score=0 before reading each word
    memcpy(tempC,counts,sizeof(counts));           //sets the number of current counts before each word is checked
    while(s[i] != 0){         
      if(!(s[i]<123 && s[i]>96)){                  //contains an non-lowercase letter
	memcpy(counts,tempC,sizeof(counts));       //restore the counts to what they were before the invalid word was read
	score = 0;                                 //sets score to zero and stops reading the word
	break;
      }

      if(counts[s[i]-97]!= 0) {
	counts[s[i]-97]-= 1;                       //decrement the count of specific char by 1
	score += points[s[i]-97];                  //score is incremented by the point value
      }
      else {
	if (counts[26]!= 0)                        //use BLANK if there are any available
	  counts[26]-= 1;                          //decrement the count of BLANKs by 1
	else {
	  memcpy(counts,tempC,sizeof(counts));     //restore the counts to what they were before the invalid word was read
	  score = 0;                               //word exceeds counts so set score to zero and stop reading the word
	  break;                                
	}
      }
      i++;
    }
    if(score != 0)
      printf("%s%s%d%s", s, " ",  score, "\n");    //prints word and non-zero score 
  }
}
