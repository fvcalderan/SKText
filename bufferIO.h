#ifndef _BUFFERIO_H
#define _BUFFERIO_H

#include "global.h"

void free_text(char *text[MAX_LINE_NO]);
void load_buffer(char *text[MAX_LINE_NO], long unsigned int *act_line,
                 long unsigned int *act_char,
                 int argc, char **argv);
void save_buffer(char *text[MAX_LINE_NO], int argc, char **argv);

#endif
