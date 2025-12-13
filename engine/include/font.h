#ifndef FONT_H
#define FONT_H

struct font_bitmap {
    int size;
};

struct font_bitmap load_bdf(const char *path);

#endif
