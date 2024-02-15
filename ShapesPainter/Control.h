#pragma once
#include "Model.h"

class ShapeControl
{
public:
	ShapeModel model;

	bool MouseClick(int x, int y);

	ShapeType selected_type();
};

class ColorControl
{
public:
	ColorModel model;


	bool MouseClick(int x, int y);

	D2D1_COLOR_F selected_color();
};

class PenControl
{
public:
	PenModel model;


	bool MouseClick(int x, int y);

	int selected_stroke();
};

class PaintControl
{
public:
	PaintModel model;
	ShapeControl* shape_control;
	ColorControl* color_control;
	PenControl* pen_control;

	Paint* current_paint;

	PaintControl();

	void MouseDown(int x, int y);

	bool MouseMove(int x, int y);

	void MouseUp(int x, int y);
};

