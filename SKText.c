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

/* include standard headers */
#include <stdio.h>
#include <stdlib.h>

/* defines */
#define BUF_SIZE        256
#define MAX_LINE_LEN    256
#define MAX_LINE_NO     65536
#define CHAR_WIDTH      6
#define CHAR_HEIGHT     16
#define MAX_FN_SIZE     512
#define TAB             "    "

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

void free_text(char *text[MAX_LINE_NO])
{
    int i;
    for (i = 0; i < MAX_LINE_NO; i++)
        if (text[i] != NULL)
            free(text[i]);
        else
            break;
    free(text);
}

void close_x(char *text[MAX_LINE_NO], int argc, char **argv)
{
    int i;

    /* verify args */
    char file[MAX_FN_SIZE] = "saved_buffer.txt";
    if (argc > 1)
        strcpy(file, argv[1]);

    /* save buffer to file */
    FILE *fp = fopen(file, "w+");
    if (fp == NULL) {
        printf("Failed to open buffer\n");
        free_text(text);
        exit(1);
    }
    for (i = 0; i < MAX_LINE_NO; i++)
        if (text[i] != NULL) {
            strcat(text[i], "\n");
            fprintf(fp, text[i]);
        } else {
            break;
        }
    fclose(fp);
    free_text(text);

    /* destroy X window and exit */
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(1);
}

void load_buffer(char *text[MAX_LINE_NO], int *actual_line, int *actual_char,
                 int argc, char **argv)
{
    int i = 0;

    /* load file to buffer */
    FILE *fp;

    /* verify args - if none specified, create/edit saved_buffer.txt */
    char file[MAX_FN_SIZE] = "saved_buffer.txt";
    if (argc > 1) {
        strcpy(file, argv[1]);
    } else {
        fp = fopen(file, "w+");
        if (fp == NULL) {
            printf("Failed to open buffer\n");
            exit(1);
        }
        text[i] = calloc (MAX_LINE_LEN, 1);
        return;
    }

    fp = fopen(file, "r+");
    char buffer[MAX_LINE_LEN];
    if (fp == NULL) {
        /* if file doesn't exist, create it */
        fp = fopen(file, "w+");
        if (fp == NULL) {
            printf("Failed to open buffer\n");
            exit(1);
        }
        text[i] = calloc (MAX_LINE_LEN, 1);
        return;
    }

    /* populate lines */
    while((fgets (buffer, MAX_LINE_LEN, fp))!= NULL) {
        text[i] = calloc (MAX_LINE_LEN, 1);
        for (*actual_char = 0; *actual_char < MAX_LINE_LEN; (*actual_char)++)
            if (buffer[*actual_char] == '\n') {
                buffer[*actual_char] = '\0';
                break;
            } else if (buffer[*actual_char] == '\0') {
                break;
            }
        (*actual_line)++;
        *actual_char = 0;
        strcpy(text[i], buffer);
        i++;
    }

    /* allocate at least one line if the file is empty */
    text[i] = i == 0 ? calloc (MAX_LINE_LEN, 1) : text[i];

    /* move cursor to the correct position */
    *actual_line = *actual_line > 0 ? *actual_line - 1 : 0;
    for (i = 0; i < MAX_LINE_LEN; i++)
        if (text[*actual_line][i] == '\0')
            break;
    *actual_char = i;

    fclose(fp);
}

void redraw(char *text[MAX_LINE_NO])
{
    /* clear screen and load text */
    int i;
    XClearWindow(dis, win);
    for (i = 0; i < MAX_LINE_NO; i++)
        if (text[i] != NULL)
            XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(i+1),
                        text[i], strlen(text[i]));
}

void write_char(char *text[MAX_LINE_NO], char kp_buffer[BUF_SIZE],
                  int actual_line, int *actual_char)
{
    strcat(text[actual_line], kp_buffer);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(actual_line+1),
                text[actual_line], strlen(text[actual_line]));
    (*actual_char)++;
}

void write_tab(char *text[MAX_LINE_NO], int actual_line, int *actual_char)
{
    strcat(text[actual_line], TAB);
    XDrawString(dis, win, gc, CHAR_WIDTH, CHAR_HEIGHT*(actual_line+1),
                text[actual_line], strlen(text[actual_line]));
    (*actual_char)+=4;
}

void carriage_return(char *text[MAX_LINE_NO], int *actual_line,
                     int *actual_char)
{
    (*actual_line)++;
    *actual_char = 0;
    if (text[*actual_line] == NULL)
        text[*actual_line] = calloc (MAX_LINE_LEN, 1);
}

void backspace(char *text[MAX_LINE_NO], int *actual_line, int *actual_char)
{
    int i;
    if (*actual_char > 0) {                /* there are erasables */
        text[*actual_line][*actual_char-1] = '\0';
        (*actual_char)--;
        redraw(text);
    } else if (*actual_line > 0) {         /* there aren't erasables */
        free(text[*actual_line]);
        text[*actual_line] = NULL;
        (*actual_line)--;
        for (i = 0; i < MAX_LINE_LEN; i++) /* move cursor to the correct pos */
            if (text[*actual_line][i] == '\0')
                break;
        *actual_char = i;
    }
}

void draw_cursor(int actual_line, int actual_char)
{
    XDrawString(dis, win, gc, CHAR_WIDTH*(actual_char+1),
                CHAR_HEIGHT*(actual_line+1), "|", 1);
}

int main (int argc, char **argv)
{
    XEvent event;                       /* XEvent declaration */
    KeySym key;                         /* handle KeyPresses */
    char kp_buffer[BUF_SIZE];           /* char buffer for KeyPress events */
    char *text[MAX_LINE_NO] = { NULL }; /* actual text buffer */
    int actual_line = 0;                /* current line number */
    int actual_char = 0;                /* current row number */

    init_x();
    load_buffer(text, &actual_line, &actual_char, argc, argv);

    while(1) {                      /* keeps checking for events */
        XNextEvent(dis, &event);

        if (event.type == KeyPress &&
            XLookupString(&event.xkey, kp_buffer, BUF_SIZE, &key, 0) == 1) {
            switch (kp_buffer[0]) {
                case 8:             /* bksp */
                    backspace(text, &actual_line, &actual_char);
                    break;
                case 9:             /* tab */
                    write_tab(text, actual_line, &actual_char);
                    break;
                case 13:            /* enter */
                    carriage_return(text, &actual_line, &actual_char);
                    break;
                case 27:            /* esc */
                    close_x(text, argc, argv);
                    break;
                case 32 ... 126:     /* from space to right curly bracket */
                    write_char(text, kp_buffer, actual_line, &actual_char);
                    break;
            }
        }
        redraw(text);
        draw_cursor(actual_line, actual_char);
    }
    free_text(text);
    return 0;
}
