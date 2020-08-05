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

/* student functions
void * gcAllocate(GCManager *pMgr, short shDataSize, short shNodeType, char sbData[], GCResult *pgcResult);
void gcInit(GCManager *pMgr);
void gcMark(GCManager *pMgr, GCResult *pgcResult);
void gcFollow(GCManager *pMgr, void *pUserData, GCResult *pgcResult);
void gcCollect(GCManager *pMgr, GCResult *pgcResult);
void gcAssoc(GCManager *pMgr, void *pUserDataFrom, char szAttrName[], void *pUserDataTo, GCResult *pgcResult);
 */
void test(){
    
        
}