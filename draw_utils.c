#include "draw_utils.h"

void redraw(char *text[MAX_LINE_NO], unsigned long int scroll)
{
    /* clear screen and load text */
    int i;
    XClearWindow(dis, win);
    for (i = 0; i < MAX_LINE_NO; i++)
        if (text[i] != NULL)
            XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(i+1-scroll),
                        text[i], strlen(text[i]));
}

void draw_cursor(int act_line, int act_char, unsigned long int scroll)
{
    XDrawString(dis, win, gc, CHAR_WIDTH*(act_char+1),
                CHAR_HEIGHT*(act_line+1-scroll), (char[]){2,0}, 1);
}
