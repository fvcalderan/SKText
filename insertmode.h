#ifndef _INSERTMODE_H
#define _INSERTMODE_H

#include "global.h"
#include "bufferIO.h"
#include "draw_utils.h"
#include "accent.h"

void close_x(char *text[MAX_LINE_NO], int argc, char **argv);
void write_char(char *text[MAX_LINE_NO], unsigned char kp_buffer[BUF_SIZE],
                unsigned long int *act_line, unsigned long int *act_char,
                unsigned long int scroll);
void write_tab(char *text[MAX_LINE_NO],
               unsigned long int *act_line,
               unsigned long int *act_char);
void carriage_return(char *text[MAX_LINE_NO], unsigned long int *act_line,
                     unsigned long int *act_char);
void backspace(char *text[MAX_LINE_NO], unsigned long int *act_line,
               unsigned long int *act_char, unsigned long int scroll);
void write_accent(char *text[MAX_LINE_NO], unsigned char *last_char,
                  unsigned long int *act_line, unsigned long int *act_char,
                  unsigned char kp_buffer[BUF_SIZE],
                  unsigned long int scroll);
void imode_process_key(char *text[MAX_LINE_NO],
                       unsigned char kp_buffer[BUF_SIZE],
                       unsigned long int *act_line,
                       unsigned long int *act_char,
                       unsigned char *last_char, unsigned char *temp_accent,
                       unsigned int scroll, int argc, char **argv);

#endif
