#pragma once
#include <vector>
#include <d2d1.h>
#include "Declarations.h"

struct Pen
{
public:
	int stroke;
	D2D1_POINT_2F posi;

	bool Contains(D2D1_POINT_2F point);
};

struct PenModel
{
public:
	Pen* pens;
	Pen* selected_pen;

	PenModel();
};

struct Color
{
public:
	D2D1_COLOR_F color;
	D2D1_POINT_2F posi;

	bool Contains(D2D1_POINT_2F point);
};

struct ColorModel
{
public:
	int colors_no;
	Color* colors;
	Color* selected_color;

	ColorModel();
};

enum ShapeType
{
	Line,
	Rect,
	Ellip,
	Trian,
};

struct Shape
{
public:
	ShapeType type;
	D2D1_POINT_2F posi;

	bool Contains(D2D1_POINT_2F point);
};

struct ShapeModel
{
public:
	Shape* shapes;
	Shape* selected_shape;

	ShapeModel();
};

struct Paint
{
public:
	ShapeType type;
	D2D1_COLOR_F color;
	int stroke;
	D2D1_POINT_2F start;
	D2D1_POINT_2F end;

	inline int Width();
	inline int Height();
};

struct PaintModel
{
public:
	std::vector<Paint> paints;
};

