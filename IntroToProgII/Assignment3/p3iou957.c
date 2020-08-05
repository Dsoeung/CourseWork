/**********************************************************************
p3iou957.c by Darin Soeung
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
#include "cs1713p3.h"

/******************** getBooks **************************************
    int getBooks(Book bookM[], char * pszBookFileName)
Purpose:
    Retrieves the books from the file specified..
Parameters:
    O   Book bookM[]          an array which is used for returning the books.
    I   char *pszBookFileName name of the file containing the book data
Returns:
    A count of the number of read.
Notes:
    1. This opens the file based on the specified file name. 
    2. The data in the data file is in this format
    szBookId szCustomerId szCheckedOutDt dLateFeePerDay dMaxLateFee szTitle
    9s        6s             10s           lf            lf          40s (may contain spaces)
**************************************************************************/

int getBooks(Book bookM[], char * pszBookFileName)
{
    char szInputBuffer[100];		// input buffer for reading data
    int i = 0;                      // subscript in bookM
    int iScanfCnt = 0;                  // returned by sscanf
    FILE *pFileBook;              // Stream Input for Books data. 

    /* open the Books stream data file */
    if (pszBookFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-b");

    pFileBook = fopen(pszBookFileName, "r");
    if (pFileBook == NULL)
        exitError(ERR_BOOK_FILENAME, pszBookFileName);

    /* get book information until EOF
    ** fgets returns null when EOF is reached.
    */

    /**** your code ******/
	while (fgets(szInputBuffer, 100, pFileBook) != NULL)
    {
        // check for too many books to fit in the bookM array
        if (i >= MAX_BOOKS)
            exitError(ERR_TOO_MANY_BOOKS, szInputBuffer);
        iScanfCnt = sscanf(szInputBuffer, "%9s %6s %10s %lf %lf %[^\n]\n"
            , bookM[i].szBookId
            , bookM[i].szCustomerId
            , bookM[i].szCheckedOutDt
            , &bookM[i].dLateFeePerDay
            , &bookM[i].dMaxLateFee
            , bookM[i].szTitle);
        
        // It should have populated all six variables, if not exit with
        // an error.
        if (iScanfCnt < 6)
            exitError(ERR_BOOK_DATA, szInputBuffer);
        i++;
    }

    fclose(pFileBook);
    return i;
}

/******************** sortBooks **************************************
    void sortBooks(Book bookM[], int iBookCnt)
Purpose: 
Bubble sort initial book organization.
Parameters:
  		Book bookM[]		Array used for holding the book information.
		int iBookCnt		Number of books in the array.
Returns:
    
Notes:
    
**************************************************************************/
void sortBooks(Book bookM[], int iBookCnt)
{
    /**** your code ******/
	bool bChange = TRUE;
	for (i=0;i<(iBookCnt-1) and bChange;i=i+1)
	{
		bChange = FALSE;
		for(j=0;j<iBookCnt-i-1;j=j+1)
		{
			if bookM[j]>bookM[j+1]
			{
				//Exchange key[j] and key[j+1];
				char temp[];
				//strcpy(temp,bookM[j]);
				//strcpy(bookM[j], bookM[1]);
				//strcpy(bookM[1], temp);
				bChange = TRUE;
			}
		}
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
void printBooks(char *pszHeading, Book bookM[], int iBookCnt)
{
    int i;
    printf("\n%s\n", pszHeading);

    // output column headings
    printf("   %-9s %-40s %-8s %-10s %-8s %-8s \n"
        , "Book Id", "Title", "Customer", "Ck Out Dt", "Late Fee", "Max Late");

    // output each book in a table format
    for (i = 0; i < iBookCnt; i++)
    {
        printf("   %-9s %-40s %-8s %-10s %8.2lf %8.2lf \n"
            , bookM[i].szBookId
            , bookM[i].szTitle
            , bookM[i].szCustomerId
            , bookM[i].szCheckedOutDt
            , bookM[i].dLateFeePerDay
            , bookM[i].dMaxLateFee
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
Notes:

**************************************************************************/
void processCustomerCommand(Book bookM[], int iBookCnt
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer)
{
    int iscanfCnt;
    Transaction transaction;
    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
        // get the Customer Identification Information
        // your code

        // Check for bad customer identification data
        if (iscanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, pszRemainingInput);
    }
    else if (strcmp(pszSubCommand, "COMPLETE") == 0)
    {
        // print the customer's total fees
        // your code 

    }
    else if (strcmp(pszSubCommand, "ADDRESS") == 0)
    {
        // get the postal address and print it
        // your code

        // print the column heading for the transactions
        printf("\t\t\t\t\t%-5s  %-9s  %-10s\n", "Trans", "Book", "Date");

    }
    else if (strcmp(pszSubCommand, "TRANS") == 0)
    {
        // your code

        printf("\n");
    }
    else printf("   *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}

/********************processBookCommand *****************************
    void processBookCommand(Book bookM[], int iBookCnt
         , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the BOOK command:
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
void processBookCommand(Book bookM[], int iBookCnt
                             , char *pszSubCommand, char *pszRemainingInput)
{
    Book book;

    int iScanfCnt;
    int i;
    // Determine what to do based on the subCommand
    // your code
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
Notes:

**************************************************************************/
int searchBooks(Book bookM[], int iBookCnt, char *pszMatchBookId)
{
    // your code
}


/*** include your processTransaction code from program #2 ***/
    // your code
