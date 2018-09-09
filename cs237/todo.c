/*(c)Qianwen (Tiffany) Zheng
 * 10/15/17
 * This program manages a to-do list and has the functionalities of addings items, removing items, moving
 * items to different places, and printing out the list.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Writes back to the file from a modified array
 */
void writeList(char** list, int count)
{
  FILE *fp;
  fp = fopen(".todo","w");
  for(int i = 0; i<count; i++){
    fputs(list[i],fp);
  }
  fclose(fp);
}
int main(int argc, char* argv[])
{
  //Allocates the memory for the list
  char** list = (char**)malloc(2*sizeof(char*));
  list[0] = (char*)malloc(512);
  list[1] = (char*)malloc(512);
  int size = 2;
  int i = 0;
  int count = 0;

  //Opens the file for reading
  FILE *fptr;
  fptr = fopen(".todo","r");
  
  //Handle array allocation and size management
  while(fgets(list[i],512,fptr)!=NULL) {
    count++;
    if(count+1 == size){
      size*=2;
      list = (char**)realloc(list,size*sizeof(char*));
      for(int j=size/2; j<size; j++)
	list[j]= (char*)malloc(512);
    }
    i++;
  }
  fclose(fptr);

  //Determine the instruction specified by the user
  if(argc == 1){
    for(int i = 0; i < count; i++){
      printf("%d%s%s",i+1, ". ", list[i]);
    }
  } else if (argc == 2){
    char sign = argv[1][0];
    argv[1]++;
    int index = atoi(argv[1])-1;
    if(index < 0){
      printf("Not a valid number!\n");
      return -1;
    }

    //Removing
    if(sign == '-'){
      if(index < count){     //as long as the element exists in the list
	char* dump = list[index];
	  if(index==count-1)   //handles case where removing last element
	    list[index]=NULL;
	  else{
	    for(int i = index; i < count-1; i++)
	      list[i] = list[i+1];
	    list[count-1] = NULL;
	  }
	  count--;
	  writeList(list,count);   //writes the list back to the file
	  printf("%s",dump);       //prints out the removed item
      } else
	printf("%s%d%c\n", "No item ",index+1,'!');
    }

    //Adding
    else if (sign == '+'){
      char* item = NULL; 
      size_t s = 0;
      if(getline(&item,&s,stdin) > 1){   //Only adds non-empty items

	//Reads in the most current list (for moving purposes)
	FILE *currFile;
	currFile = fopen(".todo","r");
	count = 0;
	i = 0;
	while(fgets(list[i],512,currFile)!=NULL) {
	  count++;
	  i++;
	}
	fclose(currFile);
	
	int pos = (index>count)?0:index;
	for(int i = count; i > pos; i--)
	  list[i] = list[i-1];
	list[pos] = item;
	count++;
	writeList(list,count);     //writes the list back to the file
      }
    }
  }
  
  //free the allocated memory
  for(int i = 0; i < size; i++)
    free(list[i]);
  free(list);
  
}

 
  
  


  
