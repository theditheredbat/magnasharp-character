CC = clang

readfile.exe: readfile.c
	$(CC) readfile.c ../../src/win32_fileio.c -o readfile.exe -I"../../src/" -lKernel32.lib -Werror -Wpedantic -g
