#include <Windows.h>

#include <fileio.h>

struct file_content
platform_get_file_content(const char *path)
{
    struct file_content res = {0};
    HANDLE fileHandle = CreateFile(path,
                                   GENERIC_READ,
                                   FILE_SHARE_READ,
                                   0,
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   0);
    LARGE_INTEGER fileSize = {0};
    GetFileSizeEx(fileHandle, &fileSize);
    res.mem = HeapAlloc(GetProcessHeap(),
                        0,
                        fileSize.QuadPart);
    if(res.mem) {
        DWORD bytesRead = 0;
        ReadFile(fileHandle, res.mem,
                 fileSize.QuadPart, &bytesRead,
                 0);
        if(fileSize.QuadPart != bytesRead) {
            HeapFree(GetProcessHeap(), 0, res.mem);
            res.mem = 0;
        }
    }
    CloseHandle(fileHandle);
    res.len = fileSize.QuadPart;
    return (res);
}

void
platform_free_file_memory(struct file_content *file)
{
    HeapFree(GetProcessHeap(), 0, file->mem);
    file->mem = 0;
    file->len = 0;
}
