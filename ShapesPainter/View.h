#pragma once
#include "UIWidgets.h"
#include "Control.h"

class ColorBox : public BaseWindow<ColorBox>
{
public:
    ColorControl* control;
    WidgetPaint paint;

    void Build(ColorControl* control, HWND parent);
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void Paint(ID2D1HwndRenderTarget* pRenderTarget);
};

class ShapeBox : public BaseWindow<ShapeBox>
{
public:
    ShapeControl* control;
    WidgetPaint paint;

    void Build(ShapeControl* control, HWND parent);
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void Paint(ID2D1HwndRenderTarget* pRenderTarget);
};

class PenBox : public BaseWindow<PenBox>
{
public:
    PenControl* control;
    WidgetPaint paint;

    void Build(PenControl* control, HWND parent);
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void Paint(ID2D1HwndRenderTarget* pRenderTarget);
};

class MainWindow : public BaseWindow<MainWindow>
{
public:
    PaintControl control;
    ColorBox color_box;
    ShapeBox shape_box;
    PenBox pen_box;

    WidgetPaint paint;

    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void BuildWindow();
    void Show(int nCmdShow) override;
    void OnMouseDown(LPARAM lParam);
    void OnMouseMove(LPARAM lParam);
    void OnMouseUp(LPARAM lParam);
    void Paint(ID2D1HwndRenderTarget* pRenderTarget);
};
