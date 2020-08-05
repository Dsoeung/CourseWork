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
#include <string.h>
#include <assert.h>
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
#include "cs3743p2.h"

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
int hashCreate(char szFileNm[], HashHeader *pHashHeader)
{
    FILE *pFile;
    pFile = fopen(szFileNm, "rb");
    if(pFile != NULL)
    {
        return RC_FILE_EXISTS;      // File existed
    }

    pFile = fopen(szFileNm, "wb");
    long rc, lRBA;
    int rcFseek;

    lRBA = 0 * pHashHeader->iRecSize;           // determines the offset
    rcFseek = fseek(pFile, lRBA, SEEK_SET);     // fseek to the position in the file
    assert(rcFseek == 0);

    rc = fwrite(&(*pHashHeader), pHashHeader->iRecSize, 1L, pFile);
    if(rc == 0)
    {
        return RC_LOC_NOT_WRITTEN;
    }

    fclose(pFile);
    return RC_OK;
}

/* int hashCreate(char szFileNm[], HashHeader *pHashHeader){
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
} */

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
int hashOpen(char szFileNm[], HashFile *pHashFile)
{
    long rc;

    pHashFile->pFile = fopen(szFileNm, "rb+");
    if(pHashFile->pFile == NULL)
    {
        return RC_FILE_NOT_FOUND;      // File was not found
    }

    rc = fread(&pHashFile->hashHeader, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile);
    if(rc == 0){
        return RC_HEADER_NOT_FOUND;
    }else
    {
        return RC_OK;
    }
}

/******************** readRec ********************************************
    int readRec(HashFile *pHashFile, int iRBN, void *pRecord)
Purpose:
    This function reads a record at the specified RBN in the
    specified file. Returns if the location couldn't be found
    or if was found.
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I int iRBN                  RBN to be looked for in file
    O void *pRecord             Vehicle info
Return value:
    RC_LOC_NOT_FOUND - Location not found for read
    RC_OK - normal
**************************************************************************/
int readRec(HashFile *pHashFile, int iRBN, void *pRecord)
{
    Book tmp;

    int rcFseek, RecSize;
    long lRBA, rc;

    RecSize = pHashFile->hashHeader.iRecSize;               // size for one Record
    lRBA = iRBN * RecSize;                                  // determines the offset
    rcFseek = fseek(pHashFile->pFile, lRBA, SEEK_SET);      // fseek to the position in the file
    assert(rcFseek == 0);


    rc = fread(&tmp ,RecSize, 1L, pHashFile->pFile);
    *(Book*) pRecord = tmp;
    if(rc != 1 || tmp.szBookId[0] == '\0'){
        return RC_LOC_NOT_FOUND;
    }else
    {
        return RC_OK;
    }

}

/******************** writeRec *******************************************
    int writeRec(HashFile *pHashFile, int iRBN, void *pRecord)
Purpose:
    This function writes a record to the specified RBN in the
    specified file and returns if the write failed or if it
    was successful.
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I int iRBN                  RBN to be written to in file
    I void *pRecord             Vehicle info
Return value:
    RC_LOC_NOT_WRITTEN - Location not written for write
    RC_OK - normal
**************************************************************************/
int writeRec(HashFile *pHashFile, int iRBN, void *pRecord)
{
    int rcFseek, RecSize;
    long lRBA, rc;
    Book tmp = *(Book*) pRecord;

    RecSize = pHashFile->hashHeader.iRecSize;               // size for one Record
    lRBA = iRBN * RecSize;                                  // determines the offset
    rcFseek = fseek(pHashFile->pFile, lRBA, SEEK_SET);      // fseek to the position in the file
    assert(rcFseek == 0);

    rc = fwrite(&tmp, RecSize, 1L, pHashFile->pFile);
    if(rc == 0)
        {
        return RC_LOC_NOT_WRITTEN;
    }else
    {
        return RC_OK;
    }
}

