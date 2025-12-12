#include <stdio.h>

#include "fileio.h"

int
main(void)
{
    int i = 0;
    struct file_content file = {0};

    file = platform_get_file_content("readfile.c");
    for(i = 0; i < file.len; ++i)
        printf("%c", file.mem[i]);

    platform_free_file_memory(&file);

    file = platform_get_file_content("../../src/win32_fileio.c");
    for(i = 0; i < file.len; ++i)
        printf("%c", file.mem[i]);

    platform_free_file_memory(&file);
    
    return (0);
}
