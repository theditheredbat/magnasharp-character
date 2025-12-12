#include <stdio.h>

#include "fileio.h"

int
main(void)
{
    int i = 0;
    struct file_content hello, goodbye;

    hello = platform_get_file_content("testfiles/hello.txt");
    goodbye = platform_get_file_content("testfiles/goodbye.txt");

    printf("hello length: %d\ngoodbye length: %d\n", hello.len, goodbye.len);

    platform_free_file_memory(&hello);
    platform_free_file_memory(&goodbye);

    return (0);
}
