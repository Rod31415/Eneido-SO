#ifndef TERM_H
#define TERM_H
#include "functions.h"
#include "utilities.h"
#include "multiboot.h"
#include "string.h"
#include "mem.h"
#include "vfs.h"
#include "diskDriver.h"
#include "editor.h"
#include "games.h"
#include "rtc.h"
#include "pci.h"
#include "interpreter.h"

void init_term(multiboot_info *mboot);
void new_line_term();
void loop_term();
#endif
