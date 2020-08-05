typedef struct pagetable
{
    unsigned int fnum;
    unsigned int vi;
} PageTable;

void initializePT(PageTable *PT, unsigned int entries);