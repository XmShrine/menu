#include "pmenu.h"
struct pmenu pmenu_from_menu(struct menu* mm) {
    struct pmenu pm;
    if (!mm) {
        pm.title.content = 0;
        pm.title.color = 0;
        pm.title.fmt = 0;
        pm.title.box = 0;
        pm.main.content = 0;
        pm.main.color = 0;
        pm.main.e = 0;
        pm.main.l = 0;
        pm.wid = 0;
        return pm;
    }
    if (!mm->title.content) {
        pm.title.content = NULL;
    } else {
        pm.title.content = &mm->title.content;
    }
    pm.title.color = mm->title.color;
    pm.title.fmt = &(mm->title.fmt);
    pm.title.box = &(mm->title.box);
    pm.main.content = mm->main.content;
    pm.main.color = mm->main.color;
    pm.main.fmt = &(mm->main.fmt);
    pm.main.box = &(mm->main.box);
    pm.main.e = &(mm->main.e);
    pm.main.l = &(mm->main.l);
    pm.wid = &(mm->wid);
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
    return pmenu_init(pmenu_from_menu(&mm));
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
            widget_box(wid_a, *pm.title.box);
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
            widget_box(wid_c, *pm.main.box);
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