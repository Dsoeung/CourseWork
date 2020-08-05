/**********************************************************************
cs3723p1.c by Darin Soeung
Purpose:	
    This program demonstrates Storage Management software responsible 
    for garbage collection.  It places references to data in a hash 
    table.
Command Parameters:
    Parameters located in cs3723p1Driver.c
	p1 < infile > outfile
    The stream input file contains commands.  There are commands 
    which specify comments, allocating memory, dereferencing memory, 
    associating/disaccociating nodes to other nodes, and printing 
    nodes.
	
	Commands in the input file:
    
    * comment       
        This is just a comment which helps explain what is being tested.
    
    ALLOC key nodeTypeNm val1, val2, ...
        Establish a user data node of the specified type.  The data (val1, val2, ...) will be 
        placed in the user data node.  This command invokes gcAllocate which returns a user data 
        pointer to allocated memory.  Key is used as an entry in a Hash Table which has a 
        corresponding value set to the user data pointer.
    ASSOC keyFrom attrName keyTo
        The user data for the specified keyFrom will be changed to point to the pointer
        referenced by keyTo.  Note that keyTo might be NULL, causing the attribute to 
        change to NULL.
    DEBUG dumpFlag printNodeFlag
        This is useful to help only show dump output during testing of the garbage collection.
        Values for dumpFlag:
            R - regular, only shows DUMP output when the DUMP command is encountered.
            G - only show DUMP steps within the garbage collection function after each phase.
         Values for printNodeFlag:
            A - show node info and atributes
            N - do not show attributes            
    DEREF key 
        It sets the value to NULL for the specfied key in the hash table.
    DUMP 
        Dumps all the nodes in the memory.
    GCOLL
        Causes the subphases of garbage collection to be invoked.
        Mark Subphase       Walk through all memory from the beginning and mark 
                            all nodes to a cGC of 'C' (candidate).  It is easier 
                            to combine adjacent free areas if all nodes are 
                            marked as 'C'.
        Follow Subphase     From any valid starting point (which we are simulating 
                            with a hash table), follow the nodes based on metadata 
                            and mark each referenceable node as 'I (in use).
        Collection Subphase We will build the free list from the entire memory.  
                            (We are affectively ignoring the old free linked list.)  
                            Walk through all the memory looking for 'C' nodes, combining 
                            adjacent ones into a single free area.  Each free area 
                            will be placed on the front of the new free list.
    PRTALL
        Prints all nodes in the memory.
    PRTNODE key
        Invokes printNode for the node referenced by the specified key.
		
Input:
    The program will take inputs from an input.txt file
Results:
    This program manages memory and implements garbage collection.
Returns:
    Contains only void functions
Notes:

**********************************************************************/

/**********************************************************************
cs3723p1.h
Purpose:
	Defines constants for
		MAX_KEY_SIZE
		MAX_MESSAGE_SIZE
		MAX_STRING
		MAX_NODE_TYPE
		MAX_NODE_ATTR
		MAX_DATA_SZ
		MAX_HASH_ENTRIES 
		NOT_FOUND 
		RC_NOT_AVAIL
		RC_INVALID_ADDR
		RC_ASSOC_ATTR_NOT_PTR 
		RC_ASSOC_ATTR_NOT_FOUND
	Defines typedefs for
		MetaAttr
		NodeType
		InUseNode
		FreeNode
		GCManager
		GCResult
		HashEntryPair
		HashMO
	as well as containing function prototypes for all present functions.
	
Notes:

**********************************************************************/
#include "cs3723p1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/******************** gcAllocate **********************************
void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult) 
Purpose:
    This is part of the Usage Phase. This is satisfying a memory allocation 
	request using the first node from the free list that is big enough.  
Parameters:
	I	char sbData[]		The user's data
	I/O GCManager *pMgr   	Provides metadata about the user data and
							information for storage management.
	O GCResult *pgcResult   Result structure for returning errors
	I	short shDataSize	Size of user data.
	I	short shNodeType	The user's node type. 
