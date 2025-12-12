#include <Windows.h>
#include <glad/glad.h>

#include "application.h"
#include "fileio.h"
#include "platform.h"

internal void
create_opengl_context(HWND hwnd, HDC dev_context)
{
#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

    typedef HGLRC WINAPI (*PFNCREATECONTEXTATTRIBPROC)(HDC hdc, HGLRC hShareContext, const int *attribList);
    PFNCREATECONTEXTATTRIBPROC wglCreateContextAttribsARB;

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
    int formnum = ChoosePixelFormat(dev_context, &pform); // Check for zero
    SetPixelFormat(dev_context, formnum, &pform);
    HGLRC dummy_context = wglCreateContext(dev_context);
    wglMakeCurrent(dev_context, dummy_context);

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    wglCreateContextAttribsARB = (PFNCREATECONTEXTATTRIBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    HGLRC cool_context = wglCreateContextAttribsARB(dev_context, 0, attribs);
    wglMakeCurrent(dev_context, cool_context);
    wglDeleteContext(dummy_context);

    gladLoadGLLoader((GLADloadproc)wglGetProcAddress);

#undef WGL_CONTEXT_MAJOR_VERSION_ARB
#undef WGL_CONTEXT_MINOR_VERSION_ARB
#undef WGL_CONTEXT_PROFILE_MASK_ARB
#undef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
}

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
    WNDCLASS window_class = {
        .style = CS_OWNDC | CS_VREDRAW | CS_VREDRAW,
        .lpfnWndProc = win32_window_callback,
        .hInstance = hInst,
        .lpszClassName = "win32WindowClass",
    };
    RegisterClass(&window_class);

    HWND window_handle = CreateWindowEx(0,
                                       window_class.lpszClassName,
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
    if(!window_handle)
        return (-1);
    HDC dev_context = GetDC(window_handle);
    create_opengl_context(window_handle, dev_context);
    g_running = 1;
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
        SwapBuffers(dev_context);
    }

    return (0);
}
