#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING 30                       // Maximum size of strings like 
                                            // node type names, attribute names
#define MAX_NODE_TYPE 5	                    // Maximum number of node types 
#define MAX_NODE_ATTR 50                    // Maximum number of combined node attr

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

void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult){
   
   
}


int main(){
    printf("OK");
}