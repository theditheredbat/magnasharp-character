all:
	cd out
	clang ../src/magnasharp.c ../src/win32_entry.c ../src/win32_fileio.c -o char.exe -g -Werror -fdiagnostics-absolute-paths -lUser32.lib -lGdi32.lib
