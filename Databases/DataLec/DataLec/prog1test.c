#include<stdio.h>

#define TRUE                   1
#define FALSE                  0
// Return code values (some are not used for pgm#1)
#define RC_OK                  0
#define RC_FILE_EXISTS         1   // file already exists
#define RC_REC_EXISTS          2   // record already exists
#define RC_REC_NOT_FOUND       3   // record not found
#define RC_FILE_NOT_FOUND      4   // file not found
#define RC_HEADER_NOT_FOUND    5   // Header record not found
#define RC_BAD_REC_SIZE        6   // invalid record size in info record
#define RC_LOC_NOT_FOUND       7   // Location not found for read
#define RC_LOC_NOT_WRITTEN     8   // Location not written for write
#define RC_SYNONYM             9   // Synonym exists at the specified location
#define RC_NOT_IMPLEMENTED     10  // Not implemented
#define RC_TOO_MANY_COLLISIONS 11  // probing found too many collisions
#define RC_INVALID_PARM        99

#define MAX_REC_SIZE        500

typedef struct
{
    int  iMaxPrimary;       // Number of records defined for Primary
                            // area.  (This isn't the number of
                            // records currently in the Primary Area.)
    int  iRecSize;          // Size in bytes for each record in the
                            // hash file
    int  iMaxOvRBN;         // Maximum RBN in overflow.  (This isn't
                            // the highest RBN used so far.) (Pgm 2)
    int  iMaxProbes;        // Max number of probes for a collision (Pgm 2)

    char sbFiller[MAX_REC_SIZE];     // This should be set to zeros.  It is
                            // here to make certain the Info record is
                            // at least as big as the data.
} HashHeader;

typedef struct
{
    FILE *pFile;            // FILE pointer to the hash file
    HashHeader hashHeader;  // the header record contents for a hash file
} HashFile;

int hashCreate(char szFileNm[], HashHeader *pHashHeader){
	HashFile *pHashFileTemp;
	int iWrite;
	
	pHashFileTemp->pFile  = fopen(szFileNm, "rb");
	
	if ((pHashFileTemp->pFile != NULL))
    {
        fclose(pHashFileTemp->pFile);
        return RC_FILE_EXISTS;
    }
	//Open and create Binary file
	pHashFileTemp->pFile = fopen(szFileNm, "wb+");
	//fwrite (pHashHeader, sizeof(HashHeader), 1L, pHashFileTemp->pFile ); pHashHeader -> iRecSize instead of sizeof hashheader?
	
	pHashHeader->iMaxOvRBN = pHashHeader->iMaxPrimary; //set iMaxOvRbn to iMaxPrimary
	iWrite = fwrite(pHashHeader, pHashHeader->iRecSize, 1L, pHashFileTemp->pFile);
	fclose(pHashFileTemp->pFile);
	return RC_OK;
}

int hashOpen(char szFileNm[], HashFile *pHashFile){
pHashFile->pFile = fopen(szFileNm, "rb+");

    if (pHashFile->pFile == NULL)
        return RC_FILE_NOT_FOUND;
   
    if (fread(&(pHashFile->hashHeader), sizeof(HashHeader), 1L, pHashFile->pFile) == 0)
        return RC_HEADER_NOT_FOUND;

    return RC_OK;
}




int main() {
    int x=10;
    int y=25;
    int z=x+y;
    printf("Sum of x+y = %i", z);
}
