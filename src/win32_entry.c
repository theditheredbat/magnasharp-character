#include <Windows.h>

#include <stdint.h>

#define internal static

typedef struct {
    int width, height;
    int bytesPerPixel;
    void *memory;
    BITMAPINFO info;
} win32_back_buffer;

internal int g_running;
internal win32_back_buffer g_buffer;

internal void
render_gradient(win32_back_buffer *buf, int xOffset, int yOffset) {
    uint32_t *pixel = buf->memory;
    for(int r = 0; r < buf->height; r++) {
        for(int c = 0; c < buf->width; c++) {
            uint8_t red = (r + yOffset);
            uint8_t green = (c + xOffset);
            uint8_t blue = 0;

            *pixel++ = (red << 16) | (green << 8) | blue;
        }
    }
}

internal void
win32_create_dib_section(win32_back_buffer *buf, int width, int height) {
    buf->width = width;
    buf->height = height;
    buf->bytesPerPixel = 4;
    
    buf->info.bmiHeader.biSize = sizeof(buf->info.bmiHeader);
    buf->info.bmiHeader.biWidth = buf->width;
    buf->info.bmiHeader.biHeight = -buf->height;
    buf->info.bmiHeader.biPlanes = 1;
    buf->info.bmiHeader.biBitCount = buf->bytesPerPixel * 8;
    buf->info.bmiHeader.biCompression = BI_RGB;

    int bMMemSize = buf->width * buf->height * buf->bytesPerPixel;
    buf->memory = VirtualAlloc(0, bMMemSize, MEM_COMMIT, PAGE_READWRITE);
}

internal void
win32_display_buffer(const win32_back_buffer *buf, HDC deviceContext, const RECT *clientRect) {
    int windowWidth = clientRect->right - clientRect->left;
    int windowHeight = clientRect->bottom - clientRect->top;
    StretchDIBits(deviceContext,
                  0, 0,
                  windowWidth, windowHeight,
                  0, 0,
                  buf->width, buf->height,
                  buf->memory,
                  &buf->info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

LRESULT CALLBACK
win32_window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT res = 0;
    switch(uMsg) {
        case WM_PAINT:
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(hWnd, &paint);
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            win32_display_buffer(&g_buffer, deviceContext, &clientRect);
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
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
    win32_create_dib_section(&g_buffer, 1280, 720);
    
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
        int xOffset = 0;
        int yOffset = 0;
        HDC deviceContext = GetDC(windowHandle);
        while(g_running) {
            MSG msg;
            while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            render_gradient(&g_buffer, xOffset, yOffset);
            RECT clientRect;
            GetClientRect(windowHandle, &clientRect);
            win32_display_buffer(&g_buffer, deviceContext, &clientRect);
            xOffset++;
            yOffset++;
        }
    }

    return (0);
}
