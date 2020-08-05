#include <stdio.h>

/* Displays an unsigned integer in bits. Print a space after every 8
bits. */
//& 0x01 is anding the value of n with 1.
void displayBits(unsigned int n)
{
   int count = 0;
   int bit;
   for(bit=0;bit<(sizeof(unsigned int) * 8); bit++)
   {
      //starting from end and shifting left.
      printf("%i", n & 1<<31 ? 1 : 0);
      n = n << 1;
      count++;
      if (count == 8)
      {
          printf(" ");
          count = 0;
      }
   }
}

/* Sets the kth bit of n from the right to 1*/
unsigned int setKthBit(unsigned int n, int k)
{
   int bit,total,x,count;
   for(bit=0;bit<(sizeof(unsigned int) * 8); bit++)
   {
      if (total == (31 - k)){
          //And operator clears bit not needed but just in case
          n &= ~(1 << x);
          //Xor operator toggles bit to 0 or 1
          n ^= 1 << x;
      }
      printf("%i", n & 1<<31 ? 1 : 0);
      n = n << 1;
      count++;
      if (count == 8){
          printf(" ");
          count = 0;
      }
      total++;
}
}

/*returns 1 if n is a power of 2 and 0 otherwise.*/
int isPowerOfTwo(unsigned int n)
{
   //takes n and subtracts 1 from it, will return 0 if a power of 2, and returns n-1 otherwise.
   if((n & (n - 1)) == 0){
        return 1;
    } else 
       return 0;
}

/* returns the number of bits required to convert a to b. */
int getNoBits(unsigned int a,unsigned int b)
{
   unsigned int diff = sizeof(unsigned int);
    diff = a^b; //Xor compare the two.
    int count = 0; //then count the times xor sets.
    while (diff != 0)
    {
        count++;
        diff &= diff - 1;
    }
    return count;
}

/* computes the XOR of all the numbers from 1 to n (including n). */
unsigned int computeXor(unsigned int n)
{
   unsigned int a = sizeof(unsigned int);
   unsigned int b = sizeof(unsigned int);
   unsigned int temp = sizeof(unsigned int);
   int i;
   for(i = 1; i <= n - 1; i++){
       a = i;
       b = i+1;
       temp = a^b;
       printf("%i\n",temp);
   }
}



























