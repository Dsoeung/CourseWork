/**********************************************************************
cs3743p2.c by Darin Soeung
Purpose:	
    The purpose of this program is to handle hashFiles and their
	respective records.
Command Parameters:
    Command parameters are located from cs3743p1Driver.c : 
	p1 < infile > outfile
    The stream input file contains commands.  There are commands 
    which specify create, insert, read, delete, update, and printall.
   
    Commands in the input file:
    
    * comment       
        This is just a comment which helps explain what is being tested.
    
    CREATE BOOK fileName maxPrimary maxOvr maxProbes   
        Create the specified hash file if it doesn't already exist
        using hashCreate.  This only writes the header record at 
        record 0.  This closes the created file.
        The size of each record is the size of a Book structure.   
    OPEN BOOK fileName 
        Opens the specified file using hashOpen.  Place the returned
        FILE pointer in pHashFile->pFile.
    INSERT BOOK bookId,title,publisher,subject,pages
        Uses sscanf to get those attributes and populate a Book structure.
        It invokes insertBook to insert the Book into the hash file.
    READ BOOK bookId
        Invokes readBook to read the Book from the hash file and (if found) 
        prints that Book.
    PRINTALL BOOK fileName
        Prints the contents of the specified file.  
        - opens the file
        - reads and prints each record
    NUKE BOOK fileName
        Removes the specified file.
Input:
    The program will take inputs from an input.txt file
Results:
    This program creates HashFiles.
Returns:
    return RC_FILE_EXISTS	File already exists
	return RC_FILE_NOT_FOUND	HashFile read has failed.
	return RC_HEADER_NOT_FOUND	HashHeader read has failed.
	return RC_LOC_NOT_FOUND		HashHeader read has failed.
	return RC_LOC_NOT_WRITTEN		fwrite has failed to write record to file.
	return RC_OK			Function executed successfully.
	return RC_REC_EXISTS		If that record exists and that book's szBookId matches pBook->szBookId
	return RC_REC_NOT_FOUND		If the book at that location does not match the specified pBook->szBookId.
	return RC_SYNONYM			If that record exists and that book's szBookId does not match pBook->szBookId
Notes:
    
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/**********************************************************************
cs3743p2.h
Purpose:
Defines constants for
        return codes
        maximum record size
Defines typedefs for
    HashHeader - the first record of the HashFile will contain information 
	describing the contents.
	HashFile - contains the file pointer and hashHeader for the hash file.
	Book - describes the contents of a book record
Notes:

**********************************************************************/
#include "cs3743p2.h"

/******************** hashCreate **********************************
int hashCreate(char szFileNm[], HashHeader *pHashHeader) 
Purpose:
    This function creates a hash file containing only the HashHeader record.
Parameters:
    I char szFileNm[]			Hash file name
	I HashHeader *pHashHeader	the header record contents for a hash file
	I
Notes:
    Include any special assumptions.  If global variables are referenced, 
    reference them.  Explain critical parts of the algorithm.
Return value:
    return RC_FILE_EXISTS	File already exists
	return RC_OK			Function executed successfully.
**************************************************************************/
int hashCreate(char szFileNm[], HashHeader *pHashHeader){
	//initialize file to hold szFileNm[]
	FILE *pHashFileTemp;
	
	//Check if file exists, if yes return RC_FILE_EXISTS, rb cause if it exists opens for update.
    if (fopen(szFileNm, "rb+") != NULL)
	{
        return RC_FILE_EXISTS;
	}
	else
	{
	//Open and create Binary file since we checked existence!
    pHashFileTemp = fopen(szFileNm, "wb+");

	//updating iMaxOvRbn to iMaxPrimary
    pHashHeader->iMaxOvRBN = pHashHeader->iMaxPrimary;
	
	//Write the pHashHeader record to file at RBN 0.
    fwrite(pHashHeader, sizeof(HashHeader), 1L, pHashFileTemp);
	
	//fclose the file!
    fclose(pHashFileTemp);
	
	//Return RC_OK on success
    return RC_OK;
	}
}

 /******************** hashOpen **********************************
int hashOpen(char szFileNm[], HashFile *pHashFile)
Purpose:
    This function opens an existing hash file which must contain a HashHeader record, and sets the pHashFile->pFile.
	It returns the HashHeader record by setting pHashFile->hashHeader.
Parameters:
    I char szFileNm[]		Hash file name
	I HashFile *HashFile	HashFile structure containing a FILE pointer and HashHeader
Notes:
	This function will NOT close the file since the other functions will assume that it is open.
Return value:
    return RC_FILE_NOT_FOUND	HashFile read has failed.
	return RC_HEADER_NOT_FOUND	HashHeader read has failed.
	return RC_OK				Function executed successfully.
**************************************************************************/
int hashOpen(char szFileNm[], HashFile *pHashFile){
	//sets the pHashFile->pFile
	pHashFile->pFile = fopen(szFileNm, "rb+");
	
	//check if file exists from fopen
    if (pHashFile->pFile == NULL)
	{
        return RC_FILE_NOT_FOUND;
    }
	else //File exists!
	{
		//reading the HashHeader and reaturn it through the parameter.
		if (fread(&(pHashFile->hashHeader), pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile) == 0)
		{
			//on fail
			return RC_HEADER_NOT_FOUND;
		}
	}
	//Function successfully executed, DO NOT close pFile!
	//files will assume it is open
    return RC_OK;
}

