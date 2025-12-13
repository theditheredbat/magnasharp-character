#include <assert.h>

int
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

int
main(void)
{
    assert(tokeq("FONTBOUNDINGBOX 6 10 0 -2\r\n", "FONTBOUNDINGBOX"));
    assert(tokeq("POINT_SIZE 100\r\n", "POINT_SIZE"));
    assert(tokeq("STARTPROPERTIES 13\r\n", "STARTPROPERTIES"));
    assert(tokeq("BITMAP\r\n", "BITMAP"));
    assert(tokeq("BBX 6 10 0 -2\r\n", "BBX"));
    assert(tokeq("STARTCHAR char0\r\n", "STARTCHAR"));
    assert(tokeq("CHARSET_REGISTRY \"ISO8859\"\r\n", "CHARSET_REGISTRY"));
    assert(tokeq("DWIDTH 6 0\r\n", "DWIDTH"));
    
    assert(!tokeq("FONTBOUNDINGBOX 6 10 0 -2\r\n", "6"));
    assert(!tokeq("FONTBOUNDINGBOX 6 10 0 -2\r\n", "ENDCHAR"));
    assert(!tokeq("SPACING \"C\"\r\n", "SWIDTH"));
    assert(!tokeq("FONTBOUNDINGBOX 6 10 0 -2\r\n", "FONT"));
    assert(!tokeq("FONT -t-cherry-Medium-R-Normal--10-100-75-75-C-100-ISO8859-1\r\n",
                  "FONTBOUNDINGBOX"));
    assert(!tokeq("ENDFONT", "FONTBOUNDINGBOX"));
    assert(!tokeq("STARTCHAR char255\r\n", "ENDCHAR"));
    assert(!tokeq("FONTBOUNDINGBOX 6 10 0 -2\r\n", "BOUNDINGBOX 6 10 0 -2\r\n"));

    return (0);
}
