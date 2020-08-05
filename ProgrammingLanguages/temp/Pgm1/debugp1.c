#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING 30                       // Maximum size of strings like 
                                            // node type names, attribute names
#define MAX_NODE_TYPE 5	                    // Maximum number of node types 
#define MAX_NODE_ATTR 50                    // Maximum number of combined node attr
#define MAX_MESSAGE_SIZE 100 
#define MAX_DATA_SZ 500                     // Maximum size of sbData
#define NODE_OVERHEAD_SZ (sizeof(short)+sizeof(short)+1)

#define RC_NOT_AVAIL 901            // There isn't any free memory to handle alloc request
#define RC_INVALID_ADDR 903         // Invalid address which isn't within heap
#define RC_ASSOC_ATTR_NOT_PTR 801   // Attribute name for ASSOC not a pointer attribute
#define RC_ASSOC_ATTR_NOT_FOUND 802 // Attribute name for ASSOC not found for the from node

typedef struct MetaAttr
{
    short  shNodeType;                      // Type of node
    char   szAttrName[MAX_STRING+1];        // Name of the attribute
    char   cDataType;                       // Data type: S - char string, P - Ptr, D - double, I - int
    short  shSizeBytes;                     // size in bytes including zero byte for strings
    short  shOffset;                        // Within the data portion, this is the offset to this attribute
}MetaAttr;
// NodeType describes one type of node
typedef struct NodeType
{
	char szNodeTypeNm[MAX_STRING+1];
	short shBeginMetaAttr;              // Subscript in metaAttrM of first attribute for
                                        // this node type.
	short shNodeTotalSize;
}NodeType;


typedef struct FreeNode
{
    short shNodeSize;                   // Total size of this free node.
    short shNodeType;                   // Not used
    char  cGC;                          // Garbage Collection status byte has one of these
                                        // values:  F - free, C - candidate to free,
                                        //          I - in use
    struct FreeNode *pFreeNext;         // Points to next free node
} FreeNode;

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

typedef struct
{
    int rc;                                // Return Code is 0 if it is normal.  Otheriwise,
                                           // it is not zero.  See the defined constants.
    char szErrorMessage[MAX_MESSAGE_SIZE + 1];  // If a problem is encountered, this should 
                                                // explain the error.
} GCResult;

typedef struct InUseNode
{
    short shNodeSize;                   // total size of the allocated item.  
    short shNodeType;                   // Node Type subscript.	
    char  cGC;                          // Garbage Collection status byte has one of these
                                        // values:  F - free, C - candidate to free,
                                        //          I - in use
    char  sbData[MAX_DATA_SZ];          // This is the user's data in the node.  It might
                                        // be bigger than MAX_STRING.
} InUseNode;

/******************** gcAllocate **********************************
void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult) 
Purpose:
    Explain what the function does including a brief overview of what it
    returns.
Parameters:
	I/O	GCManager *pMgr			GCManager stucture passed in, modify pFreeHead.
	I	short shDataSize		User data size
	I	short shNodeType		Node type
	I	char sbData[]			User's data
	O	GCResult *pgcResult		mm function status return, 0 for normal.
Notes:
	We will ALWAYS add to the FRONT of the free list.
	NODE_OVERHEAD_SZ = (sizeof(short)+sizeof(short)+1)
**************************************************************************/

void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult){
   
   //Initialize free nodes for free list parsing
   FreeNode *pFreeCurr = pMgr->pFreeHead;
   FreeNode *pFreePrev = NULL;
   
   //How much do we allocate?  the user data size plus ?
   //the user data size plus overhead size! (NODE_OVERHEAD_SZ)
   int iAllocate = shDataSize + NODE_OVERHEAD_SZ;
   
   //Initialize InUseNode pointer we'll use to return the allocated memory
   InUseNode *pReturnNode;
   
   //FreeNode *pParent = pMgr->pFreeHead;
   //int minSize = pMgr->iMinimumNodeSize;
   


   //While pFreeCurr is not NULL check for free node with enough memory to satisfy the needed size. (iAllocate)
   while (pFreeCurr != NULL){
      //if Current node has enough memory break
	  if (pFreeCurr->shNodeSize >= iAllocate){
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
   if (pFreeCurr == NULL){
      pgcResult->rc = RC_NOT_AVAIL;
	  strcpy(pgcResult->szErrorMessage, "There isn't a large enough free node to satisfy the request\n");
	  return NULL;
	}

	//Remove that free node from the free list, updating the free list.  
	//Do this regardless of whether there is enough free space left in the free node to make a new free node
   if (pFreePrev == NULL){
      pMgr->pFreeHead = pFreeCurr->pFreeNext;
   }
   if (pFreePrev != NULL){
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
   
   if ((pFreeCurr->shNodeSize - iAllocate) >= pMgr->iMinimumNodeSize){
      //add the new free node (which uses the remaining space) to the free list.
      short shRemainder = pFreeCurr->shNodeSize - iAllocate;
      FreeNode *pNewFree = (FreeNode *)((char *)pFreeCurr + iAllocate);
      pNewFree->cGC = 'F';
      pNewFree->shNodeSize = shRemainder;
      pNewFree->pFreeNext = pMgr->pFreeHead;
      pMgr->pFreeHead = pNewFree;
   }
   
   //If there isn't enough space left, change the allocated size to be the free node's size.
   if ((pFreeCurr->shNodeSize - iAllocate) < pMgr->iMinimumNodeSize){
	   
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
   
   //difCha
   pReturnNode = (InUseNode *)((char *)pReturnNode + NODE_OVERHEAD_SZ);
   pgcResult->rc = 0;

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
   FreeNode *nTemp;
   
   //Zero out the entire heap.
   memset(pMgr->pBeginStorage, '\0', pMgr->iHeapSize);
   
   //Initialize the memory to one huge free node.
   nTemp = (FreeNode *) pMgr->pBeginStorage;
   
   //Set pMgr->pFreeHead to starting address of that huge node.
   pMgr->pFreeHead = nTemp;
   
   //Set the cGC to 'F' (pFreeHead)
   pMgr->pFreeHead->cGC = 'F';
   
   //Set the shNodeSize (pFreeHead)
   pMgr->pFreeHead->shNodeSize = pMgr->iHeapSize;
}
int main(){
    printf("OK");
}