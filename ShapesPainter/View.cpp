#include "View.h"
#include <windowsx.h>

template<class T> void OnPaint(T* src)
{
    HRESULT hr = src->paint.CreateGraphicsResources(src->hwnd);
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(src->hwnd, &ps);
        src->paint.pRenderTarget->BeginDraw();
        src->Paint(src->paint.pRenderTarget);
        hr = src->paint.pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            src->paint.DiscardGraphicsResources();
        }
        EndPaint(src->hwnd, &ps);
    }
}

template<class T> void OnLBClick(T* src, LPARAM lParam) 
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    if (src->control->MouseClick(xPos, yPos))
        InvalidateRect(src->hwnd, NULL, false);
}

template<class T>LRESULT HandleMessageT(T* src, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        src->paint.CreateFactory();
        return 0;

    case WM_DESTROY:
        src->paint.DiscardFactory();
        return 0;

    case WM_LBUTTONUP:
        OnLBClick(src, lParam);
        return 0;

    case WM_PAINT:
        OnPaint(src);
        return 0;
    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return 0;
    default:
        return DefWindowProc(src->hwnd, message, wParam, lParam);
    }
    return TRUE;
}


void ColorBox::Build(ColorControl* control, HWND parent)
{
    if (!Create(L"Colors", WS_BORDER, 0, ColorWindowX, ColorWindowY, ColorWindowW, ColorWindowH, parent))
        throw std::exception("Failed to Create Colors");
    this->control = control;
}

PCWSTR ColorBox::ClassName() const
{
    return L"ColorBox";
}

LRESULT ColorBox::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    return HandleMessageT(this, message, wParam, lParam);
}

void ColorBox::Paint(ID2D1HwndRenderTarget* pRenderTarget)
{
    pRenderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
    int x, y;
    x = y = ColorGap;
    int nRows = control->model.colors_no / ColorsRow;
    Color* color = control->model.colors;
    for (size_t i = 0; i < nRows; i++)
    {
        for(size_t j = 0; j < ColorsRow; j++)
        {
            color->posi = D2D1::Point2F(x, y);
            auto rec = D2D1::RectF(x, y, x + ColorWidth, y + ColorHeigth);
            paint.pBrush->SetColor(color->color);
            pRenderTarget->FillRectangle(rec, paint.pBrush);
            color++;
            x += ColorWidth + ColorGap;
        }
        x = ColorGap;
        y += ColorHeigth + ColorGap;
    }
    color = control->model.selected_color;
    x = color->posi.x;
    y = color->posi.y;
    auto rec = D2D1::RectF(x - 2, y - 2, x + ColorWidth + 2, y + ColorHeigth + 2);
    paint.pBrush->SetColor(color->color);
    pRenderTarget->DrawRectangle(rec, paint.pBrush);
}

void ShapeBox::Build(ShapeControl* control, HWND parent)
{
    if (!Create(L"Shapes", WS_BORDER, 0, ShapeWindowX, ShapeWindowY, ShapeWindowW, ShapeWindowH, parent))
        throw std::exception("Failed to Create Shapes");
    this->control = control;
}

PCWSTR ShapeBox::ClassName() const
{
    return L"ShapeBox";
}

LRESULT ShapeBox::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    return HandleMessageT(this, message, wParam, lParam);
}

void ShapeBox::Paint(ID2D1HwndRenderTarget* pRenderTarget)
{
    pRenderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
    int x, y;
    x = y = ShapeGap;
    Shape* s = control->model.shapes;
    for (size_t i = 0; i < ShapesNo; i++)
    {
        s->posi = D2D1::Point2F(x, y);
        
        switch (s->type)
        {
        case Line:
            pRenderTarget->DrawLine(
                D2D1::Point2F(x + 5, y + 5), D2D1::Point2F(x + ShapeWidth - 5, y + ShapeHeigth - 5),
                paint.pBrush, 2);
            break;
        case Rect:
            pRenderTarget->DrawRectangle(
                D2D1::RectF(x + 5, y + 5, x + ShapeWidth - 5, y + ShapeHeigth - 5),
                paint.pBrush, 2);
            break;
        case Ellip:
            pRenderTarget->DrawEllipse(D2D1::Ellipse(
                D2D1::Point2F(x + ShapeWidth / 2, y + ShapeHeigth / 2),
                ShapeWidth / 2 -5, ShapeHeigth / 2 -5),
                paint.pBrush, 2);
            break;
        case Trian:
            pRenderTarget->DrawLine(
                D2D1::Point2F(x + 5, y + 5), D2D1::Point2F(x + ShapeWidth - 5, y + ShapeHeigth - 5),
                paint.pBrush, 2);
            pRenderTarget->DrawLine(
                D2D1::Point2F(x + 5, y + 5), D2D1::Point2F(x + ShapeWidth - 5, y + 5),
                paint.pBrush, 2);
            pRenderTarget->DrawLine(
                D2D1::Point2F(x + ShapeWidth - 5, y + 5), D2D1::Point2F(x + ShapeWidth - 5, y + ShapeHeigth - 5),
                paint.pBrush, 2);
            break;
        default:
            break;
        }
        x += ShapeWidth + ShapeGap;
        s++;
    }
    s = control->model.selected_shape;
    x = s->posi.x;
    y = s->posi.y;
    auto rec = D2D1::RectF(x, y, x + ShapeWidth, y + ShapeHeigth);
    pRenderTarget->DrawRectangle(rec, paint.pBrush);
}

