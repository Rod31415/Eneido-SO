#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "vfs.h"
#include "types.h"
#include "utilities.h"
#include "vga.h"

void initInterpreterByFile(DIR file,uint8 opt1,uint8 opt2);
void initInterpreterByArgv(int8* argv);


#endif
