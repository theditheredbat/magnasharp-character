#include "fileio.h"
#include "font.h"

static int
tokeq(const char *stream, const char *tok)
{
    while(*stream != ' ' && *stream != '\r' && *stream != '\n' && *tok) {
        if(*stream != *tok)
            return (0);
        ++stream;
        ++tok;
    }
    return (!(*tok) && (*stream == ' ' || *stream == '\r' || *stream == '\n'));
}

static int
get_num(const char *stream)
{
    int num = 0;

    while(*stream && *stream != ' ')  {
        num *= 10;
        num += *stream - 48;
        ++stream;
    }

    return (num);
}

struct font_bitmap
load_bdf(const char *path)
{
    struct font_bitmap font = {0};
    struct file_content file = {0};
    char *readch = 0;

    file = platform_get_file_content(path);
    readch = file.mem;
    while(*readch) {
        if(tokeq(readch, "SIZE")) {
            while(*readch++ != ' ')
                ;
            font.size = get_num(readch);
        }
        while(*readch && *readch != '\n')
            ++readch;
        if(*readch)
            ++readch;
    }
    platform_free_file_memory(&file);

    return (font);
}
