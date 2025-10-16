#ifndef MEM_H
#define MEM_H
#include "functions.h"

  #define HEAPSIZE  (8*1024*1024)
  #define MINBLOCK 32

#define MAXNODES ((HEAPSIZE / MINBLOCK) * 2 - 1)

static uint8*  HEAP[HEAPSIZE];
//#define HEAP 0x100000
#define HEAPSTART  (&HEAP)

enum NodeState {
    FREE,
    SPLIT,
    USED
};

typedef struct BuddyNode {
    NodeState state;
    uint32 size;         // tamaño del bloque
    void* addr;       // pos heap
    BuddyNode* left;
    BuddyNode* right;
} BuddyNode;





class BuddyAllocator{
  public:
    BuddyAllocator();

    void init(void * start,uint32 heapS,BuddyNode* pool,uint32* count);
    
    void buddySplit(BuddyNode* N);

    void* buddyAlloc(BuddyNode* N, uint32 size);

    bool buddyFree(BuddyNode* N,void* ptr);

    void* balloc(uint32 size);

    bool bfree(void* ptr);

    private:
    BuddyNode* rootBuddyAllocator;
    void* heapStart;
    BuddyNode* newPool;
    uint32* newCount;
    uint32 heapSize;
    uint32 maxOrder;

};

void initAlloc();
void* kalloc(uint32 size);
bool kfree(void* ptr);


void initPAlloc();
void* palloc(uint32 size);
bool pfree(void* ptr);

#endif 

