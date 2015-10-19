#ifndef UIATLAS_H
#define UIATLAS_H

struct AtlasImage
{
    float u1, v1, u2, v2;
    int w, h;
    const char *name;
};

// IMAGES_ui
#define I_SOLIDWHITE 0
#define I_H 1
#define I_I 2
#define I_V 3
#define I_E 4
#define I_SELECT 5
#define I_START 6
#define I_ARROW 7
#define I_DIR 8
#define I_ROUND 9
#define I_RECT 10
#define I_STICK 11
#define I_STICK_BG 12
#define I_SHOULDER 13
#define I_DIR_LINE 14
#define I_ROUND_LINE 15
#define I_RECT_LINE 16
#define I_SHOULDER_LINE 17
#define I_STICK_LINE 18
#define I_STICK_BG_LINE 19
#define I_CHECKEDBOX 20
#define I_BG 21
#define I_BG_GOLD 22
#define I_L 23
#define I_R 24
#define I_DROP_SHADOW 25
#define I_LINES 26
#define I_GRID 27
#define I_LOGO 28
#define I_ICON 29
#define I_FOLDER 30
#define I_UP_DIRECTORY 31
#define I_GEAR 32

#endif // UIATLAS_H