Notes:
	We will ALWAYS add to the FRONT of the free list.
	NODE_OVERHEAD_SZ = (sizeof(short)+sizeof(short)+1)
**************************************************************************/

void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult){
   int iAllocate;			//Size to allocate
   FreeNode *pFreeCurr;		//Current free node
   FreeNode *pFreePrev;		//Previous free node in list
   InUseNode *pReturnNode;	//Initialize InUseNode pointer we'll use to return the allocated memory
   
   //How much do we allocate?  the user data size plus ?
   //the user data size plus overhead size! (NODE_OVERHEAD_SZ)
    iAllocate = shDataSize + NODE_OVERHEAD_SZ;
   
   //Initialize free nodes for free list parsing
   pFreeCurr = pMgr->pFreeHead;
   pFreePrev = NULL;
   
   //While pFreeCurr is not NULL check for free node with enough memory to satisfy the needed size. (iAllocate)
   while (pFreeCurr != NULL)
   {
      //if Current node has enough memory break
	  if (pFreeCurr->shNodeSize >= iAllocate)
      {
         break;
      }
	  //Iterate through given GCManager (pMgr) free list
      pFreePrev = pFreeCurr;
	  pFreeCurr = pFreeCurr->pFreeNext;
   }

   //If the while loop runs full and there is no node with enough memory
   //Set pGCResult->rc to RC_NOT_AVAIL. (cs3723p1.h has pgcResult so we'll keep that)
   //Copy an error message to pGCResult->szErrorMessage.
   //Functionally return NULL.
   if (pFreeCurr == NULL)
   {
      pgcResult->rc = RC_NOT_AVAIL;
	  strcpy(pgcResult->szErrorMessage, "There isn't a large enough free node to satisfy the request\n");
	  return NULL;
	}

	//Remove that free node from the free list, updating the free list.  
	//Do this regardless of whether there is enough free space left in the free node to make a new free node
   if (pFreePrev == NULL)
   {
      pMgr->pFreeHead = pFreeCurr->pFreeNext;
   }
   if (pFreePrev != NULL)
   {
      //if previous node is not NULL, then remove pFreeCurr from list 
      pFreePrev->pFreeNext = pFreeCurr->pFreeNext;
   }
   /*
   Determine whether there is enough remaining space in the free node to
   make that remaining space a new free node (see pMgr->iMininumNodeSize).  
   If there isn't enough space left, change the allocated size to be the free node's size.
   Otherwise, add the new free node (which uses the remaining space) to the free list.
   (We will ALWAYS add to the FRONT of the free list.) 
   */
   
   if ((pFreeCurr->shNodeSize - iAllocate) >= pMgr->iMinimumNodeSize)
   {
      //add the new free node (which uses the remaining space) to the free list.
      FreeNode *pFree; 
	  short shRemainder;
	  shRemainder = pFreeCurr->shNodeSize - iAllocate;
      pFree = (FreeNode *)((char *)pFreeCurr + iAllocate);
      pFree->cGC = 'F';
      pFree->shNodeSize = shRemainder;
      pFree->pFreeNext = pMgr->pFreeHead;
      pMgr->pFreeHead = pFree;
   }
   
   //If there isn't enough space left, change the allocated size to be the free node's size.
   if ((pFreeCurr->shNodeSize - iAllocate) <= pMgr->iMinimumNodeSize)
   {
      iAllocate = pFreeCurr->shNodeSize;
   }
	
	
   pReturnNode = (InUseNode *)pFreeCurr;
   //Set its cGc to 'I'.
   pReturnNode->cGC = 'I';
   //Set its node type.
   pReturnNode->shNodeType = shNodeType;
   //Set its size.
   pReturnNode->shNodeSize = iAllocate;
   //Set its sbData.
   memcpy(pReturnNode->sbData, sbData, shDataSize);
   
   pReturnNode = (InUseNode *)((char *)pReturnNode + NODE_OVERHEAD_SZ);

   return pReturnNode;
   	
}


