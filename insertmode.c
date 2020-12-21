#include "insertmode.h"

void close_x(char *text[MAX_LINE_NO], int argc, char **argv)
{
    /* save the buffer */
    save_buffer(text, argc, argv);

    /* destroy X window and exit */
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(1);
}

void write_char(char *text[MAX_LINE_NO], unsigned char kp_buffer[BUF_SIZE],
                unsigned long int *act_line, unsigned long int *act_char,
                unsigned long int scroll)
{
    strcat(text[*act_line], (char *)kp_buffer);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(*act_line+1-scroll),
                text[*act_line], strlen(text[*act_line]));
    (*act_char)++;
}

void write_tab(char *text[MAX_LINE_NO],
               unsigned long int *act_line,
               unsigned long int *act_char)
{
    strcat(text[*act_line], TAB);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(*act_line+1),
                text[*act_line], strlen(text[*act_line]));
    (*act_char)+=4;
}

void carriage_return(char *text[MAX_LINE_NO], unsigned long int *act_line,
                     unsigned long int *act_char)
{
    (*act_line)++;
    *act_char = 0;
    if (text[*act_line] == NULL)
        text[*act_line] = calloc (MAX_LINE_LEN, 1);
}

void backspace(char *text[MAX_LINE_NO], unsigned long int *act_line,
               unsigned long int *act_char, unsigned long int scroll)
{
    int i;
    if (*act_char > 0) {                /* there are erasables */
        text[*act_line][*act_char-1] = '\0';
        (*act_char)--;
        redraw(text, scroll);
    } else if (*act_line > 0) {         /* there aren't erasables */
        free(text[*act_line]);
        text[*act_line] = NULL;
        (*act_line)--;
        for (i = 0; i < MAX_LINE_LEN; i++) /* move cursor to the correct pos */
            if (text[*act_line][i] == '\0')
                break;
        *act_char = i;
    }
}


void write_accent(char *text[MAX_LINE_NO], unsigned char *last_char,
                  unsigned long int *act_line, unsigned long int *act_char,
                  unsigned char kp_buffer[BUF_SIZE],
                  unsigned long int scroll)
{
    unsigned char letter = get_accented_char(kp_buffer[0], *last_char);
    strcat(text[*act_line], (char[]){letter, 0});
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(*act_line+1-scroll),
                text[*act_line], strlen(text[*act_line]));
    (*act_char)++;
}


void imode_process_key(char *text[MAX_LINE_NO],
                       unsigned char kp_buffer[BUF_SIZE],
                       unsigned long int *act_line,
                       unsigned long int *act_char,
                       unsigned char *last_char, unsigned char *temp_accent,
                       unsigned int scroll, int argc, char **argv)
{
    switch (kp_buffer[0]) {
        case 94: case 96:   /* accents */
        case 126: case 168: case 180:
            *temp_accent = kp_buffer[0];
            break;
        case 8:             /* bksp */
            backspace(text, act_line, act_char, scroll);
            *temp_accent = 0;
            break;
        case 9:             /* tab */
            write_tab(text, act_line, act_char);
            *temp_accent = 0;
            break;
        case 13:            /* enter */
            carriage_return(text, act_line, act_char);
            *temp_accent = 0;
            break;
        case 27:            /* esc */
            *temp_accent = 0;
            close_x(text, argc, argv);
            break;
        case 'a': case 'A': case 'e': case 'E': case 'i': case 'I':
        case 'o': case 'O': case 'u': case 'U': case 'y': case 'Y':
        case 'n': case 'N': /* accentable characters */
            write_accent(text, last_char, act_line, act_char,
                         kp_buffer, scroll);
            *temp_accent = 0;
            break;
        default:            /* everything else */
            if (*temp_accent > 0) {
                write_char(text, (unsigned char[]){*temp_accent, 0},
                           act_line, act_char, scroll);
            }
            write_char(text, kp_buffer, act_line, act_char, scroll);
            *temp_accent = 0;
            break;
    }
    *last_char = kp_buffer[0];
}
