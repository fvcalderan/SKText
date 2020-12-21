/* __                _     _
  / _|_   _____ __ _| | __| | ___ _ __ __ _ _ __
 | |_\ \ / / __/ _` | |/ _` |/ _ \ '__/ _` | '_ \
 |  _|\ V / (_| (_| | | (_| |  __/ | | (_| | | | |
 |_|   \_/ \___\__,_|_|\__,_|\___|_|  \__,_|_| |_|

BSD 3-Clause License
Copyright (c) 2020, Felipe V. Calderan
All rights reserved.
See the full license inside LICENSE.txt file */

/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* custom lib headers */
#include "global.h"
#include "accent.h"
#include "bufferIO.h"

/* X variables */
Display *dis;
int screen;
Window win;
GC gc;

void init_x()
{
    unsigned long black, white;
    dis    = XOpenDisplay((char *)0);
    screen = DefaultScreen(dis);
    black  = BlackPixel(dis, screen),
    white  = WhitePixel(dis, screen);
    win    = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 300,
                                 5, white, black);
    gc     = XCreateGC(dis, win, 0,0);
    XSetStandardProperties(dis, win, "SKText", "icon", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
    XSetBackground(dis, gc, black);
    XSetForeground(dis, gc, white);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
}

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

void write_char(char *text[MAX_LINE_NO], unsigned char kp_buffer[BUF_SIZE],
                  int act_line, int *act_char, unsigned long int scroll)
{
    strcat(text[act_line], (char *)kp_buffer);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(act_line+1-scroll),
                text[act_line], strlen(text[act_line]));
    (*act_char)++;
}

void write_tab(char *text[MAX_LINE_NO], int act_line, int *act_char)
{
    strcat(text[act_line], TAB);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(act_line+1),
                text[act_line], strlen(text[act_line]));
    (*act_char)+=4;
}

void carriage_return(char *text[MAX_LINE_NO], int *act_line,
                     int *act_char)
{
    (*act_line)++;
    *act_char = 0;
    if (text[*act_line] == NULL)
        text[*act_line] = calloc (MAX_LINE_LEN, 1);
}

void backspace(char *text[MAX_LINE_NO], int *act_line, int *act_char,
               unsigned long int scroll)
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

void draw_cursor(int act_line, int act_char, unsigned long int scroll)
{
    XDrawString(dis, win, gc, CHAR_WIDTH*(act_char+1),
                CHAR_HEIGHT*(act_line+1-scroll), (char[]){2,0}, 1);
}

void write_accent(char *text[MAX_LINE_NO], unsigned char last_char,
                  int act_line, int *act_char,
                  unsigned char kp_buffer[BUF_SIZE],
                  unsigned long int scroll)
{
    unsigned char letter = get_accented_char(kp_buffer[0], last_char);
    strcat(text[act_line], (char[]){letter, 0});
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(act_line+1-scroll),
                text[act_line], strlen(text[act_line]));
    (*act_char)++;
}

int main (int argc, char **argv)
{
    XEvent event;                       /* XEvent declaration */
    KeySym key;                         /* handle KeyPresses */
    unsigned char kp_buffer[BUF_SIZE];  /* char buffer for KeyPress events */
    char *text[MAX_LINE_NO] = { NULL }; /* actual text buffer */
    int act_line = 0;                   /* current line number */
    int act_char = 0;                   /* current row number */
    unsigned long int scroll = 0;       /* scroll offset */
    unsigned char temp_accent = 0;      /* temp variable to store accent */
    unsigned char last_char = 0;        /* temp variable to store last char */

    init_x();
    load_buffer(text, &act_line, &act_char, argc, argv);

    while(1) {                      /* keeps checking for events */
        XNextEvent(dis, &event);
        if (event.type == KeyPress &&
            XLookupString(&event.xkey,(char*)kp_buffer,BUF_SIZE,&key,0) == 1) {
            switch (kp_buffer[0]) {
                case 94: case 96: case 126: case 168: case 180: /* accents */
                    temp_accent = kp_buffer[0];
                    break;
                case 8:             /* bksp */
                    backspace(text, &act_line, &act_char, scroll);
                    temp_accent = 0;
                    break;
                case 9:             /* tab */
                    write_tab(text, act_line, &act_char);
                    temp_accent = 0;
                    break;
                case 13:            /* enter */
                    carriage_return(text, &act_line, &act_char);
                    temp_accent = 0;
                    break;
                case 27:            /* esc */
                    temp_accent = 0;
                    close_x(text, argc, argv);
                    break;
                case 'a': case 'A': case 'e': case 'E': case 'i': case 'I':
                case 'o': case 'O': case 'u': case 'U': case 'y': case 'Y':
                case 'n': case 'N': /* accentable characters */
                    write_accent(text, last_char, act_line, &act_char,
                                 kp_buffer, scroll);
                    temp_accent = 0;
                    break;
                default:            /* everything else */
                    if (temp_accent > 0) {
                        write_char(text, (unsigned char[]){temp_accent, 0},
                                   act_line, &act_char, scroll);
                    }
                    write_char(text, kp_buffer, act_line, &act_char, scroll);
                    temp_accent = 0;
                    break;
            }
            last_char = kp_buffer[0];
        }
        /* check for mouse scroll */
        if (event.type == ButtonPress) {
            switch (event.xbutton.button) {
                case Button4:
                    scroll = scroll > 0 ? scroll - 1 : 0;
                    break;
                case Button5:
                    scroll++;
                    break;
            }
        }
        redraw(text, scroll);
        draw_cursor(act_line, act_char, scroll);
    }
    free_text(text);
    return 0;
}
