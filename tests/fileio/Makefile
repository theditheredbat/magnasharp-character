CC = clang
CFLAGS = -Werror -Wpedantic -g

readfile: readfile.c
	$(CC) readfile.c ../../engine/win32_fileio.c -o readfile.exe -I"../../engine/include" -lKernel32.lib $(CFLAGS)

filesize: filesize.c
	$(CC) filesize.c ../../engine/win32_fileio.c -o filesize.exe -I"../../engine/include" -lKernel32.lib $(CFLAGS)
