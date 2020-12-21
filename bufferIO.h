#include "global.h"

#ifndef _BUFFERIO_H
#define _BUFFERIO_H
void load_buffer(char *text[MAX_LINE_NO], int *act_line, int *act_char,
                 int argc, char **argv);
void save_buffer(char *text[MAX_LINE_NO], int argc, char **argv);
#endif