/******************** vehicleInsert **********************************
    int vehicleInsert(HashFile *pHashFile, Vehicle *pVehicle)
Purpose:
    This function inserts a vehicle into the specified file and
    returns if the record already exist or if it's a synonym.
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I Vehicle *pVehicle         Vehicle info
Return value:
    RC_REC_EXISTS - record already exists
    RC_TOO_MANY_COLLISIONS - probing found too many collisions
**************************************************************************/
int insertBook(HashFile *pHashFile, Book *pBook)
{
    int i, rc, RBN;
    Book rBook;

    RBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
    rc = readRec(&(*pHashFile), RBN, &rBook);
    if(rc == RC_LOC_NOT_FOUND || rBook.szBookId[0] == '\0')
    {
        writeRec(&(*pHashFile), RBN, &(*pBook));
    }
    else if(rc == RC_OK && (strcmp(pBook->szBookId, rBook.szBookId)==0))
    {
        return RC_REC_EXISTS;
    }else
    {
        //synonym code goes here
        int nRBN = -1;  // RBN for inserting synonym
                        // if still -1 when loop ends then no open spots
        for(i=1;i<pHashFile->hashHeader.iMaxProbes;i++)
        {
            rc = readRec(&(*pHashFile), RBN+i, &rBook);
            if(rBook.szBookId[0] == '\0')
            {
               if(nRBN == -1)
               {
                  nRBN = RBN+i;
               }
            }else if(rc == RC_OK && strcmp(pBook->szBookId, rBook.szBookId)==0)
            {
                return RC_REC_EXISTS;
            }
        }
        if(nRBN != -1)
        {
            writeRec(&(*pHashFile), nRBN, &(*pBook));
        }else
        {
            return RC_TOO_MANY_COLLISIONS;
        }

    }
}

/******************** BookRead **********************************
    int BookRead(HashFile *pHashFile, Book *pBook, int *piRBN)
Purpose:
    This function reads the specified Book by its szBookId.
    Returns if RC_OK if Book is found or RC_REC_NOT_FOUND if
    not found
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I/O Book *pBook       Book info
    O int *piRBN                result RBN
Return value:
    RC_OK - normal
    RC_REC_NOT_FOUND - record not found
**************************************************************************/
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN)
{
    int i, RBN;
    Book rBook;

    RBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
    readRec(&(*pHashFile), RBN, &rBook);
    if(strcmp(pBook->szBookId, rBook.szBookId) == 0)
    {
        *pBook = rBook;
        *piRBN = RBN;
        return RC_OK;
    }else
    {
        //synonym code goes here
        for(i=1;i<pHashFile->hashHeader.iMaxProbes;i++)
        {
            if(RBN+i > pHashFile->hashHeader.iMaxPrimary)
            {
                return RC_REC_NOT_FOUND; // went past max amount of records
            }

            readRec(&(*pHashFile), RBN+i, &rBook);

            if(strcmp(pBook->szBookId, rBook.szBookId) == 0)
            {
                *pBook = rBook;
                *piRBN = RBN+i;
                return RC_OK;
            }

        }
        // if loop ends max probes reached
        return RC_REC_NOT_FOUND;
    }
}

/******************** vehicleUpdate **********************************
    int vehicleUpdate(HashFile *pHashFile, Vehicle *pVehicle)
Purpose:
    This function reads the specified vehicle by its szVehicleId.
    If found, it updates the contents of the vehicle in the hash file.
    If not found, it returns RC_REC_NOT_FOUND.
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I Vehicle *pVehicle       Vehicle info
Return value:
    RC_OK - normal
    RC_REC_NOT_FOUND - record not found
**************************************************************************/
int updateBook(HashFile *pHashFile, Book *pBook)
{
    int i, RBN;
    Book rBook;

    RBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);

    for(i=0;i<pHashFile->hashHeader.iMaxProbes;i++)
    {
        if(RBN+i > pHashFile->hashHeader.iMaxPrimary)
        {
            return RC_REC_NOT_FOUND; // went past max amount of records
        }

        readRec(&(*pHashFile), RBN+i, &rBook);

        if(strcmp(pBook->szBookId, rBook.szBookId) == 0)
        {
            writeRec(&(*pHashFile), RBN+i, &(*pBook));
            return RC_OK;
        }

    }
    // if loop ends max probes reached
    return RC_REC_NOT_FOUND;

}

/******************** vehicleDelete **********************************
    int vehicleDelete(HashFile *pHashFile, char *pszVehicleId)
Purpose:
    This function finds the specified vehicle and deletes it by simply
    setting all bytes in that record to '\0'.
Parameters:
    I HashFile *pHashFile       contains HashHeader and File pointer
    I char *pszVehicleId        Vehicle id
Return value:

**************************************************************************/
int deleteBook(HashFile *pHashFile, Book *pBook)
{
   return RC_NOT_IMPLEMENTED;
}