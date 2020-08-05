#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"

int ReadInteger(void){
      char *input;
      int final,loop, i;
      loop = 0; //initialize do-while loop
         do {
      loop = 0;
	  printf("Input a sequence of integers : ");
      input = ReadLine(); //put stdin into input
      for (i = 0; i < strlen(input); i++) {
         if (i == 0&& input[i] == '-'||i == 0&& input[i] == '+')
            continue;
         if (input[i] == 0)
            break;
         else if (input[i] > '9' || input[i] < '0') { //straight comparison between char and literals
            loop = 1;
            printf("ERROR invalid sequence of characters : %s \n",input);
            printf("Input a sequence of integers : \n");
            break;
         }
     }
   } while (loop == 1);
   sscanf(input, "%d", &final);
   free(input);
   return final;
   }
   

double ReadDouble(){
   int dot, loop;
   char *input;
   double num;
   int i;
   do {
      dot = 0;
      loop = 0;
      printf("Input a double : ");
      input = ReadLine();
      for (i = 0; i < strlen(input); i++) {
         if (i == 0 && input[i] == '-'||i == 0 && input[i] == '+') //check for positive minus sign
            continue;
         if (input[i] == 0)
            break;
         else if (input[i] == '.' && dot == 0) //check if dot is present in number
            dot = 1;
         else if (input[i] > '9' || input[i] < '0') { //check if digit
            loop = 1;
            printf("ERROR: INVALID INPUT : \n");
            break;
         }
      }
   } while (loop == 1);
   num = strtod(input, NULL);
   free(input);
   return num;
   }


char *ReadLine(void){
   //printf("Enter a line of characters : \n");
   return(ReadLineFile(stdin));
}


char *ReadLineFile(FILE *infile){
	//initialize variables
   char* buffer;
   int currChar;
   int i = 0;
   size_t buff_len = 0;
   buffer = malloc(buff_len + 1);
   if (!buffer) return NULL;  // Out of memory
   while ((currChar = fgetc(infile)) != '\n' && currChar != EOF)
   {
      buff_len++;
      char *tmp = realloc(buffer, buff_len + 1);
      if (tmp == NULL) 
      {
         free(buffer);
         return NULL; // Out of memory
      }
         buffer = tmp;
         buffer[i] = (char) currChar;
         i++;
   }
         buffer[i] = '\0';
  // Check EOF
   if (currChar == EOF && (i == 0 || ferror(infile))) 
   {
      free(buffer);
      return NULL;
   }
	  return buffer;
}













