#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myio.h"

/* gets two command-line arguments: input_file.txt output_file.txt.
 * reads each line and removes the extra space characters between the words and
 * prints the new line into output_file.txt.
 * Remove all the space charecters from the begining and end of the string. Leave a space character between words.
 * copy all characters that are not white space and spit it out in normal format
 */
int main(int argc, char *argv[]) {
   FILE *inputFile, *outputFile;
   char *inputLine, *outputLine;
   char temp;
   int i , j, loop;
   
   //Error check arguement count
   if(argc != 3)
   {
	   printf("USAGE: %s <input_file> <ouput_file>\n", argv[0]);
	   return 1;
   }
   
   //open input file for reading R
   inputFile = fopen(argv[1], "r");
   if(inputFile == NULL)
   {
      printf("ERROR: input file %s could not be opened\n", argv[1]);
      return 1;
   }
  
  //open output file for writing W
  outputFile = fopen (argv[2], "w");
  if(outputFile == NULL)
   {
      printf("ERROR: output file %s could not be opened\n", argv[2]);
      return 1;
   }
   
   //read in file line by line
   while((inputLine = ReadLineFile(inputFile)) != NULL)
   {
      i = 0;
      j = 0;
      loop = 0;
	  //making room in outputline 
      outputLine = calloc(strlen(inputLine), sizeof(char));
      
      while(!loop)
      {
		 //Parse through each char in input line
         temp = inputLine[i++];
         //Grab all characters that AREN'T whitespace
         while(temp != ' ' && temp != '\t'&& temp != '	')
         {
            if(temp == '\0')
            {
               loop = 1;
			   break;
            }
				  //move through outputLine
                  outputLine[j++] = temp;
				  //temp = nextChar
				  temp = inputLine[i++];
         }
		 if(!loop)
         {
            //send a single whitespace added to output
            outputLine[j++] = ' ';
            //deal with multiple whitespace gaps!
            while(inputLine[i] == ' ' || inputLine[i] == '\t')
                  i++;
         }
		 else
		 {
			 outputLine[j] = '\0';
			 //Print/Send the outputLine ->into outputFile
			 fprintf(outputFile, "%s\n", outputLine);
         }
      }
	  //free all allocated space
	  free(inputLine);
	  free(outputLine);
   }
  
fclose(inputFile);
fclose(outputFile);
  
printf("Generated output file %s\n", argv[2]);
}