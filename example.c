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
struct pmenu pmenu_from_menu(struct menu mm) {
    struct pmenu pm;
    if (!mm.title.content) {
        pm.title.content = NULL;
    } else {
        pm.title.content = &mm.title.content;
    }
    pm.title.color = mm.title.color;
    pm.title.fmt = &mm.title.fmt;
    pm.title.box = &mm.title.box;
    pm.main.content = mm.main.content;
    pm.main.color = mm.main.color;
    pm.main.e = &mm.main.e;
    pm.main.l = &mm.main.l;
    pm.wid = &mm.wid;
    return pm;
}
int menu_str(int x, int y, int l, char* str) {
    if (!str) {
        return -1;
    }
    int e = strlen(str)/l + ((strlen(str)/l)*l<strlen(str));
    struct widget_content_format fmt = {0,0};
    struct widget wid;
    wid.bx = x;
    wid.by = y;
    wid.lx = l;
    wid.ly = e;
    wid.content = str;
    widget_content(wid, fmt);
    return 0;
}
int menu_init(struct menu mm) {
    return 0;
}
int pmenu_strlen(int l, char* str) {
    if (!str) {
        return -1;
    }
    int r = strlen(str)/l + ((strlen(str)/l)*l<strlen(str));
    return r;
}
int pmenu_init(struct pmenu pm) {
    if (!pm.wid) {
        return -1;
    }
    int e = 0;
    int h = 0;
    struct widget wid_r;
    if (pm.title.content && *pm.title.content) {
        // e = strlen(*pm.title.content)/(pm.wid->lx-2) + ((strlen(*pm.title.content)/(pm.wid->lx-2))*(pm.wid->lx-2)<strlen(*pm.title.content));
        e = pmenu_strlen(pm.wid->lx-2, *pm.title.content);
        struct widget wid_a, wid_b;
        struct widget_content_format fmt = {0,0};
        wid_a.bx = pm.wid->bx, wid_a.by = pm.wid->by, wid_a.lx = pm.wid->lx, wid_a.ly = e+2;
        wid_b = wid_a;
        wid_b.bx++, wid_b.by++, wid_b.lx-=2, wid_b.ly-=2, wid_b.content=*pm.title.content;
        if (pm.title.color) {
            array* temp = frcolor(pm.title.color);
            array_append(scrstr, temp);
            array_drop(&temp);
        }
        if (pm.title.box) {
            array* temp = widget_box(wid_a, *pm.title.box);
            array_append(scrstr, temp);
            array_drop(&temp);
        }
        if (pm.title.fmt) {
            fmt = *pm.title.fmt;
        }
        widget_content(wid_b, fmt);
        e += 2;
        h += e;
    }
    array* temp = fcolor(0);
    array_append(scrstr, temp);
    array_drop(&temp);
    temp = fmove(pm.wid->bx, pm.wid->by+h);
    array_append(scrstr, temp);
    array_drop(&temp);
    if (pm.main.content && !array_is_empty(pm.main.content)) {
        struct widget wid_c;
        struct widget_content_format fmt;
        wid_c.bx = pm.wid->bx, wid_c.by = pm.wid->by+e, wid_c.lx = pm.wid->lx;
        size_t max_len = 0;
        int l=0, r=0, t=0;
        if (!pm.main.l || *pm.main.l>pm.wid->lx-2) {
            l = pm.wid->lx-2;
        } else {
            l = *pm.main.l;
        }
        for (size_t i=0; i<array_len(pm.main.content); i++) {
            t += pmenu_strlen(l, array_fetch_deref(pm.main.content, i, char*));
        }
        if (!pm.main.e) {
            r = 0;
        } else {
            r = *pm.main.e;
        }
        wid_c.ly = 2+2*r+t;
        h += wid_c.ly;
        if (pm.main.color) {
            array* temp = frcolor(pm.main.color);
            array_append(scrstr, temp);
            array_drop(&temp);
        }
        if (pm.main.box) {
            array* temp = widget_box(wid_c, *pm.main.box);
            array_append(scrstr, temp);
            array_drop(&temp);
        }
        if (pm.title.fmt) {
            fmt = *pm.title.fmt;
        }
        int x = 0;
        for (size_t i=0; i<array_len(pm.main.content); i++) {
            int dev = pmenu_strlen(l, array_fetch_deref(pm.main.content, i, char*));
            struct widget wid_d;
            wid_d.bx = wid_c.bx+(wid_c.lx-l)/2;
            wid_d.by = wid_c.by+r+x+1;
            wid_d.lx = l;
            wid_d.ly = dev;
            wid_d.content = array_fetch_deref(pm.main.content, i, char*);
            widget_content(wid_d, fmt);
            x+=dev;
        }
    }
    temp = fcolor(0);
    array_append(scrstr, temp);
    array_drop(&temp);
    temp = fmove(pm.wid->bx, pm.wid->by+h);
    array_append(scrstr, temp);
    array_drop(&temp);
    pm.wid->ly = h;
    return 0;
}
int main() {
    char* cd[] = {"[0] Test model for using one","[1] Test model for using two","[2] Test model for using three"};
    array* content = array_from_nar(cd, sizeof(int*), sizeof(cd)/sizeof(*cd));
    int color[] = {STYLE_HIGH_LIGHT, WORD_BLUE};
    struct widget_content_format fmt = {0,1};
    array* c = array_from_nar(color, sizeof(int), sizeof(color)/sizeof(*color));
    scr_init();
    widget_fill(scr_std(), " ");
    struct widget wid = {10,3,40,10};
    char title_content[] = "PMenu Dispay";
    char* p = title_content;
    struct widget_box box = {"–","–",0,0,"–","–","–","–"};
    struct widget_box bo1x = {"$","$","+","+","O","O","O","O"};
    int e=2, l=25;
    // struct pmenu pm = {{&p,c,&fmt,&box},{content, c, &fmt, &bo1x, &e, &l},&wid};
    struct pmenu pm = {{},{content, c, &fmt, &bo1x, &e, &l},&wid};
    pmenu_init(pm);
    scr_flush();
    scr_getch();
    scr_drop();
    return 0;
}