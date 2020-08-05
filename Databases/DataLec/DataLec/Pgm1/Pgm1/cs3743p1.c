#include <stdio.h>
#include "cs3743p1.h"

/**********************************************************************
cs3743p1.c
Purpose:
Description of its purpose.
Notes:
Description of any special information around usage.
**********************************************************************/
 
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
	
	HashFile *pHashFileTemp;
	
	//Check if file exists, if yes return RC_FILE_EXISTS, rb cause if it exists opens for update.
	pHashFileTemp->pFile  = fopen(szFileNm, "rb");
	if ((pHashFileTemp->pFile != NULL))
    {
        fclose(pHashFileTemp->pFile);
        return RC_FILE_EXISTS;
    }
	//Open and create Binary file
	pHashFileTemp->pFile = fopen(szFileNm, "w");
	//fwrite (pHashHeader, sizeof(HashHeader), 1L, pHashFileTemp->pFile ); pHashHeader -> iRecSize instead of sizeof hashheader?
	
	//setting iMaxOvRbn to iMaxPrimary
	pHashHeader->iMaxOvRBN = pHashHeader->iMaxPrimary;
	
	//Write the pHashHeader record to file at RBN 0.
	fwrite(pHashHeader, pHashHeader->iRecSize, 1L, pHashFileTemp->pFile);
	
	//fclose the file!
	fclose(pHashFileTemp->pFile);
	
	//Return RC_OK on success
	return RC_OK;
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
	//sets the pHashFile->pFile. 
	pHashFile->pFile = fopen(szFileNm, "rb+");
	
	//check if file exists from fopen
    if (pHashFile->pFile == NULL)
	{
        return RC_FILE_NOT_FOUND;
	}
    
	//reading the HashHeader and reaturn it through the parameter.
    if (fread(&(pHashFile->hashHeader), sizeof(HashHeader), 1L, pHashFile->pFile) == 0)
	{
        return RC_HEADER_NOT_FOUND;
	}
	//Function successfully executed, DO NOT pFile
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
	//initialize an int to make checking if the read failed less messy.
	int iReadCheck;
	
	//Determine int(RBA) based on the iRBN and pHashFile->hashHeader.iRecSize.
	int iRBA = pHashFile->hashHeader.iRecSize * iRBN;
	
	//Use fseek to position the file in that location
	int RecSeek = fseek(pHashFile->pFile, iRBN, SEEK_SET);

	//Use fread to read that record and return it through pRecord.
	iReadCheck = fread(&pRecord, sizeof(&pRecord), 1L, pHashFile->pFile);
	
	//Check if location is not found, return RC_LOC_NOT_FOUND.
	//if iReadCheck has not read 1 record, return RC_LOC_NOT_FOUND.
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
	//initialize an int to make checking if the write failed less messy.
	int iWriteCheck;
	
	//Determine int(RBA) based on the iRBN and pHashFile->hashHeader.iRecSize.
	int iRBA = pHashFile->hashHeader.iRecSize * iRBN;
	
	//Use fseek to position the file in that location.
    fseek(pHashFile->pFile, iRBA, SEEK_SET);

	//Use fwrite to write that record to the file.
	iWriteCheck = fwrite(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
	
	//if iWriteCheck does not equal 1 (has not successfully written the single record passed), return RC_LOC_NOT_WRITTEN
    if (iWriteCheck!= 1)
	{
        return RC_LOC_NOT_WRITTEN;
	}
	
    return RC_OK;
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
	return RC_REC_EXISTS		If that record exists and that book's szBookId matches pBook->szBookId
	return RC_OK				Function executed successfully.
**************************************************************************/
int insertBook(HashFile *pHashFile, Book *pBook){
    Book bTemp;  
	//Integer that holds the Record Book Number in the file
	int RBN;   
	
	//Determine the RBN using the driver's hash function
	RBN = hash(pBook -> szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	//Use readRec to read the record at that RBN. 
        readRec(pHashFile, RBN, &bTemp);

	//IF that location doesn't exist or (||) the record at that location has a szBookId[0] == '\0':
	if((bTemp.szBookId == NULL) || (bTemp.szBookId[0] == '\0'))
	{
			//Write the new  book record (using pBook) at that location using writeRec.
	writeRec(pHashFile, RBN, pBook);
	}
	else
	{
		// IF that record exists and that book's szBookId matches pBook->szBookId ~
		if(strncmp(bTemp.szBookId, pBook -> szBookId, sizeof(bTemp.szBookId))== 0)
		{
			//(Do not update it.)
			return RC_REC_EXISTS; 
		}
	}
	//Otherwise, return RC_SYNONYM.
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
	readRec(pHashFile, iRBN, &bTemp);
	
	//check if the book at that location matches the specified pBook->szBookId
	if(strcmp(bTemp.szBookId, pBook -> szBookId)== 0)
	{
		
		//return the book via pBook and return RC_OK.
		readRec(pHashFile, iRBN, pBook);
		return RC_OK;
	}
	//Otherwise, return RC_REC_NOT_FOUND 
	return RC_REC_NOT_FOUND;
}