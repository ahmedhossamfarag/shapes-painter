#pragma once
#include <Windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Declarations.h"
#include <functional>

template<class WIDGET>
class BaseWindow
{
public:
	HWND hwnd;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
   
    BaseWindow() : hwnd(NULL) {}
    
    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    );

    RECT GetBounds();

    virtual void Show(int nCmdShow);

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

};


template<class WIDGET>
inline LRESULT BaseWindow<WIDGET>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WIDGET* pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (WIDGET*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->hwnd = hwnd;
    }
    else
    {
        pThis = (WIDGET*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

template<class WIDGET>
inline BOOL BaseWindow<WIDGET>::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WIDGET::WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = ClassName();
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
        nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
    );

    return (hwnd ? TRUE : FALSE);
}

template<class WIDGET>
inline RECT BaseWindow<WIDGET>::GetBounds()
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    return rc;
}

template<class WIDGET>
inline void BaseWindow<WIDGET>::Show(int nCmdShow)
{
    ShowWindow(hwnd, nCmdShow);
}

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class WidgetPaint
{
public:
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;

    WidgetPaint();

    void CreateFactory();
    virtual HRESULT CreateGraphicsResources(HWND hwnd);
    virtual void DiscardGraphicsResources();
    void DiscardFactory();
};