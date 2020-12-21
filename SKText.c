/* __                _     _
  / _|_   _____ __ _| | __| | ___ _ __ __ _ _ __
 | |_\ \ / / __/ _` | |/ _` |/ _ \ '__/ _` | '_ \
 |  _|\ V / (_| (_| | | (_| |  __/ | | (_| | | | |
 |_|   \_/ \___\__,_|_|\__,_|\___|_|  \__,_|_| |_|

BSD 3-Clause License
Copyright (c) 2020, Felipe V. Calderan
All rights reserved.
See the full license inside LICENSE.txt file */

/* custom lib headers */
#include "global.h"
#include "draw_utils.h"
#include "bufferIO.h"
#include "insertmode.h"

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

int main (int argc, char **argv)
{
    XEvent event;                       /* XEvent declaration */
    KeySym key;                         /* handle KeyPresses */
    unsigned char kp_buffer[BUF_SIZE];  /* char buffer for KeyPress events */
    char *text[MAX_LINE_NO] = { NULL }; /* actual text buffer */
    unsigned long int act_line = 0;     /* current line number */
    unsigned long int act_char = 0;     /* current row number */
    unsigned long int scroll = 0;       /* scroll offset */
    unsigned char temp_accent = 0;      /* temp variable to store accent */
    unsigned char last_char = 0;        /* temp variable to store last char */

    init_x();
    load_buffer(text, &act_line, &act_char, argc, argv);

    while(1) {                      /* keeps checking for events */
        XNextEvent(dis, &event);
        if (event.type == KeyPress &&
            XLookupString(&event.xkey,(char*)kp_buffer,BUF_SIZE,&key,0) == 1) {
            imode_process_key(text, kp_buffer, &act_line, &act_char,
                              &last_char, &temp_accent, scroll, argc, argv);
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
