#include <X11/Xlib.h>
#include <cstdio>

int main() {

    Display *d = XOpenDisplay(NULL);
    if(d == NULL) {
        printf("Unable to open the display\n");
        return -1;
    }

    int s = DefaultScreen(d);
    Window w = XCreateSimpleWindow(d, RootWindow(d, s), 0, 0, 100, 100, 1, 
                                    BlackPixel(d, s), WhitePixel(d, s));
    XMapWindow(d, w);

    XPending(d);

    getchar();

    printf("EXITING\n");
}