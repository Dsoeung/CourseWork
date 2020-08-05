int vehicleRead(HashFile *pHashFile, Book *pBook, int *piRBN)
{
    int i, RBN;
    Book bTemp;

    RBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxHash);
    readRec(&(*pHashFile), RBN, &bTemp);
    if(strcmp(pBook->szBookId, bTemp.szBookId) == 0)
    {
        *pBook = bTemp;
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

            readRec(&(*pHashFile), RBN+i, &bTemp);

            if(strcmp(pBook->szBookId, bTemp.szBookId) == 0)
            {
                *pBook = bTemp;
                *piRBN = RBN+i;
                return RC_OK;
            }

        }
        // if loop ends max probes reached
        return RC_REC_NOT_FOUND;
    }
}
int readBook(HashFile *pHashFile, Book *pBook, int *piRBN){
	
	Book bTemp;
	int iRBN;
	int k;
	
	//Since pBook->szBookId ^ was provided
	//determine the RBN using the driver's hash function.
    iRBN = hash(pBook -> szBookId, pHashFile->hashHeader.iMaxPrimary);

	//Use readRec to read the record at that RBN. Be careful to not initially overwrite pBook!
	int readCheck = readRec(pHashFile, *piRBN, &bTemp);
	
	//check if the book at that location matches the specified pBook->szBookId
	//size to bTemp.szBookId as it should ALWAYS exist
	if(readCheck !=RC_OK){
		//printf("%s",bTemp.szBookId);
		//printf("%s",pBook -> szBookId);
        return readCheck;
	}
	//if book match return rec_ok else return not found
	if(strcmp(bTemp.szBookId, pBook -> szBookId)== 0)
	{
		//return the book via pBook
        *pBook = bTemp;
		//set *piRBN
		*piRBN = iRBN;
		//printf("%s",pBook -> szBookId);
		return RC_OK;
	}else
		//Pgm2 Start! : 
		//Otherwise, it is a synonym to the book in the hashed location
	{
		for(k=1;k<pHashFile->hashHeader.iMaxProbes;k++)
        {
			//If you read past the end of the file
            if(iRBN+k > pHashFile->hashHeader.iMaxPrimary)
            {
				//return RC_REC_NOT_FOUND.
				printf("	Read: first if ");
                return RC_REC_NOT_FOUND;
            }

			//error check over, readrec at iRBN+k
            readRec(&(*pHashFile), iRBN+k, &bTemp);
			//If it does exist,
            if(strcmp(pBook->szBookId, bTemp.szBookId) == 0)
            {
				//return the book via pBook
                *pBook = bTemp;
				//set *piRBN to its actual RBN 
                *piRBN = iRBN+k;
				//return RC_OK
                return RC_OK;
            }
        }
		//If you have read for the maximum probes and it wasn’t found
		//return RC_REC_NOT_FOUND.
		//printf("%s",pBook -> szBookId);
		//both problems are here.
		printf("	Read loop ends");
		return RC_REC_NOT_FOUND;
	}
}

int vehicleInsert(HashFile *pHashFile, Vehicle *pVehicle)
{
    int i, rc, RBN;
    Vehicle rVehicle;

    RBN = hash(pVehicle->szVehicleId, pHashFile->hashHeader.iMaxHash);
    rc = readRec(&(*pHashFile), RBN, &rVehicle);
    if(rc == RC_LOC_NOT_FOUND || rVehicle.szVehicleId[0] == '\0')
    {
        writeRec(&(*pHashFile), RBN, &(*pVehicle));
    }
    else if(rc == RC_OK && (strcmp(pVehicle->szVehicleId, rVehicle.szVehicleId)==0))
    {
        return RC_REC_EXISTS;
    }else
    {
        //synonym code goes here
        int nRBN = -1;  // RBN for inserting synonym
                        // if still -1 when loop ends then no open spots
        for(i=1;i<pHashFile->hashHeader.iMaxProbe;i++)
        {
            rc = readRec(&(*pHashFile), RBN+i, &rVehicle);
            if(rVehicle.szVehicleId[0] == '\0')
            {
               if(nRBN == -1)
               {
                  nRBN = RBN+i;
               }
            }else if(rc == RC_OK && strcmp(pVehicle->szVehicleId, rVehicle.szVehicleId)==0)
            {
                return RC_REC_EXISTS;
            }
        }
        if(nRBN != -1)
        {
            writeRec(&(*pHashFile), nRBN, &(*pVehicle));
        }else
        {
            return RC_TOO_MANY_COLLISIONS;
        }

    }
}

