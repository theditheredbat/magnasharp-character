#include <Windows.h>
#include <glad/glad.h>

#include "application.h"
#include "fileio.h"
#include "platform.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

typedef HGLRC WINAPI CREATECONTEXTATTRIBPROC(HDC hdc, HGLRC hShareContext, const int *attribList);
CREATECONTEXTATTRIBPROC *wglCreateContextAttribsARB;

internal int g_running;

LRESULT CALLBACK
win32_window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT res = 0;
    switch(uMsg) {
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
    if(!windowHandle)
        return (-1);
    // Separate code into functions
    g_running = 1;
    HDC deviceContext = GetDC(windowHandle);
    PIXELFORMATDESCRIPTOR pform = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 24,
        .cAccumBits = 8,
        .iLayerType = PFD_MAIN_PLANE
    };
    int formnum = ChoosePixelFormat(deviceContext, &pform); // Check for zero
    SetPixelFormat(deviceContext, formnum, &pform);
    HGLRC tempGLContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, tempGLContext);
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    wglCreateContextAttribsARB = (CREATECONTEXTATTRIBPROC *)wglGetProcAddress("wglCreateContextAttribsARB");
    gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
    HGLRC glContext = wglCreateContextAttribsARB(deviceContext, 0, attribs);
    wglMakeCurrent(deviceContext, glContext);
    wglDeleteContext(tempGLContext);
    while(g_running) {
        struct key_events input = {0};
        MSG msg;
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_KEYDOWN && input.len < 50) {
                input.codes[input.len++] = msg.wParam;
            }
            else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        app_update_and_render(&input);
        SwapBuffers(deviceContext);
    }

    return (0);
}
