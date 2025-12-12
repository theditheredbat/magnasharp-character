#ifndef FILEIO_H
#define FILEIO_H

struct file_content {
    char *mem;
    int len;
};

/*
   Param:   Path to file
   Return: Pointer to Heap Allocated memory
            containing contents of file with
            null-terminator appended, and
            length of file excluding
            null-terminator
 */
struct file_content platform_get_file_content(const char *path);
void platform_free_file_memory(struct file_content *file);

#endif