/******************** gcInit **********************************************
void gcInit(GCManager *pMgr) 
Purpose:
    Beginning of the Init phase, 
	Initialize the memory in pMgr to one huge free node.
Parameters:
    I/O GCManager *pMgr		GCManager stucture passed in, modify pFreeHead. 
Notes:
    The driver already set pMgr->pBeginStorage to the address of the heap.
	You need to set pMgr->pFreeHead to point to the starting address of that huge node.
**************************************************************************/

void gcInit(GCManager *pMgr){
   //Driver sets first four variables of *pMgr,
   //iHeapSize, iMinimumNodeSize, *pBeginStorage & pEndStorage.
   FreeNode *pFree;
   
   //Zero out the entire heap.
   memset(pMgr->pBeginStorage, 0, pMgr->iHeapSize);
   
   //Initialize the memory to one huge free node.
   pFree = (FreeNode *) pMgr->pBeginStorage;
   
   //Set pMgr->pFreeHead to starting address of that huge node.
   pMgr->pFreeHead = pFree;
   
   //Set the cGC to 'F' (pFreeHead)
   pMgr->pFreeHead->cGC = 'F';
   
   //Set the shNodeSize (pFreeHead)
   pMgr->pFreeHead->shNodeSize = pMgr->iHeapSize;
}

/******************** gcMark ********************************************
void gcMark(GCManager *pMgr, GCResult *pgcResult)
Purpose:
    Searches the address book for the specified name to return the location 
    (i.e., subscript) of that name.
Parameters:
	I GCManager *pMgr    	Provides metadata about the user data and
							information for storage management.
    O GCResult *pgcResult   Result structure for returning errors
Notes:
    
Return Value:
    -1    Name not found
    >=0   Subscript of where the name was found in the Address Book.
**********************************************************************/
/*
This is the first subphase of Garbage Collection.  
Beginning with the start of the heap, sequentially mark every node as 'C'.
Look at the driver's drDump function to see how to advance through the heap touching every adjacent node.
*/

void gcMark(GCManager *pMgr, GCResult *pgcResult){
   char *pCh;			//Char pointer
   short shTempSize;
   InUseNode *pAlloc;

   //drDump function heap iterator, exactly the same
   //cs3723p1Driver.c line 632
   for (pCh = pMgr->pBeginStorage; pCh < pMgr->pEndStorage; pCh += shTempSize)
   {
      pAlloc = (InUseNode *)pCh;
      shTempSize = pAlloc->shNodeSize;
      //sequentially mark every node as 'C'.
      pAlloc->cGC = 'C';
   }
   
}


