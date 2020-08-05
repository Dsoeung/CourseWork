/**********************************************************************
p4iou957.c By Darin Soeung
Purpose:
    This program reads book information and a command file.   It 
    processes the commands against the book information.
    This is the driver code for the routines written by the students.
Command Parameters:
    p3 -b bookFileName -c commandFileName
Input:
    Book	same as Programming Assignment #2 although there is different data.

    Command	This is different from the previous assignment.  The file 
            contains  text in the form of commands.  
        CUSTOMER BEGIN szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle  
            specifies the beginning of customer request and includes all the 
            identification information from program 2.
        CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
            specifies the address for a customer (separated by commas)
        CUSTOMER TRANS cTransType    szBookId   szTransDt
            specifies a single book transaction.  Steps:
            -	Print the Transaction Type, Book Id, and Transaction Date 
            -	Lookup the book ID using a binary search.  If not found, print a 
                warning (but do not terminate your program) and return.
            -	If the transaction date is invalid, show a message stating "invalid date", 
                but do not terminate.  Use the validateDate  function.
            -	If the transaction type is C (meaning checkout):
                o	If the book is already checked out, show a message stating 
                    "already checked out", but do not terminate.
                o	Otherwise, check out the book to this customer, setting the book's 
                    customer ID.  The book's checked out date needs to be set to the 
                    transaction's date.
            -	If the transaction type is R (meaning return):
                o	Use dateDiff to subtract the book's szCheckOutDt from the transaction's szTransDt
                o	If the difference is more than 14:
                    --	Determine the number of days late by subtracting 14 from that difference.  
                    --	Compute the late fee by multiplying the number of days late 
                        by the book's dLateFeePerDay.
                    --	If that late fee is greater than the book's dMaxLateFee, 
                        change it to dMaxLateFee.
                    --  Print the late fee.
                    --	Add the computed late fee to the customer's dFeeBalance
                o	Set the book's customer ID to "NONE".
                o	Set the book's check out date to "0000-00-00".
        CUSTOMER COMPLETE
            specifies the completion   of a customer.  Print the total fees for this customer.
        BOOK CHANGE szBookId dLateFeePerDay dMaxLateFee
            change the Late Fee Per Day and Max Late Fee to the specified values
        BOOK SHOW szBookId    
            requests a display of a particular book.  Show all of its information.
               
Results:
    Prints the Books prior to sorting
    Prints the Books after sorting.
    Processes the commands (see above) and shows any errors.
    Prints the resulting Books
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information

Notes:
    p3 -?       will provide the usage information.  In some shells,
                you will have to type p3 -\?

**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p4.h"
void sortBooks(Book bookM[], int iBookCnt)
{
    int i=0;
    int j=0;
    int k;
    char first, second;
    Book temp;


    for(i=0; i< iBookCnt; i++)
    {
         for(j=0; j< iBookCnt-1; j++)
         {
              k=0;
              first= bookM[j].szBookId[k];
              second= bookM[j+1].szBookId[k];
              while(first==second)
              {
                 k++;
                 first= bookM[j].szBookId[k];
                 second= bookM[j+1].szBookId[k];
              }
              if(first>second)
              {
                  temp= bookM[j];
                  bookM[j]= bookM[j+1];
                  bookM[j+1]= temp;
              }
         }
         j=0;
    }
}
/******************** printBooks **************************************
    void printBooks(char *pszHeading, Book bookM[], int iBookCnt)
Purpose:
    Prints each book in a table format
Parameters:
    I char *pszHeading    Heading to print before printing the table of books
    I Book bookM[]        Array of books
    I int iBookCnt        Number of elements in the bookM array.
Returns:
    n/a
Notes:
    
**************************************************************************/
void printBooks(char *pszHeading, Node *pHead)
{
	Node *i;
	int g;
    printf("\n%s\n", pszHeading);

    // output column 
    printf("   %-9s %-40s %-8s %-10s %-8s %-9s \n"
        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");
        
    // output each book 
    for (i = pHead; i != NULL; i=i->pNext )
    {
        printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
            , i->book.szBookId				//bookM[i].szBookId
            , i->book.szTitle				//bookM[i].szTitle
            , i->book.szCustomerId			//bookM[i].szCustomerId
            , i->book.szCheckedOutDt		//bookM[i].szCheckedOutDt
            , i->book.dLateFeePerDay		//bookM[i].dLateFeePerDay
            , i->book.dMaxLateFee			//bookM[i].dMaxLateFee
            );
    }
}


