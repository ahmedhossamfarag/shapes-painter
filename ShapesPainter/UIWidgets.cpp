#include "UIWidgets.h"
#include <WindowsX.h>

WidgetPaint::WidgetPaint() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
{
}

void WidgetPaint::CreateFactory()
{
    if (FAILED(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
    {
       throw std::exception("Failed To Create Factory");
    }
}

HRESULT WidgetPaint::CreateGraphicsResources(HWND hwnd)
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &pRenderTarget);
        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
        }
    }
    return hr;
}

void WidgetPaint::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void WidgetPaint::DiscardFactory()
{
    DiscardGraphicsResources();
    SafeRelease(&pFactory);
}


