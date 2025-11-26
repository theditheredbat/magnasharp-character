#ifndef FILEIO_H
#define FILEIO_H

struct file_content {
    char *mem;
    int len;
};

struct file_content platform_get_file_content(const char *path);
void platform_free_file_memory(struct file_content *file);

#endif