void PenBox::Build(PenControl* control, HWND parent)
{
    if (!Create(L"Pens", WS_BORDER, 0, PenWindowX, PenWindowY, PenWindowW, PenWindowH, parent))
        throw std::exception("Failed to Create Pens");
    this->control = control;
}

PCWSTR PenBox::ClassName() const
{
    return L"PenBox";
}

LRESULT PenBox::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    return HandleMessageT(this, message, wParam, lParam);
}

void PenBox::Paint(ID2D1HwndRenderTarget* pRenderTarget)
{
    pRenderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
    int x, y;
    x = y = PenGap;
    Pen* p = control->model.pens;
    for (size_t i = 0; i < PensNO; i++)
    {
        p->posi = D2D1::Point2F(x, y);

        pRenderTarget->DrawLine(
            D2D1::Point2F(x + 5, y + PenHeigth / 2), D2D1::Point2F(x + PenWidth - 5, y + PenHeigth / 2),
            paint.pBrush, p->stroke);
       
        y += PenHeigth + PenGap;
        p++;
    }
    p = control->model.selected_pen;
    x = p->posi.x;
    y = p->posi.y;
    auto rec = D2D1::RectF(x, y, x + PenWidth, y + PenHeigth);
    pRenderTarget->DrawRectangle(rec, paint.pBrush);
}

PCWSTR MainWindow::ClassName() const
{
    return L"MainWindow";
}

LRESULT MainWindow::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        paint.CreateFactory();
        return 0;
    case WM_DESTROY:
        paint.DiscardFactory();
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        OnPaint(this);
        return 0;
    case WM_LBUTTONDOWN:
        OnMouseDown(lParam);
        return 0;
    case WM_MOUSEMOVE:
        OnMouseMove(lParam);
        return 0;
    case WM_LBUTTONUP:
        OnMouseUp(lParam);
        return 0;
    case WM_SETCURSOR:
        SetCursor(LoadCursor(NULL, IDC_CROSS));
        return 0;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return TRUE;
}

void MainWindow::BuildWindow()
{
    if (!Create(L"Paint", WS_OVERLAPPEDWINDOW, 0, MainWindowX, MainWindowY, MainWindowW, MainWindowH))
        throw std::exception("Failed to Create Paint");
    color_box.Build(control.color_control, hwnd);
    shape_box.Build(control.shape_control, hwnd);
    pen_box.Build(control.pen_control, hwnd);
}

void MainWindow::Show(int nCmdShow)
{
    ShowWindow(hwnd, nCmdShow);
    color_box.Show(nCmdShow);
    shape_box.Show(nCmdShow);
    pen_box.Show(nCmdShow);
}

void MainWindow::OnMouseDown(LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    control.MouseDown(xPos, yPos);
}

void MainWindow::OnMouseMove(LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    if (control.MouseMove(xPos, yPos))
        InvalidateRect(hwnd, NULL, false);
}

void MainWindow::OnMouseUp(LPARAM lParam)
{
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    control.MouseUp(xPos, yPos);
}

void MainWindow::Paint(ID2D1HwndRenderTarget* pRenderTarget)
{
    pRenderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f));
    int st, en;
    for (auto s = control.model.paints.begin(); s < control.model.paints.end(); s++)
    {
        paint.pBrush->SetColor(s->color);
        switch (s->type)
        {
        case Line:
            pRenderTarget->DrawLine(
                s->start, s->end,
                paint.pBrush, s->stroke);
            break;
        case Rect:
            pRenderTarget->DrawRectangle(
                D2D1::RectF(s->start.x, s->start.y, s->end.x, s->end.y),
                paint.pBrush, s->stroke);
            break;
        case Ellip:
            pRenderTarget->DrawEllipse(D2D1::Ellipse(
                D2D1::Point2F((s->start.x + s->end.x) / 2, (s->start.y + s->end.y) / 2),
                abs((s->start.x - s->end.x) / 2), abs((s->start.y - s->end.y) / 2)),
                paint.pBrush, s->stroke);
            break;
        case Trian:
            pRenderTarget->DrawLine(
                s->start, s->end,
                paint.pBrush, s->stroke);
            pRenderTarget->DrawLine(
                s->start, D2D1::Point2F(s->end.x, s->start.y),
                paint.pBrush, s->stroke);
            pRenderTarget->DrawLine(
                D2D1::Point2F(s->end.x, s->start.y), s->end,
                paint.pBrush, s->stroke);
            break;
        default:
            break;
        }
    }
}
