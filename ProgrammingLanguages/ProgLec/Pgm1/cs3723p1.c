/**********************************************************************
cs3723p1.c
    Copyright 2020 Larry Clark, this code may not be copied to any 
    other website.
Purpose:
    Defines constants for
        boolean values
        maximum sizes
        program return codes
        error messages
    Defines typedefs for
        MetaAttr - describes one attribute within a node type
        NodeType - describes one node type
        InUseNode - represents an allocated node that will be in use.
            The actual size of an InUse node is actually larger than
            what the user requested.  The size of an allocated 
            item cannot be less than the size of a FreeNode.
        FreeNode - represents a node that is on the free list.
        GCManager - the primary structure used by this program.
        GCResult - used by the mm... functions to 
            specify whether they executed successfully.
        HashEntryPair - hash key and value pair used in the HashMO
        HashMO - returned by getAll. This contains an array of 
            HashEntryPair and the number of entries in that array.
    Prototypes
Notes:
    
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs3723p1.h"

void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult){
   /*
   Searches for a free node with enough memory to satisfy the needed size.  
   If there isn't a large enough free node to satisfy the request:
   Set pGCResult->rc to RC_NOT_AVAIL.
   Copy an error message to pGCResult->szErrorMessage.
   Functionally return NULL.
   */
   
   //It is passed the user data size, shDataSize
   //(which is less than the size to allocate due to the node overhead of size, node type, and cGC),
   //the node type, and the user's data.  How much do we allocate?  the user data size plus (?)
   
}


/*
typedef struct
{
    int iHeapSize;                       // Total size of the heap memory being managed
    int iMinimumNodeSize;                // The minimum size of any node.
    char *pBeginStorage;                 // Beginning of the heap memory being managed
    char *pEndStorage;                   // End address immediately after the heap memory
    FreeNode *pFreeHead;                 // Head of the free list
    NodeType nodeTypeM[MAX_NODE_TYPE];   // array of node types
    MetaAttr metaAttrM[MAX_NODE_ATTR];   // array of attribute meta data
} GCManager;
*/

void gcInit(GCManager *pMgr){
   //Driver sets first four variables of *pMgr,
   //iHeapSize, iMinimumNodeSize, *pBeginStorage & pEndStorage.
   FreeNode *nTemp;
   
   //Zero out the entire heap.
   memset(pMgr->pBeginStorage, 0, pMgr->iHeapSize);
   
   //Initialize the memory to one huge free node.
   nTemp = (FreeNode *) pMgr->pBeginStorage;
   
   //Set pMgr->pFreeHead to starting address of that huge node.
   pMgr->pFreeHead = nTemp;
   
   //Set the cGC to 'F' (pFreeHead)
   pMgr->pFreeHead->cGC = 'F';
   
   //Set the shNodeSize (pFreeHead)
   pMgr->pFreeHead->shNodeSize = pMgr->iHeapSize;
}



void gcMark(GCManager *pMgr, GCResult *pgcResult);
void gcFollow(GCManager *pMgr, void *pUserData, GCResult *pgcResult);
void gcCollect(GCManager *pMgr, GCResult *pgcResult);
void gcAssoc(GCManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, GCResult *pgcResult);