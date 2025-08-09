#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "utilities.h"

#define enterKeyCode 128
#define   retKeyCode 129
#define  downKeyCode 130
#define  leftKeyCode 131
#define    upKeyCode 132
#define rightKeyCode 133
#define   escKeyCode 135

#define keyCode_ESC   1
#define keyCode_ENTER 28
#define keyCode_TAB   14
#define keyCode_DOWN  80
#define keyCode_UP    72
#define keyCode_LEFT  75
#define keyCode_RIGHT 77

#define keyCode_Q 16
#define keyCode_W 17
#define keyCode_E 18
#define keyCode_R 19
#define keyCode_T 20
#define keyCode_Y 21
#define keyCode_U 22
#define keyCode_I 23
#define keyCode_O 24
#define keyCode_P 25

#define keyCode_A 30
#define keyCode_S 31
#define keyCode_D 32
#define keyCode_F 33
#define keyCode_G 34
#define keyCode_H 35
#define keyCode_J 36
#define keyCode_K 37
#define keyCode_L 38

#define keyCode_Z 44
#define keyCode_X 45
#define keyCode_C 46
#define keyCode_V 47
#define keyCode_B 48
#define keyCode_N 49
#define keyCode_M 50


void keyboard_irq_handler(struct regs *r);
void initKeyboard();
uint8 getLastAsciiKey();
uint8 getLastKeyCode();
bool isKeyBuffered();
bool isKeyPressed(uint8 key);
void eatKeyBuffered();
void eatBuffer();
#endif