#include <Windows.h>

#include "fileio.h"

struct file_content
platform_get_file_content(const char *path)
{
    struct file_content res = {0};
    HANDLE hfile = CreateFile(path,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   0,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   0);
    LARGE_INTEGER file_size = {0};
    GetFileSizeEx(hfile, &file_size);
    res.mem = HeapAlloc(GetProcessHeap(),
                        0,
                        file_size.QuadPart - 1);
    if(res.mem) {
        DWORD nbytes = 0;
        ReadFile(hfile, res.mem,
                 file_size.QuadPart - 2, &nbytes,
                 0);
        if((file_size.QuadPart - 2) != nbytes) {
            HeapFree(GetProcessHeap(), 0, res.mem);
            res.mem = 0;
        }
    }
    CloseHandle(hfile);
    res.len = file_size.QuadPart - 2;
    res.mem[file_size.QuadPart - 2] = '\0';
    return (res);
}

void
platform_free_file_memory(struct file_content *file)
{
    HeapFree(GetProcessHeap(), 0, file->mem);
    file->mem = 0;
    file->len = 0;
}
