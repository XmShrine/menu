#ifndef pmenu_h
#define pmenu_h
#include "widget.h"
#define BACKGROUND_BLACK 40
#define BACKGROUND_DARK_RED 41
#define BACKGROUND_GREEN 42
#define BACKGROUND_YELLOW 43
#define BACKGROUND_BLUE 44
#define BACKGROUND_PURPLE 45
#define BACKGROUND_DARK_GREEN 46
#define BACKGROUND_WHITE 47
#define WORD_BLACK 30
#define WORD_RED 31
#define WORD_GREEN 32
#define WORD_YELLOW 33
#define WORD_BLUE 34
#define WORD_PURPLE 35
#define WORD_DARK_GREEN 36
#define WORD_WHITE 37
#define STYLE_OPEN 0
#define STYLE_CLOSE 0
#define STYLE_HIGH_LIGHT 1
#define STYLE_LOW_LIGHT 2
#define STYLE_UNDERLINE 4
#define STYLE_BLINK 5
#define STYLE_INVERSE 7
struct menu {
    struct {
        char* content;
        array* color;
        struct widget_content_format fmt;
        struct widget_box box;
    }title;
    struct {
        array* content;
        array* color;
        struct widget_content_format fmt;
        struct widget_box box;
        int e; // enter num
        int l; // how long the word displayed
    }main;
    struct widget wid;
};
struct pmenu {
    struct {
        char** content;
        array* color;
        struct widget_content_format* fmt;
        struct widget_box* box;
    }title;
    struct {
        array* content;
        array* color;
        struct widget_content_format* fmt;
        struct widget_box* box;
        int* e;
        int* l;
    }main;
    struct widget* wid;
};
struct pmenu pmenu_from_menu(struct menu* mm);
int menu_init(struct menu* mm);
int pmenu_init(struct pmenu pm);
#endif