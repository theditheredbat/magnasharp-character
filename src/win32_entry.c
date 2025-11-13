#include <Windows.h>

#include "platform.h"
#include "magnasharp.h"

struct win32_back_buffer {
    int width, height;
    int bytesPerPixel;
    void *memory;
    BITMAPINFO info;
};

internal int g_running;
internal struct win32_back_buffer g_buffer;

internal void
win32_create_dib_section(int width, int height)
{
    g_buffer.width = width;
    g_buffer.height = height;
    g_buffer.bytesPerPixel = 4;
    
    g_buffer.info.bmiHeader.biSize = sizeof(g_buffer.info.bmiHeader);
    g_buffer.info.bmiHeader.biWidth = g_buffer.width;
    g_buffer.info.bmiHeader.biHeight = -g_buffer.height;
    g_buffer.info.bmiHeader.biPlanes = 1;
    g_buffer.info.bmiHeader.biBitCount = g_buffer.bytesPerPixel * 8;
    g_buffer.info.bmiHeader.biCompression = BI_RGB;

    int bitMapSize = g_buffer.width * g_buffer.height * g_buffer.bytesPerPixel;
    g_buffer.memory = VirtualAlloc(0, bitMapSize, MEM_COMMIT, PAGE_READWRITE);
}

internal void
win32_display_buffer(HDC devContext, const RECT *clientRect)
{
    int windowWidth = clientRect->right - clientRect->left;
    int windowHeight = clientRect->bottom - clientRect->top;
    StretchDIBits(devContext,
                  0, 0,
                  windowWidth, windowHeight,
                  0, 0,
                  g_buffer.width, g_buffer.height,
                  g_buffer.memory,
                  &g_buffer.info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK
win32_window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;
    switch(uMsg) {
    case WM_PAINT:
        PAINTSTRUCT paint;
        HDC deviceContext = BeginPaint(hWnd, &paint);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        win32_display_buffer(deviceContext, &clientRect);
        EndPaint(hWnd, &paint);
        break;
    case WM_CLOSE:
    case WM_DESTROY:
        g_running = 0;
        break;
    default:
        res = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return (res);
}

int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow)
{
    win32_create_dib_section(1280, 720);
    struct back_buffer buf = {
        .width = g_buffer.width,
        .height = g_buffer.height,
        .memory = g_buffer.memory
    };
    
    WNDCLASS windowClass = {
        .style = CS_OWNDC | CS_VREDRAW | CS_VREDRAW,
        .lpfnWndProc = win32_window_callback,
        .hInstance = hInst,
        .lpszClassName = "win32WindowClass",
    };

    RegisterClass(&windowClass);

    HWND windowHandle = CreateWindowEx(0,
                                       windowClass.lpszClassName,
                                       "char",
                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       0,
                                       0,
                                       hInst,
                                       0);

    if(windowHandle) {
        g_running = 1;
        HDC deviceContext = GetDC(windowHandle);
        while(g_running) {
            struct key_events input = {0};
            MSG msg;
            while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                if(msg.message == WM_KEYDOWN) {
                    input.codes[input.len++] = msg.wParam;
                }
                else {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            update_and_render(&buf, &input);
            RECT clientRect;
            GetClientRect(windowHandle, &clientRect);
            win32_display_buffer(deviceContext, &clientRect);
        }
    }

    return (0);
}