/******************** gcFollow ********************************************
void gcMark(GCManager *pMgr, GCResult *pgcResult)
Purpose:
    Searches the address book for the specified name to return the location 
    (i.e., subscript) of that name.
Parameters:
    I GCManager *pMgr    	Provides metadata about the user data and
							information for storage management.
    I void *pUserData       Given user data.
    O GCResult *pgcResult   Result strcuture for returning errors.
Notes:
Look at cs3723p1Driver.c function, setData
hint: void **ppNode = (void **)&(pInUseFrom->sbData[pAttr->shOffset]);
Return Value:
    -1    Name not found
    >=0   Subscript of where the name was found in the Address Book.
**********************************************************************/
void gcFollow(GCManager *pMgr, void *pUserData, GCResult *pgcResult){		
   int iAt;				// index for metaAttrM
   InUseNode *pInUse;	// InUseNode used to hold data from pUserDataFrom
   MetaAttr *pAttr;		// slightly simplifies referencing item in metaAttrM
   void **ppNode;		// pointer into user data if this attribute is a pointer

   pInUse = (InUseNode *)((char *)pUserData - NODE_OVERHEAD_SZ);

   //If the current node's cGC is 'I', it has already been followed; 
   if (pUserData == NULL || pInUse->cGC == 'I')
   {
      //current nodes cGC is I therefore, return.
      return;
   }
   
   // Loop through each of the user data's attributes.  The subscripts start with
   // shBeginMetaAttr from nodeTypeM and end when the corresponding metaAttr's node type is
   // different.
   //(pCh = pMgr->pBeginStorage; pCh < pMgr->pEndStorage; pCh += shTempSize)
   //Meta data is in node type and GCManager
   pInUse->cGC = 'I';
   //From SetData in Driver
   //for (iAt = pMgr->nodeTypeM[shNodeType].shBeginMetaAttr; pMgr->metaAttrM[iAt].shNodeType == shNodeType; iAt++)
   for (iAt = pMgr->nodeTypeM[pInUse->shNodeType].shBeginMetaAttr; pMgr->metaAttrM[iAt].shNodeType == pInUse->shNodeType; iAt++)
   {
      pAttr = &(pMgr->metaAttrM[iAt]);
	  //No need for cases since we're just lookin for P
      if(pAttr->cDataType == 'P')
      {
		 //HINT!
         ppNode = (void **) &(pInUse->sbData[pAttr->shOffset]);
		 //Recursively follow each non-null pointer referenced by this node. 
         gcFollow(pMgr, *ppNode, pgcResult); 
      }
   } 
}


/******************** gcCollect ********************************************
void gcCollect(GCManager *pMgr, GCResult *pgcResult)
Purpose:
    Searches the address book for the specified name to return the location 
    (i.e., subscript) of that name.
Parameters:
    I GCManager *pMgr   Provides metadata about the user data and
                        information for storage management.
    O GCResult *pgcResult   Result strcuture for returning errors
Notes:
As you collect free space, call one of these two driver functions:
drPrintCollecting(pMgr, pCandidate);
drPrintCombining(pMgr, pPrecedes, pCandidate);
lots of code came from drDump in cs3723p1Driver.
    
Return Value:
    -1    Name not found
    >=0   Subscript of where the name was found in the Address Book.
**********************************************************************/
void gcCollect(GCManager *pMgr, GCResult *pgcResult){
	char *pCh;				//Char pointer
	FreeNode *pFreeCurr;	//Current free node passed
	FreeNode *pFreePrev;	//Previous free node in heap
    short shTempSize;		//Temporary size when iterating
    short shCombineSize;		//Combined size of nodes
    
	
    pMgr->pFreeHead = NULL;
	pFreeCurr = NULL;
	pFreePrev = NULL;
	
	//you will sequentially traverse the heap
	//Taken from drDump	
    for (pCh = pMgr->pBeginStorage; pCh < pMgr->pEndStorage; pCh += shTempSize)
    {
		//New iter so we set combine to 0 until then.
        shCombineSize = 0;
		//pFreeCurr to pMgr(pBeginStorage)
        pFreeCurr = (FreeNode *)pCh;
		//set temp size to size of pFreeCurr Node size
        shTempSize = pFreeCurr->shNodeSize;
		
		
		//If pFreeCurr is a candidate to free
		//no need for cases like in drDump only looking for 'C'
        if (pFreeCurr->cGC == 'C')
        {
			//indicate node is being collected !
			drPrintCollecting(pMgr, pFreeCurr);
            // Dollecting the 'C' nodes and starting a combine
			// In case the next node is also a Candidate
            shCombineSize = pFreeCurr->shNodeSize;
			//Since pFreeCurr is a Candidate we'll make it
			//pFreePrev to check if the next node is a candidate also
			//in case of combining.
            pFreePrev = pFreeCurr;
            // Touch adjecent node
            pFreeCurr = (FreeNode *)((char *)pFreeCurr + pFreeCurr->shNodeSize);

            // while the current node is a candidate
            // and while we haven't reached the end of the heap
            while (pFreeCurr->cGC == 'C')
            {
			   // indicate nodes are being combined !
               drPrintCombining(pMgr, pFreePrev, pFreeCurr);
               // to scale iterate through the heap
			   // we must add the current node for displacement.
               shTempSize += pFreeCurr->shNodeSize;
               // add node size to total
               shCombineSize += pFreeCurr->shNodeSize;
               // iter next node
               pFreeCurr = (FreeNode *)((char *)pFreeCurr + pFreeCurr->shNodeSize);
            }
			
			//iterate through, leaving frees behind
             pFreePrev->cGC = 'F';
			 
			 //ending size
             pFreePrev->shNodeSize = shCombineSize;

             // Each insertion will be to the FRONT of the free list
             pFreePrev->pFreeNext = pMgr->pFreeHead;
             pMgr->pFreeHead = pFreePrev;
        }
    }
}


