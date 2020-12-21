#ifndef _GLOBAL_H

/* include the X library headers */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

/* include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GLOBAL_H

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

#endif
