#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "myio.h"

/*gets three command-line arguments: x y z. 
 *asks user to enter x many integers, y many doubles, and z many lines
 *program prints the largest integer, the largest double, and the longest line
 */

bool legalInt(char *str)
{
    while (str != 0)
    {
        if (!isdigit(*str)) 
        {
           return false;
        }
        str++;
    }
    return true;
}

int main(int argc, char *argv[]) {
   int doubles, i, integers, maxInt, lines, maxLine;
   char *longLine;
   double maxDoub;
   FILE *output;
   int w, x, y, z;

   if (argc != 5) {
      printf("USAGE is x y z File_Name.out\n");
      return 0;
   }
   
   //Check all arguements for validity
   
   //Check and initialize integers
   if (legalInt(argv[1]))
   {
      return 0;
   }
   sscanf(argv[1], "%d", &integers);
   
   //Check and initialize doubles
   if (legalInt(argv[2]))
   {
      return 0;
   }
   sscanf(argv[2], "%d", &doubles);
   
   //Check and initialize lines
   if (legalInt(argv[3]))
   {
      return 0;
   }
   sscanf(argv[3], "%d", &lines);
   
   output = fopen(argv[4], "a");
   if (output == NULL) {
      perror("File could not be opened");
      return 0;
   }
   
   //Save highest int!
   maxInt = -9999;
   for (x = 0; x < integers; x++) {
      int n = ReadInteger();
      printf("%d\n", n);
      fprintf(output, "%d\n", n);
	  //compare longest digit
	  if(maxInt < n ){
         maxInt = n;
	  }
   }
   
   //Save highest double!
   maxDoub = -9999.0;
   for (y = 0; y < doubles; y++) {
      double d = ReadDouble();
      printf("%lf\n", d);
      fprintf(output, "%lf\n", d);
	  //Compare largest double
	  if(maxDoub < d ){
         maxDoub = d;
	  }
   }

   //Save longest line!
   maxLine = 0;
   for (z = 0; z < lines; z++) {
	  printf("Enter a line of characters : ");
      char *l = ReadLine();
	  //Compare longest line length
	  if(strlen(l) > maxLine)
	  {
		longLine = l;
	  }
      printf("%s\n", l);
      fprintf(output, "%s\n", l);
   }
   
   //done writing to the output file ! close!
   fclose(output);
   
   //printing the largest value from each category!
   printf("Largest int : %d \n",maxInt);
   printf("Largest double : %f \n",maxDoub);
   printf("Longest Line : %s \n",longLine);

   return 0;
}

