default:
	cd out
	cl ../src/win32_entry.c User32.lib Gdi32.lib -Zi -FC -Fe:char