/******************** readRec **********************************
int readRec(HashFile *pHashFile, int iRBN, void *pRecord)
Purpose:
    This function reads a record at the specified RBN in the specified file.
Parameters:
    I HashFile *pHashFile	HashFile structure containing a FILE pointer and HashHeader
	I int iRBN				
	I void *pRecord
Notes:
	n/a
Return value:
	return RC_LOC_NOT_FOUND		HashHeader read has failed.
	return RC_OK	Function executed successfully.
**************************************************************************/
int readRec(HashFile *pHashFile, int iRBN, void *pRecord){
	//initialize an int to make checking if the read failed less messy.
	int iReadCheck;
	
	//Determine int(RBA) based on the iRBN and pHashFile->hashHeader.iRecSize.
	int iRBA = 0;
    iRBA = iRBN * pHashFile->hashHeader.iRecSize;
	
	//Use fseek to position the file in that location
	//SEEK_SET to start at beginning
    fseek(pHashFile->pFile, iRBA, SEEK_SET);
	
	//Use fread to read that record and return it through pRecord.
	iReadCheck = fread(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
	
	//Check if location is not found, return RC_LOC_NOT_FOUND.
	//if iReadCheck has not read 1 record, return RC_LOC_NOT_FOUND.
	
	//printf("iReadCheck : %d \n", iReadCheck);
    if (iReadCheck != 1)
	{
        return RC_LOC_NOT_FOUND;
	}
	//If the location is found, return RC_OK.
    return RC_OK;
}

/******************** writeRec **********************************
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord)
Purpose:
    This function writes a record to the specified RBN in the specified file.
Parameters:
    I HashFile *pHashFile	HashFile structure containing a FILE pointer and HashHeader
	I int iRBN
	I void *pRecord
Notes:
	n/a
Return value:
	return RC_LOC_NOT_WRITTEN		fwrite has failed to write record to file.
	return RC_OK					Function executed successfully.
**************************************************************************/
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord){
	
	//initialize an int to make checking if the write failed less messy.
	int iWriteCheck;
	
	//Determine int(RBA) based on the iRBN and pHashFile->hashHeader.iRecSize.
	int iRBA = 0;
    iRBA = iRBN * pHashFile->hashHeader.iRecSize;

	//Use fseek to position the file in that location (iRBA).
	//SEEK_SET to start at beginning
    fseek(pHashFile->pFile, iRBA, SEEK_SET);

	//Use fwrite to write that record to the file.
	iWriteCheck = fwrite(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
	//printf("iWriteCheck : %d \n", iWriteCheck);
	
	//if iWriteCheck does not equal 1 (has not successfully written the single record passed), return RC_LOC_NOT_WRITTEN
    if ( iWriteCheck!= 1)
	{
        return RC_LOC_NOT_WRITTEN;
	}
	else
	{
	//return RC_OK so all branches have a return statement
    return RC_OK;
	}
}

/******************** insertBook **********************************
int insertBook(HashFile *pHashFile, Book *pBook)
Purpose:
    This function inserts a book into the specified file.
Parameters:
    I HashFile *pHashFile	HashFile structure containing a FILE pointer and HashHeader
	I Book *pBook			Book structure represents one book.
Notes:
	n/a
Return value:
	return RC_OK				Function executed successfully.
	return RC_SYNONYM			If that record exists and that book's szBookId does not match pBook->szBookId
	return RC_REC_EXISTS		If that record exists and that book's szBookId matches pBook->szBookId
**************************************************************************/
int insertBook(HashFile *pHashFile, Book *pBook){
	Book bTemp;
	bool bSpots;
	int iNewRBN;
	int iRBN; 
	int K = 1;
	int readCheck;
	
	//Assuming there are no open spots
	//true = none open, false = open spots
	bSpots = true;
	
    //Determine the RBN using the driver's hash function
	iRBN = hash(pBook -> szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	//Use readRec to read the record at that RBN. 
	//USE &bTemp as that holds memory not just bTemp
    readCheck = readRec(pHashFile, iRBN, &bTemp);
	
	//IF that location doesn't exist (readCheck == RC_LOC_NOT_FOUND)
	//or (||) the record at that location has a szBookId[0] == '\0':
    if(readCheck == RC_LOC_NOT_FOUND || bTemp.szBookId[0] == '\0')
    {
		//Write the new  book record (using pBook) at that location using writeRec.
        writeRec(pHashFile, iRBN, pBook);
    }
    else 
	// IF that record exists and that book's szBookId matches pBook->szBookId ~
	// Don't need to deref szBookId, bTemp is NOT a pointer
	//size to bTemp.szBookId as it should ALWAYS exist
	if(strncmp( pBook -> szBookId, bTemp.szBookId, sizeof(bTemp.szBookId))== 0)
    {
		//(Do not update it.)
        return RC_REC_EXISTS;
    }else //Pgm2
    {               
		//Determine if it exists as a synonym using probing with a K value of 1.  
		//If it does exist, return the book via pBook, set *piRBN to its actual RBN (where it was found),
		//and return RC_OK.
        while(K<pHashFile->hashHeader.iMaxProbes)
        {
			
            readCheck = readRec(pHashFile, iRBN+K, &bTemp);
			//if the record at that location has a szBookId[0] == '\0':
            if(bTemp.szBookId[0] == '\0')
            {
				//if there is an open spot
               if(bSpots)
               {
				   //set spots to open
				   bSpots = false;
				   //set the RBN to the empty spot
				   iNewRBN = iRBN+K;
               }
            }
			//If it does already exist
			else if(readCheck == RC_OK && strncmp(pBook->szBookId, bTemp.szBookId, sizeof(bTemp.szBookId))==0)
            {
				// return RC_REC_EXISTS.
				// (Do not update it.)
                return RC_REC_EXISTS;
            }
			//Iterate to next record
			K++;
        }
		//Spots are open ! after we've probed
        if(bSpots == false)
        {
			//write it to that empty slot
			//at iNewRBN
            writeRec(pHashFile, iNewRBN, pBook);
        }
		//Spots aren't open !
		if(bSpots == true)
        {
			//return RC_TOO_MANY_COLLISIONS.
            return RC_TOO_MANY_COLLISIONS;
        }
    }
	return RC_OK;
}

/******************** readBook **********************************
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN)
Purpose:
    This function reads the specified book by its szBookId.
Parameters:
    I HashFile *pHashFile	HashFile structure containing a FILE pointer and HashHeader
	I Book *pBook			Book structure represents one book.
	O int *piRBN			Pointer to Record block number.
Notes:
	n/a
Return value:
	return RC_OK				If the book at that location matches the specified pBook->szBookId.
	return RC_REC_NOT_FOUND		If the book at that location does not match the specified pBook->szBookId.
**************************************************************************/
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN)
{
	Book bTemp;
	int iRBN;
    int K = 1;
	
	//Since pBook->szBookId ^ was provided
	//determine the RBN using the driver's hash function.
    iRBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	//Use readRec to read the record at that RBN. Be careful to not initially overwrite pBook!
    readRec(pHashFile, iRBN, &bTemp);
	
	//check if the book at that location matches the specified pBook->szBookId
	//size to bTemp.szBookId as it should ALWAYS exist
    if(strncmp(pBook->szBookId, bTemp.szBookId, sizeof(bTemp.szBookId))== 0)
    {
		//return the book via pBook 
		*piRBN = iRBN;
        *pBook = bTemp;
		//and return RC_OK.
        return RC_OK;
    }else
    {
		//pgm2 code
		//Determine if it exists as a synonym using probing with a K value of 1.  
		//If it does exist, return the book via pBook, set *piRBN to its actual RBN (where it was found),
		//and return RC_OK.
        while(K<pHashFile->hashHeader.iMaxProbes)
        {
			//if greater than the number of records
            if(iRBN+K > pHashFile->hashHeader.iMaxPrimary)
            {
				//return RC_REC_NOT_FOUND;
                return RC_REC_NOT_FOUND;
            }
			
			//read rec at RBN == iRBN+K, and put it into bTemp
            readRec(pHashFile, iRBN+K, &bTemp);
			
			//If it does exist 
            if(strncmp(pBook->szBookId, bTemp.szBookId, sizeof(bTemp.szBookId)) == 0)
            {
				//return the book via pBook
                *pBook = bTemp;
                //set *piRBN to its actual RBN 
				*piRBN = iRBN+K;
				//return RC_OK.
                return RC_OK;
            }
			//Iterate to next record
			K++;
        }
		//If you have read for the maximum probes and it wasn’t found.
        return RC_REC_NOT_FOUND;
    }
}

/******************** updateBook **********************************
    int updateBook(HashFile *pHashFile, Book *pBook) 
Purpose:
    This function reads the specified book by its szBookId.
	If found, it updates the contents of the book in the hash file.  
	If not found, it returns RC_REC_NOT_FOUND.
Parameters:
    I HashFile	*pHashFile	HashFile structure containing a FILE pointer and HashHeader
    I Book		*pBook		Book structure represents one book.
Return value:
    RC_OK - normal
    RC_REC_NOT_FOUND - record not found
**************************************************************************/
int updateBook(HashFile *pHashFile, Book *pBook)
{
	Book bTemp;
	int iRBN;
	int iReadCheck;
	//probably shouldof just made a global K
	int K = 1; 
    
	//Since pBook->szBookId ^ was provided
	//determine the RBN using the driver's hash function.
    iRBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	iReadCheck = readRec(pHashFile, iRBN, &bTemp);

	//IF that location doesn't exist (readCheck == RC_LOC_NOT_FOUND)
	//or (||) the record at that location has a szBookId[0] == '\0':
	if(iReadCheck == RC_LOC_NOT_FOUND || bTemp.szBookId[0] == '\0'){
		//record is not found
		return RC_REC_NOT_FOUND;
	} 
	//Determine if it exists as a synonym using probing with a K value of 1.  
	//If it does exist, return the book via pBook, set *piRBN to its actual RBN (where it was found),
	//and return RC_OK.
	if (iReadCheck == RC_OK){
		while(K<pHashFile->hashHeader.iMaxProbes)
		{
			//if greater than the number of records
			if(iRBN+K > pHashFile->hashHeader.iMaxPrimary)
			{
				//return RC_REC_NOT_FOUND;
				return RC_REC_NOT_FOUND;
			}
		
			//read rec at RBN == iRBN+K, and put it into bTemp
			readRec(pHashFile, iRBN+K, &bTemp);
		
			//If it does exist
			if(strncmp(pBook->szBookId, bTemp.szBookId, sizeof(bTemp.szBookId)) == 0)
			{
				//it updates the contents of the book in the hash file
				writeRec(pHashFile, iRBN+K, pBook);
				//return RC_OK
				return RC_OK;
			}
			//Iterate to next record
			K++;
		}
	}
	//If you have read for the maximum probes and it wasn’t found.
    return RC_REC_NOT_FOUND;
}

/******************** deleteBook **********************************
    int deleteBook(HashFile *pHashFile, Book *pBook) 
Purpose:
    This function finds the specified book and deletes it 
	by simply setting all bytes in that record to '\0'. 
Notes:
	If you did not do the extra credit, create a simple function that just returns RC_NOT_IMPLEMENTED.
Parameters:
    I HashFile *pHashFile	HashFile structure containing a FILE pointer and HashHeader
    I Book *pBook       	Book structure represents one book.
Return value:
	return RC_NOT_IMPLEMENTED - Function not implemented.
**************************************************************************/
int deleteBook(HashFile *pHashFile, Book *pBook)
{
   return RC_NOT_IMPLEMENTED;
}