/********************processCustomerCommand *****************************
    void processCustomerCommand(Book bookM[], int iBookCnt
     , char *pszSubCommand, char *pszRemainingInput
     , Customer *pCustomer)
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
        CUSTOMER BEGIN szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle  
            specifies the beginning of customer request and includes all the identification information from program 2.
        CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
            specifies the address for a customer (separated by commas)
        CUSTOMER TRANS cTransType    szBookId   szTransDt
            specifies a single book transaction.  Steps:
            -	Print the Transaction Type, Book Id, and Transaction Date 
            -	Lookup the book ID using a binary search.  If not found, print a warning (but do not terminate your program) and return.
            -	If the transaction date is invalid, show a message stating "invalid date", but do not terminate.  Use the validateDate  function.
            -	If the transaction type is C (meaning checkout):
                o	If the book is already checked out, show a message stating "already checked out", but do not terminate.
                o	Otherwise, check out the book to this customer, setting the book's customer ID.  The book's checked out date needs to be set to the transaction's date.
            -	If the transaction type is R (meaning return):
                o	Use dateDiff to subtract the book's szCheckOutDt from the transaction's szTransDt
                o	If the difference is more than 14:
                    --	Determine the number of days late by subtracting 14 from that difference.  
                    --	Compute the late fee by multiplying the number of days late by the book's dLateFeePerDay.
                    --	If that late fee is greater than the book's dMaxLateFee, change it to dMaxLateFee.
                    --  Print the late fee.
                    --	Add the computed late fee to the customer's dFeeBalance
                o	Set the book's customer ID to "NONE".
                o	Set the book's check out date to "0000-00-00".
        CUSTOMER COMPLETE
            specifies the completion   of a customer.  Print the total fees for this customer.


Parameters:
    I/O Book bookM[]                  Array of books
    I   int   iBookCnt                Number of elements in bookM[]
    I   char  *pszSubCommand          Should be BEGIN, ADDRESS, REQUEST or COMPLETE
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
    I/O Customer *pCustomer           The BEGIN subcommand begins a new customer.  
Notes:commented out print statements used to see flow of the method

**************************************************************************/
void processCustomerCommand(Node **ppHead, char *pszSubCommand,
                                    char *pszRemainingInput, Customer *pCustomer)
{
    int iscanfCnt;
    Transaction transaction;
    // Determine what to do based on the subCommand
   // printf("%s", pszSubCommand);
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
        // get the Customer Identification Information
        iscanfCnt = sscanf(pszRemainingInput, "%s %lf %s %s\n", pCustomer->szCustomerId,
        &pCustomer->dFeeBalance, pCustomer->szEmailAddr, pCustomer->szFullName);

        // Check for bad customer identification data
        if (iscanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, pszRemainingInput);
    }
    else if (strcmp(pszSubCommand, "COMPLETE") == 0)
    {
        // print the customer's total fees
       printf("\t\t\t\t\t\t\t Customer's Total Fees: $%.2lf\n", pCustomer->dFeeBalance);

    }
    else if (strcmp(pszSubCommand, "ADDRESS") == 0)
    {
        // get the postal address and print it
        iscanfCnt = sscanf(pszRemainingInput, "%[^,], %[^,], %[^,] %s\n", pCustomer->szStreetAddress,
        pCustomer->szCity, pCustomer->szEmailAddr, pCustomer->szZipCd);
       // printf("no");
       // printf("iscanfCnt = %d", iscanfCnt);
       
       if(iscanfCnt != 4)
       {
             exitError(ERR_CUSTOMER_ADDRESS_DATA, pszRemainingInput);
       }
           

        printf("\n**********************Library Reciept**********************\n");

        printf("%s %s %s %s %.2lf \n", pCustomer->szCustomerId, pCustomer->szEmailAddr, pCustomer->szFullName,
        "Previously Owed:", pCustomer->dFeeBalance);

        printf("%s %s %s %s\n", pCustomer->szStreetAddress, pCustomer->szCity, pCustomer->szEmailAddr, pCustomer->
        szZipCd);

        // print the column heading for the transactions
        printf("\t\t\t\t\t%-5s  %-9s  %-10s\n", "Trans", "Book", "Date");

    }
    else if (strcmp(pszSubCommand, "TRANS") == 0)
    {
    
        //Tranaction of the customer
        //fgets(pszRemainingInput,MAX_LINESIZE,pFileCust);
        iscanfCnt = sscanf(pszRemainingInput, "%c %s %s" , &transaction.cTransType, transaction.szBookId, transaction.szTransDt);
        if(iscanfCnt < 3)
        {
            //Error Handeling
             exitError(ERR_TRANSACTION_DATA, pszRemainingInput);
        }
        printf("\t\t\t\t\t %c %s %s", transaction.cTransType, transaction.szBookId, transaction.szTransDt);
        pCustomer->dFeeBalance += processTransaction(ppHead, *pCustomer, transaction);
        
        printf("\n");
    }
    else printf("    *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}

//old transaction flow used in lab2
   /* while(transaction.cTransType!='X'){
    printf("%c %s %s \n", transaction.cTransType, transaction.szBookId,
    transaction.szTransDt);

    //fgets(pszRemainingInput,MAX_LINESIZE,pFileCust);
    sscanf(pszRemainingInput, "%c %s %s", &transaction.cTransType, transaction.szBookId,
    transaction.szTransDt);&*/


/********************processBookCommand *****************************
    void processBookCommand(Book bookM[], int iBookCnt
         , char *pszSubCommand, char *pszRemainingInput)
Purpose:Processes the subcommands associated with the BOOK command:
        BOOK CHANGE szBookId dLateFeePerDay dMaxLateFee
            change the Late Fee Per Day and Max Late Fee to the specified values
        BOOK SHOW szBookId    
            requests a display of a particular book.  Show all of its information.
                

Parameters:
    I/O Book bookM[]                  Array of books
    I   int   iBookCnt                Number of elements in bookM[]
    I   char  *pszSubCommand          Should be CHANGE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processBookCommand(Node**ppHead, char *pszSubCommand, char *pszRemainingInput)
{
	Node**pPrecedes;
	Node* Li = *ppHead;
	Book book;
	Node* p = searchLL(*ppHead, book.szBookId, pPrecedes);		
    int iScanfCnt;
    int i;
    // Determine what to do based on the subCommand
    // your code
    //iScanfCnt = sscanf(pszRemainingInput, "%9s %lf %lf", p->book.szBookId,
    //p->book.dLateFeePerDay, p->book.dMaxLateFee);
    //i = searchBooks(bookM, iBookCnt, book.szBookId);

    if(strcmp(pszSubCommand, "CHANGE") == 0)
    {
        if(p != NULL)
        {
            p->book.dLateFeePerDay = book.dLateFeePerDay;
            p->book.dMaxLateFee = book.dMaxLateFee;
        }
    }else if(strcmp(pszSubCommand, "SHOW") == 0){
        if(p != NULL)
        {
            printf("Show Book:\n");
            printf(" %-9s %40s %-8s %-10s %-8s %-8s \n", "Book id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

            printf(" %-9s %-40s %-8s %-10s %-8.2lf %-8.2lf \n",  p->book.szBookId, p->book.szTitle, p->book.szCustomerId,
            p->book.szCheckedOutDt, p->book.dLateFeePerDay, p->book.dMaxLateFee);
        }else if(strcmp(pszSubCommand, "BOOK NEW") == 0){
			//scan data/elements of book then pass through insertLL
			insertLL(ppHead,p->book);
		}
   }

}
/******************** searchBooks *****************************
    int searchBooks(Book bookM[], int iBookCnt, char *pszMatchBookId)
Purpose:
    Using a binary search, it finds the specified book in the booKM array.
Parameters:
    I   Book bookM[]                 Array of books
    I   int   iBookCnt               Number of elements in bookM[]
    I   char *pszMatchBookId         Book Id to find in the array
Returns:
    >= 0   subscript of where the match value was found
    -1     not found
Notes: commented out parts for running test on checking what went throught 

**************************************************************************/
/*int searchBooks(Node *ppHead, int iBookCnt, char *pszMatchBookId)
{
    // your code
   int upper;
   int lower;
   int middle;

   lower=0;
   upper= iBookCnt-1;
   
   //compares upper bound to lower
   while(upper>=lower)
   {
       middle = (lower + upper)/2;
       if(strcmp(pszMatchBookId, bookM[middle].szBookId) == 0)
           return middle;
       if(strcmp(pszMatchBookId, bookM[middle].szBookId) < 0)
           upper = middle -1;
       else
           lower = middle +1;
   }
   return -1;
} */

//SearchLL for linked list

Node *searchLL(Node *pHead, char szMatchBookId[], Node**pPrecedes)
{
    // your code
   Node *p;
   
   
   for (p = pHead; p!=NULL; p = p->pNext)
   {
	   if (strcmp(p->book.szBookId,szMatchBookId)==0)
	   return p;
   if (strcmp(p->book.szBookId,szMatchBookId)<0){
	   return NULL;
	   *pPrecedes = p;
   }
   if (p==NULL)
   {
   *pPrecedes = NULL;
   return NULL;
   }
}
}


/*** include your processTransaction code from program #2 ***/
    
double processTransaction(Node **ppHead, Customer customer, Transaction transaction){
	Node **pPrecedes;	
    Node* i =  searchLL(*ppHead, transaction.szBookId, pPrecedes);
    double lateFee;
    //If cannot find book searchLL will return NULL. 
        if(i == NULL)
            printf ("\tBook not Found"); 
    //based on the character 'C' or 'R' the program would follow the right cutsomer that is read
    else{
        if(transaction.cTransType == 'C')
        {       
           if ((strcmp(customer.szCustomerId, "NONE")) != 0) 
                printf("  *** Already Checked Out\n");
          else
          {
              sscanf(customer.szCustomerId, "%7s",i->book.szBookId); 
              sscanf(transaction.szTransDt, "%11s", i->book.szCheckedOutDt);
          }
        }  
        if(transaction.cTransType == 'R')
        {
            if(dateDiff(transaction.szTransDt, (i->book.szCheckedOutDt)) > 14)
            {
               // printf("%s", bookM[i].szCheckedOutDt);         
                int difference = dateDiff(transaction.szTransDt, i->book.szCheckedOutDt);
               //printf("&s", bookm[i].szCheckedOutDt);
                int numDays = (difference - 14);
                 lateFee = (numDays * i->book.dLateFeePerDay);

                if(lateFee > i->book.dMaxLateFee)
                {
                   lateFee = i->book.dMaxLateFee;
				   
                  customer.dFeeBalance = (customer.dFeeBalance + lateFee);   
                }
                    printf("\tLate Fee = %.2lf", lateFee);
            } 
                sscanf("NONE", "%7s", i->book.szCustomerId);
                sscanf("0000-00-00", "%11s", i->book.szCheckedOutDt);
        }                   
         }
         
         return lateFee;
}

Node *insertLL(Node **ppHead, Book book){
	Node *ptr, *ptr2;
	Node **pPrecedes;
	Node* new = searchLL(*ppHead, book.szBookId, pPrecedes);
	if(new = NULL){
	ptr = (Node*)malloc(sizeof(Node));
	ptr->book = book;
	ptr->pNext = NULL;
	if(*ppHead==NULL){
		*ppHead = ptr;
		return;
	}
	ptr2=*ppHead;
	while(ptr2->pNext!=NULL){
		ptr2=ptr2->pNext;
		ptr2->pNext=ptr;
	}
	}
}





