int insertBook(HashFile *pHashFile, Book *pBook){
	//We use a probing K value of 1.  
	int k;
	int iRBN;
	int nRBN;
	int readCheck;
	
	//Determine the RBN using the driver's hash function.
	iRBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
	
	// no pointer just plain Book
    Book bTemp;
	
	//Use readRec to read a record at that RBN.  
    readCheck = readRec(pHashFile, iRBN, &bTemp);
	//If that location doesn't exist or the record at that location has a szBookId[0] == '\0':
    if(readCheck == RC_LOC_NOT_FOUND || bTemp.szBookId[0] == '\0'){
        //Write the new  book record (using pBook), w/writeRec
		writeRec(pHashFile, iRBN, pBook);
        return RC_OK;
    }
	//If that record exists and that book's szBookId matches pBook->szBookId,
    if(strcmp(bTemp.szBookId, pBook->szBookId)==0){
		//return RC_REC_EXISTS
        return RC_REC_EXISTS;
    }
	//Pgm 2 Start :
	//Otherwise, it is a synonym to the book in the hashed location
	//Limit the probing to pHashFile->hashHeader.iMaxProbes.
	nRBN = -1;
    for(k=1;k<pHashFile->hashHeader.iMaxProbes;k++){
		
		//we will not probe past the header record’s iMaxOvRBN.
		if(iRBN+k > pHashFile->hashHeader.iMaxOvRBN)
            {
				//return RC_TOO_MANY_COLLISIONS.
                return RC_TOO_MANY_COLLISIONS;
            }
		
		readCheck = readRec(pHashFile, iRBN+k, &bTemp);
		//if empty slot, record at location = '\0'
        if(bTemp.szBookId[0] == '\0')
        {
			if(nRBN == -1)
            {
				nRBN = iRBN+k;
            }
        }else 
			if(readCheck == RC_OK && strcmp(pBook->szBookId, bTemp.szBookId)==0)
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

int readRec(HashFile *pHashFile, int iRBN, void *pRecord){
	Book bTemp;
	long RBA = 0;
    RBA = iRBN * pHashFile->hashHeader.iRecSize;

    fseek(pHashFile->pFile, RBA, SEEK_SET);
	*(Book*)pRecord = bTemp;
    if (fread(pRecord, pHashFile->hashHeader.iRecSize, 1L, pHashFile->pFile) != 1 || bTemp.szBookId[0] == '\0'){
        return RC_LOC_NOT_FOUND;
	}
    return RC_OK;
}

int readRec(HashFile *pHashFile, int iRBN, void *pRecord)
{
    Book bTemp;

    int rcFseek, RecSize;
    long lRBA, rc;

    RecSize = pHashFile->hashHeader.iRecSize;               // size for one Record
    lRBA = iRBN * RecSize;                                  // determines the offset
    rcFseek = fseek(pHashFile->pFile, lRBA, SEEK_SET);      // fseek to the position in the file
    assert(rcFseek == 0);


    rc = fread(&bTemp ,RecSize, 1L, pHashFile->pFile);
    *(Book*) pRecord = bTemp;
    if(rc != 1 || bTemp.szBookId[0] == '\0'){
        return RC_LOC_NOT_FOUND;
    }else
    {
        return RC_OK;
    }

}

int readBook(HashFile *pHashFile, Book *pBook, int *piRBN){
    Book bTemp;
	int iRBN;
	int k;
	k = 1;

    iRBN = hash(pBook->szBookId, pHashFile->hashHeader.iMaxPrimary);
    readRec(pHashFile, iRBN, &bTemp);
    if(strcmp(pBook->szBookId, bTemp.szBookId) == 0)
    {
        *pBook = bTemp;
        *piRBN = iRBN;
        return RC_OK;
    }else
    {
        //synonym code goes here
        while(k<pHashFile->hashHeader.iMaxProbes)
        {
            if(iRBN+k > pHashFile->hashHeader.iMaxPrimary)
            {
                return RC_REC_NOT_FOUND; // went past max amount of records
            }

            readRec(pHashFile, iRBN+k, &bTemp);

            if(strcmp(pBook->szBookId, bTemp.szBookId) == 0)
            {
                *pBook = bTemp;
                *piRBN = iRBN+k;
                return RC_OK;
            }
		k++;
        }
        // if loop ends max probes reached
        return RC_REC_NOT_FOUND;
    }
}








