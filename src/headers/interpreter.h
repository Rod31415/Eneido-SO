#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "vfs.h"
#include "types.h"
#include "utilities.h"

void initInterpreterByFile(DIR file);
void initInterpreterByArgv(int8* argv);


#endif
