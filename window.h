#include <stdbool.h>

#ifndef WINDOW_H
#define WINDOW_H

struct Window {
    int x;
    int y; 
    int width;
    int height;
    char title[30];
    int priority;
    bool isFocused;
};

void renderWindow(struct Window* win);

#endif
