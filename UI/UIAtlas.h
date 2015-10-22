#ifndef UIATLAS_H
#define UIATLAS_H

struct AtlasImage
{
    float u1, v1, u2, v2;
    int w, h;
    const char *name;
};

// IMAGES_ui
enum
{
    I_SOLIDWHITE = 0,
    I_H = 1,
    I_I = 2,
    I_V = 3,
    I_E = 4,
    I_SELECT = 5,
    I_START = 6,
    I_ARROW = 7,
    I_DIR = 8,
    I_ROUND = 9,
    I_RECT = 10,
    I_STICK = 11,
    I_STICK_BG = 12,
    I_SHOULDER = 13,
    I_DIR_LINE = 14,
    I_ROUND_LINE = 15,
    I_RECT_LINE = 16,
    I_SHOULDER_LINE = 17,
    I_STICK_LINE = 18,
    I_STICK_BG_LINE = 19,
    I_CHECKEDBOX = 20,
    I_BG = 21,
    I_BG_GOLD = 22,
    I_L = 23,
    I_R = 24,
    I_DROP_SHADOW = 25,
    I_LINES = 26,
    I_GRID = 27,
    I_LOGO = 28,
    I_ICON = 29,
    I_FOLDER = 30,
    I_UP_DIRECTORY = 31,
    I_GEAR = 32,
    I_MAX
};

#endif // UIATLAS_H
