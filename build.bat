@echo off

pushd out
    clang ../app/magnasharp.c ../engine/win32_entry.c ../engine/win32_fileio.c ../engine/font.c ../engine/third-party/glad.c -o char.exe -I"../engine/third-party" -I"../app/include" -I"../engine/include" -lUser32.lib -lGdi32.lib -lOpengl32.lib -Werror -Wall -fdiagnostics-absolute-paths -g
popd
