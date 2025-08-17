#include "headers/mem.h"
BuddyNode nodePool[MAXNODES];
uint32 nodeCount=0;



static BuddyNode* createNode(void *addr,uint32 size){
  BuddyNode* n=&nodePool[nodeCount++];
  //printf(" Nodo :%d  ",nodeCount);
  n->state=FREE;
  n->size=size;
  n->addr=addr;
  n->left=nullptr;
  n->right=nullptr;
  return n;
}


BuddyAllocator::BuddyAllocator() {
      //heapStart=start;
      //rootBuddyAllocator=createNode(0,heapS);
    
      //nodeCount=0;

}
void BuddyAllocator::init(void * start,uint32 heapS){
      heapStart=(void*)HEAP;
      rootBuddyAllocator=createNode(0,HEAPSIZE);
    
      nodeCount=1;
}

void BuddyAllocator::buddySplit(BuddyNode* N){
      if(N->state!=FREE||N->size<=MINBLOCK)return;
      
      uint32 half=N->size/2;
      N->left=createNode(N->addr,half);
      N->right=createNode(N->addr+half,half);
}

void *BuddyAllocator::buddyAlloc(BuddyNode* N, uint32 size){

      if(!N||N->state==USED||size>N->size)return nullptr;

      
      if(N->state==FREE){
        
        if(N->size==size){
          N->state=USED;
          
          return ((void*)((uint32)heapStart+(uint32)N->addr));
        }
        if(N->size>size){

          if(N->left==nullptr && N->right==nullptr)
            buddySplit(N);
          //printf("SPLITED %d %d/n",N->left->size,N->right->size);
        //refresh();
          N->state=SPLIT;

        }else if(N->size<size){
          return nullptr;
        }
      }
      //while(1);
      if(N->state==SPLIT){
        //printf("left: %d right: %d| ",N->left->state,N->right->state);
        //refresh();
        void* ptr=buddyAlloc(N->left,size);
        if(ptr)return ptr;
        return buddyAlloc(N->right,size);
      }
      return nullptr;

}

bool BuddyAllocator::buddyFree(BuddyNode* N,void* ptr){
      if(!N)return false;

      if(N->state==USED){
        if((void*)((uint32)heapStart+(uint32)N->addr)==ptr){
          N->state=FREE;
        return true;
        }
        return false;
      }

      if(N->state==SPLIT){
        bool fleft=buddyFree(N->left,ptr);
        bool fright=false;
        if(!fleft)
          fright=buddyFree(N->right,ptr);

        if(fleft||fright){
          
          if(N->left->state==FREE&&N->right->state==FREE){
            N->state=FREE;
          }
          return true;
        }
        return false;        
      }
}

void* BuddyAllocator::balloc(uint32 size){
      uint32 real_size=MINBLOCK;
      while(real_size<size)real_size<<=1;
      return buddyAlloc(rootBuddyAllocator,real_size);
}

bool BuddyAllocator::bfree(void* ptr){
      return buddyFree(rootBuddyAllocator,ptr);
}

BuddyAllocator newalloc;

void initAlloc(){
  newalloc.init((void*)HEAPSTART,HEAPSIZE);
}

void* kalloc(uint32 size){

  newalloc.balloc(size);

}

bool kfree(void* ptr){

  newalloc.bfree(ptr);

}