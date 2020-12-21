#ifndef _DRAW_UTILS_H
#define _DRAW_UTILS_H

#include "global.h"

void redraw(char *text[MAX_LINE_NO], unsigned long int scroll);
void draw_cursor(int act_line, int act_char, unsigned long int scroll);

#endif
