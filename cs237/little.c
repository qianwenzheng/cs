/*(c)Qianwen (Tiffany) Zheng                                                            
 * 11/17/17                                                                                          
 * This program exits with status 0 if it is running on a little-endian machine and 1 otherwise if the executable
 * is named "little". If the executable is named "big" it exits with status 0 if it is running on a big-endian machine
 * and 1 otherwise.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int i = 1;  //at lowest storage address 1 (little-endian) or 0 (big-endian) will be stored

  //exit status depends on the name of the executable: "little" or "big"
  exit((strcmp("little",argv[0])!=0) == *((char*)&i));  //char will be 1 if little endian, 0 if big endian
}
