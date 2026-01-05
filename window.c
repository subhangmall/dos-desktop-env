#include <stdbool.h>
#include "graphics.h"

struct Window {
    int x;
    int y; 
    int width;
    int height;
    char title[30];
    int priority; 
    bool isFocused;
    bool canResize;
};

struct Button {
    int x;
    int y;
    int width;
    int height;
    bool customBitmapEnabled;
    const char* text;
    unsigned char fgColor;
    unsigned char bgColor;
    unsigned char bgColorWhenPressed;
    unsigned char bitmap[8][8]; // Placeholder for bitmap data
};

void renderWindow(struct Window* win) {
    struct Button closeButton;

    drawRectangle(win->x, win->y, win->width, win->height, 7); // Draw window background
    drawRectangle(win->x, win->y, win->width, 15, 1); // Draw title bar
    write(win->title, 15, win->x + 20, win->y + 4); // Draw title text
    drawLine(win->x, win->y + 15, win->x + win->width, win->y + 15, 0); // Draw line below title bar
    // draw lines around window
    drawLine(win->x, win->y, win->x + win->width, win->y, 0); // Top
    drawLine(win->x, win->y, win->x, win->y + win->height, 0); // Left
    drawLine(win->x + win->width, win->y, win->x + win->width, win->y + win->height, 0); // Right
    drawLine(win->x, win->y + win->height, win->x + win->width, win->y + win->height, 0); // bottom
    // draw 2px thick light grey border around previous black lines
    drawLine(win->x - 1, win->y - 1, win->x + win->width + 1, win->y - 1, 7); // Top outer 1
    drawLine(win->x - 2, win->y - 2, win->x + win->width + 2, win->y - 2, 7); // Top outer 2
    drawLine(win->x - 1, win->y - 1, win->x - 1, win->y + win->height + 1, 7); // Left outer 1
    drawLine(win->x - 2, win->y - 2, win->x - 2, win->y + win->height + 2, 7); // Left outer 2
    drawLine(win->x + win->width + 1, win->y - 1, win->x + win->width + 1, win->y + win->height + 1, 7); // Right outer 1
    drawLine(win->x + win->width + 2, win->y - 2, win->x + win->width + 2, win->y + win->height + 2, 7); // Right outer 2
    drawLine(win->x - 1, win->y + win->height + 1, win->x + win->width + 1, win->y + win->height + 1, 7); // Bottom outer 1
    drawLine(win->x - 2, win->y + win->height + 2, win->x + win->width + 2, win->y + win->height + 2, 7); // Bottom outer 2
    // draw black lines around that 
    drawLine(win->x - 3, win->y - 3, win->x + win->width + 3, win->y - 3, 0); // Top outer 3
    drawLine(win->x - 3, win->y - 3, win->x - 3, win->y + win->height + 3, 0); // Left outer 3
    drawLine(win->x + win->width + 3, win->y - 3, win->x + win->width + 3, win->y + win->height + 3, 0); // Right outer 3
    drawLine(win->x - 3, win->y + win->height + 3, win->x + win->width + 3, win->y + win->height + 3, 0); // bottom outer 3

    // closeButton = (struct Button){win->width - 18, 2, 10, 10, false, "X", 15, 4, 12, {{0}}, false};
    closeButton.x = win->width - 18;
    closeButton.y = 2;
    closeButton.width = 10;
    closeButton.height = 10;
    closeButton.customBitmapEnabled = false;
    closeButton.text = "X";
    closeButton.fgColor = 15;
    closeButton.bgColor = 4;
    closeButton.bgColorWhenPressed = 12;
    renderCustomButton(&closeButton, win); // Close button
    
}

void renderCustomButton(struct Button* btn, struct Window *parentWin) {
    int buttonX = parentWin->x + btn->x;
    int buttonY = parentWin->y + btn->y;
    
    if (!btn->customBitmapEnabled) {
        if (strlen(btn->text) * 8 + 2 > btn->width || 8 + 2 > btn->height) {
            return; // Text too large for button
        }
    } else {
        // Check bitmap size (assuming 8x8 for simplicity)
        if (8 + 2 > btn->width || 8 + 2 > btn->height) {
            return; // Bitmap too large for button
        }
    }

    // Draw button background
    drawRectangle(buttonX, buttonY, btn->width, btn->height, btn->bgColor);
    
    // Draw 1px black border
    drawLine(buttonX, buttonY, buttonX + btn->width, buttonY, 0); // Top
    drawLine(buttonX, buttonY, buttonX, buttonY + btn->height, 0); // Left
    drawLine(buttonX + btn->width, buttonY, buttonX + btn->width, buttonY + btn->height, 0); // Right
    drawLine(buttonX, buttonY + btn->height, buttonX + btn->width, buttonY + btn->height, 0); // Bottom
    
    // Draw text
    if (!btn->customBitmapEnabled) {
        write(btn->text, btn->fgColor, buttonX + 2, buttonY + 2);
    } else {
        // Draw bitmap (placeholder - actual implementation would render bitmap data)
        // drawBitmap(btn->bitmap, buttonX + 1, buttonY + 1, btn->fgColor);
    }
}

// void renderCustomBitmapButton(int relativeX, int relativeY, int width, int height, unsigned char bitmap[][], unsigned char fgColor, unsigned char bgColor, unsigned char bgColorWhenPressed, struct Window *parentWin) {
//     int buttonX = parentWin->x + relativeX;
//     int buttonY = parentWin->y + relativeY;
    
//     // Draw button background
//     drawRectangle(buttonX, buttonY, width, height, bgColor);
    
//     // Draw 1px black border
//     drawLine(buttonX, buttonY, buttonX + width, buttonY, 0); // Top
//     drawLine(buttonX, buttonY, buttonX, buttonY + height, 0); // Left
//     drawLine(buttonX + width, buttonY, buttonX + width, buttonY + height, 0); // Right
//     drawLine(buttonX, buttonY + height, buttonX + width, buttonY + height, 0); // Bottom
    
//     // Draw bitmap (placeholder - actual implementation would render bitmap data)
//     // drawBitmap(bitmap, buttonX + 1, buttonY + 1, fgColor);
// }

// void renderNativeBitmapButton(int relativeX, int relativeY, int width, int height, unsigned char bitmap[][], struct Window *parentWin) {
// }