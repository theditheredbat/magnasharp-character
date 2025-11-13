default:
	cd out
	cl ../src/magnasharp.c ../src/win32_entry.c User32.lib Gdi32.lib -Zi -FC -Fe:char
