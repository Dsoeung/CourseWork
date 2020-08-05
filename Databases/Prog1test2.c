#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs3743p1.h"

int hashCreate(char szFileNm[], HashHeader *pHashHeader)
{
    FILE *pHashFile;

    if (fopen(szFileNm, "rb") != NULL)
        return RC_FILE_EXISTS;

    pHashFile = fopen(szFileNm, "wb");

    pHashHeader->iHighOverflowRBN = pHashHeader->iNumPrimary;

    fwrite(pHashHeader, sizeof(HashHeader), 1L, pHashFile);

    fclose(pHashFile);

    return RC_OK;
}

int hashOpen(char szFileNm[], HashFile *pHashFile)
{
    pHashFile->pFile = fopen(szFileNm, "rb+");

    if (pHashFile->pFile == NULL)
        return RC_FILE_NOT_FOUND;
    
    if (fread(&(pHashFile->hashHeader), sizeof(HashHeader), 1L, pHashFile->pFile) == 0)
        return RC_HEADER_NOT_FOUND;

    return RC_OK;
}

int readRec(HashFile *pHashFile, int iRBN, void *pRecord)
{
    long lRBA = 0;
    lRBA = iRBN * pHashFile->hashHeader.iRecSize;

    fseek(pHashFile->pFile, lRBA, SEEK_SET);

    if (fread(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile) != 1)
        return RC_LOC_NOT_FOUND;

    return RC_OK;
}

int writeRec(HashFile *pHashFile, int iRBN, void *pRecord)
{
    long lRBA = 0;
    lRBA = iRBN * pHashFile->hashHeader.iRecSize;

    fseek(pHashFile->pFile, lRBA, SEEK_SET);

    if (fwrite(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile) != 1)
        return RC_LOC_NOT_WRITTEN;

    return RC_OK;
}

int movieInsert(HashFile *pHashFile, Movie *pMovie)
{
    int iRBN = 0;
    iRBN = hash(pMovie->szMovieId, pHashFile->hashHeader.iNumPrimary);
    Movie *pCursor = (Movie *) malloc(sizeof(Movie));
    if (readRec(pHashFile, iRBN, pCursor) == RC_LOC_NOT_FOUND)
    {
        pMovie->iNextChain = 0;
        writeRec(pHashFile, iRBN, pMovie);
        free(pCursor);
        return RC_OK;
    }
    else if (pCursor->szMovieId[0] == '\0')
    {
        pMovie->iNextChain = 0;
        writeRec(pHashFile, iRBN, pMovie);
        free(pCursor);
        return RC_OK;
    }
    else if (strcmp(pCursor->szMovieId, pMovie->szMovieId) == 0)
    {
        free(pCursor);
        return RC_REC_EXISTS;
    }
    free(pCursor);
    return RC_SYNONYM;
}

int movieRead(HashFile *pHashFile, Movie *pMovie)
{
    int iRBN = hash(pMovie->szMovieId,pHashFile->hashHeader.iNumPrimary);
    Movie *pCursor = (Movie *) malloc(sizeof(Movie));
    readRec(pHashFile, iRBN, pCursor);

    if(strcmp(pCursor->szMovieId, pMovie->szMovieId) == 0)
    {
        memcpy(pMovie, pCursor, sizeof(Movie));
        free(pCursor);
        return RC_OK;
    }
    else
    {
        free(pCursor);
        return RC_REC_NOT_FOUND;
    }
}