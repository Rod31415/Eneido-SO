#ifndef IDT_H
#define IDT_H
#include "types.h"
#include "utilities.h"
#include "functions.h"
#include "vga.h"
#include "rtl8139.h"
#include "keyboard.h"
#include "pit.h"
#include "mouse.h"
#include "syscall.h"
#include "ctx.h"
#include "term.h"

void idt_install();
void safe_irq_init();

#endif
