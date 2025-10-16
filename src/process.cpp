#include "headers/process.h"

typedef enum{
  PROCESS_NEW=1,
  PROCESS_READY,
  PROCESS_RUNNING,
  PROCESS_WAITING,
  PROCESS_TERMINATED
} proc_state;

typedef struct {
    uint32 edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;
    uint32 eip, cs, eflags, useresp, ss;
} cpu_context;

typedef struct {
    uint32 code_start;
    uint32 code_size;
    uint32 stack_top;
    uint32 heap_start;
    uint32 heap_end;
} memory_layout;

typedef struct process_t{
    uint32 pid;
    proc_state state;

    uint32 *page_directory;
    memory_layout mem;

    cpu_context context;

    uint32 kernel_stack;

    uint32 priority;
    struct process_t *next;
    struct process_t *prev;
};


#define MAX_PROCESSES 32

static process_t Process_Table[MAX_PROCESSES];
static int next_pid=1;

process_t* processHead=nullptr;
process_t* processTail=nullptr;

extern "C" uint8* stack_top;


process_t* AllocProcessStruct(){
  for(uint32 i=0;i<MAX_PROCESSES;i++){
      if(Process_Table[i].state==PROCESS_TERMINATED||Process_Table[i].state==0){
        Process_Table[i].pid=next_pid++;
        Process_Table[i].state=PROCESS_NEW;
        return &Process_Table[i];
      }
  }
  return nullptr;
}

void AddProcess(process_t *p){
  p->next=nullptr;
  p->prev=nullptr;

  if(!processHead){
    processHead=p;
    processTail=p;
  }else{
    processTail->next=p;
    p->prev=processTail;
    processTail=p;
  }
}
