#include "stdafx.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HDC g_hdc;

void WndProc_OnDestroy(HWND);
void WndProc_OnMouseMove(HWND, int, int, UINT);
void WndProc_OnLButtonDown(HWND, BOOL, int, int, UINT);

LPTSTR g_lpszClassName = TEXT("ObserveMouseMove");
LPTSTR g_lpszApplicationTitle = TEXT("Developer: Dyagel Andrew");


int APIENTRY _tWinMain(HINSTANCE This,
                       HINSTANCE hPrevInstance,
                       LPTSTR lpszCmdLine,
                       int mode)
{
    HWND hWnd;
    MSG msg;
    WNDCLASSEX wc;

    memset(&wc, 0, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = This;
    wc.lpszClassName = g_lpszClassName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(This, MAKEINTRESOURCE(IDI_ICON_LOGO));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    wc.hIconSm = NULL;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Error register window class!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindowEx(NULL,
                          g_lpszClassName,
                          g_lpszApplicationTitle,
                          WS_OVERLAPPEDWINDOW,
                          350,
                          200,
                          500,
                          500,
                          HWND_DESKTOP,
                          NULL,
                          This,
                          NULL);
    if(!hWnd)
    {
        MessageBox(NULL, TEXT("Can't create window!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    g_hdc = GetDC(hWnd);

    ShowWindow(hWnd, mode);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    switch(msg)
    {
        HANDLE_MSG(hwnd, WM_DESTROY, WndProc_OnDestroy);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, WndProc_OnMouseMove);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, WndProc_OnLButtonDown);
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void WndProc_OnDestroy(HWND hwnd)
{
    ReleaseDC(hwnd, g_hdc);
    PostQuitMessage(0);
    FORWARD_WM_DESTROY(hwnd, DefWindowProc);
}

void WndProc_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    HDC hdc;
    size_t sizeBuffer = 150;
    LPTSTR lpszBuffer = (LPTSTR)calloc(sizeBuffer, sizeof(wchar_t));
    hdc = GetDC(hwnd);

    StringCchPrintf(lpszBuffer, sizeBuffer, TEXT("WM_MOUSEMOVE --> x: %d; y: %d; keyFlags: %u"),
                    x, y, keyFlags);
    TextOut(hdc, 10, 10, (LPCTSTR)lpszBuffer, sizeBuffer);

    if((WPARAM)keyFlags & MK_LBUTTON)
    {
        SetTextColor(hdc, RGB(0, 255, 0));
        LineTo(g_hdc, x, y);
    }

    ReleaseDC(hwnd, hdc);
    FORWARD_WM_MOUSEMOVE(hwnd, x, y, keyFlags, DefWindowProc);
}

void WndProc_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    MoveToEx(g_hdc, x, y, NULL);
    FORWARD_WM_LBUTTONDOWN(hwnd, fDoubleClick, x, y, keyFlags, DefWindowProc);
}