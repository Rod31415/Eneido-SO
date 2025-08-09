#ifndef PIT_H
#define PIT_H
#include "types.h"
#include "utilities.h"

void timer_irq_handler(struct regs *r);
void sleep(uint32 ms);
#endif