#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "utilities.h"

#define   enterKeyCode 128
#define   retKeyCode 129
#define  downKeyCode 130
#define  leftKeyCode 131
#define    upKeyCode 132
#define rightKeyCode 133
#define   escKeyCode 135

/*struct regs
{
    unsigned int gs, fs, es, ds;      
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  
    unsigned int int_no, err_code;    
    unsigned int eip, cs, eflags, useresp, ss;  
};*/

void keyboard_irq_handler(struct regs *r);

uint8 getLastAsciiKey();
uint8 getLastKeyCode();
uint8 isKeyPressed();
void eatKeyBuffered();

#endif