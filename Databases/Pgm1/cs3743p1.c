/**********************************************************************
cs3743p1.c by Darin Soeung
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
#include <stdlib.h>
#include <string.h>
/**********************************************************************
cs3743p1.h
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
#include "cs3743p1.h"
 
/******************** hashCreate **********************************
int hashCreate(char szFileNm[], HashHeader *pHashHeader) 
Purpose:
    This function creates a hash file containing only the HashHeader record.
Parameters:
    I char szFileNm[]
	I HashHeader *pHashHeader
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
    I char szFileNm[]
	I HashFile *HashFile
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
    I HashFile *pHashFile
	I int iRBN
	I void *pRecord
Notes:
	n/a
Return value:
	return RC_LOC_NOT_FOUND		HashHeader read has failed.
	return RC_OK	Function executed successfully.
**************************************************************************/
int readRec(HashFile *pHashFile, int iRBN, void *pRecord){
	long RBA = 0;
    RBA = iRBN * pHashFile->hashHeader.iRecSize;

     fseek(pHashFile->pFile, RBA, SEEK_SET);

    if (fread(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile) != 1)
        return RC_LOC_NOT_FOUND;

    return RC_OK;
}

/******************** writeRec **********************************
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord)
Purpose:
    This function writes a record to the specified RBN in the specified file.
Parameters:
    I HashFile *pHashFile
	I int iRBN
	I void *pRecord
Notes:
	n/a
Return value:
	return RC_LOC_NOT_WRITTEN		fwrite has failed to write record to file.
	return RC_OK					Function executed successfully.
**************************************************************************/
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord){
	Book tmp = *(Book*) pRecord;
	
	//initialize an int to make checking if the write failed less messy.
	int iWriteCheck;
	
	//Determine int(RBA) based on the iRBN and pHashFile->hashHeader.iRecSize.
	int iRBA = 0;
    iRBA = iRBN * pHashFile->hashHeader.iRecSize;

	//Use fseek to position the file in that location (iRBA).
	//SEEK_SET to start at beginning
    fseek(pHashFile->pFile, iRBA, SEEK_SET);

	//Use fwrite to write that record to the file.
	iWriteCheck = fwrite(&tmp, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
	//printf("iWriteCheck : %d \n", iWriteCheck);
	
	//if iWriteCheck does not equal 1 (has not successfully written the single record passed), return RC_LOC_NOT_WRITTEN
    if ( iWriteCheck == 0)
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
    I HashFile *pHashFile
	I Book *pBook
Notes:
	n/a
Return value:
	return RC_OK				Function executed successfully.
	return RC_SYNONYM			If that record exists and that book's szBookId does not match pBook->szBookId
	return RC_REC_EXISTS		If that record exists and that book's szBookId matches pBook->szBookId
**************************************************************************/
int insertBook(HashFile *pHashFile, Book *pBook){
	//Integer that holds the Record Book Number in the file
	int iRBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	// no pointer just plain Book
    Book bTemp;
	
		
    int rec_num = readRec(pHashFile, RBN, &bTemp);
    if(rec_num == RC_LOC_NOT_FOUND || bTemp.szBookId[0] == '\0'){
        //printf("wrote it \n");
		writeRec(pHashFile, RBN, pBook);
        return RC_OK;

    }
    if(strcmp(bTemp.szBookId, pBook->szBookId)==0){
        return RC_REC_EXISTS;
    }
    return RC_SYNONYM;
}

/******************** readBook **********************************
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN)
Purpose:
    This function reads the specified book by its szBookId.
Parameters:
    I HashFile *pHashFile
	I Book *pBook
	I int *piRBN
Notes:
	n/a
Return value:
	return RC_REC_NOT_FOUND		If the book at that location does not match the specified pBook->szBookId.
	return RC_OK				If the book at that location matches the specified pBook->szBookId.
**************************************************************************/
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN){
	
	Book bTemp;
	int iRBN;
	
	//Since pBook->szBookId ^ was provided
	//determine the RBN using the driver's hash function.
    iRBN = hash(pBook -> szBookId, pHashFile->hashHeader.iMaxPrimary);

	//Use readRec to read the record at that RBN. Be careful to not initially overwrite pBook!
	int readCheck = readRec(pHashFile, *piRBN, &bTemp);
	
	//check if the book at that location matches the specified pBook->szBookId
	//size to bTemp.szBookId as it should ALWAYS exist
	if(readCheck !=RC_OK)
		//printf("%s",bTemp.szBookId);
		//printf("%s",pBook -> szBookId);
        return readCheck;
	//if book match return rec_ok else return not found
	if(strcmp(bTemp.szBookId, pBook -> szBookId)== 0)
	{
        *pBook = bTemp;
		 //printf("%s",pBook -> szBookId);
		return RC_OK;
	}else
	{
		 //printf("%s",pBook -> szBookId);
		return RC_REC_NOT_FOUND;
	}
}