/******************** gcAssoc ********************************************
void gcAssoc(GCManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, GCResult *pgcResult)
Purpose:
    Sets a user pointer in the specified user data node to a new referenced user data node.
Parameters:
    I GCManager *pMgr   	Provides metadata about the user data and
							information for storage management.
    O GCResult *pgcResult   Result strcuture for returning errors.
    I void *pUserDataFrom	Pointer to specified user data node.
	O void *pUserDataTo  	Pointer new user data node.
	I char szAttrName[]		String containing Attribute name.
	
Notes:
	As you collect free space, call one of these two driver functions:
	#define ULAddr(addr) ((unsigned long) addr)
	#define ULAddr(addr) (((unsigned long) addr)&0x00000000FFFFFFFF)
	Hint:  once you know the offset in the user data:
	void **ppNode = (void **)&(pInUseFrom->sbData[pAttr->shOffset]);
	*ppNode = pUserDataTo;

**********************************************************************/
void gcAssoc(GCManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, GCResult *pgcResult){
   int iAt;				// index for metaAttrM
   int iFound = 0;		// check if specific attribute name is found
   InUseNode *pInUse;	// InUseNode used to hold data from pUserDataFrom
   MetaAttr *pAttr;		// slightly simplifies referencing item in metaAttrM
   void **ppNode;		// pointer into user data if this attribute is a pointer

   //set inUseNode to user data, don't forget overhead.
   pInUse = (InUseNode *)((char *)pUserDataFrom - NODE_OVERHEAD_SZ);
   
   for (iAt = pMgr->nodeTypeM[pInUse->shNodeType].shBeginMetaAttr; pMgr->metaAttrM[iAt].shNodeType == pInUse->shNodeType; iAt++)
   {
      pAttr = &(pMgr->metaAttrM[iAt]);
      //Search for the specified attribute name in the meta data for the from node.
	  if (strcmp(pAttr->szAttrName, szAttrName) == 0)
      {
		 //Found!
         iFound = 1;
		 //if data type is not pointer
		 if (pAttr->cDataType != 'P')
         {           
			strcpy(pgcResult->szErrorMessage, szAttrName);
			strcat(pgcResult->szErrorMessage, " <- Specified attribute is not a pointer.\n");
			//If the specified attribute is not a pointer, set the pGCResult->rc to RC_ASSOC_ATTR_NOT_PTR
			pgcResult->rc = RC_ASSOC_ATTR_NOT_PTR;
			return;
         }
		 //HINT!
         ppNode = (void **)&(pInUse->sbData[pAttr->shOffset]);
         *ppNode = pUserDataTo;             
      }
   }
       
   //szAttrName not found.
   if (iFound == 0)
   {
	  //provide an error message that contains the attribute name, and return.
	  strcpy(pgcResult->szErrorMessage, szAttrName);
	  strcat(pgcResult->szErrorMessage, " <- Specified attribute name not found.\n");
	  
	  //set the pGCResult->rc to RC_ASSOC_ATTR_NOT_FOUND
	  pgcResult->rc = RC_ASSOC_ATTR_NOT_FOUND;
	  return; 
   }
}





















