#include "headers/pit.h"



void pitInit(){
    uint32 divisor=1193;

    outport(0x43,0x36);
    outport(0x40,divisor&0xFF);
    outport(0x40,(divisor>>8));
}


volatile unsigned long timer_ticks=0;



void sleep(uint32 ms){
    unsigned long end=timer_ticks+ms;
    while(timer_ticks<end){
        __asm__ __volatile__("nop");
    }
}

void timer_irq_handler(struct regs *r){
    timer_ticks++;
}