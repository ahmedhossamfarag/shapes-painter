#include "Model.h"

bool Pen::Contains(D2D1_POINT_2F point)
{
    return point.x - posi.x < PenWidth && point.y - posi.y < PenHeigth;
}

PenModel::PenModel()
{
    pens = new Pen[PensNO];
	int stroke = 1;
	for (size_t i = 0; i < PensNO; i++)
	{
		pens[i] = Pen{ stroke };
		stroke += PenDelta;
	}
	selected_pen = pens;
}

bool Color::Contains(D2D1_POINT_2F point)
{
	return point.x - posi.x < ColorWidth && point.y - posi.y < ColorHeigth;
}

ColorModel::ColorModel()
{
	int colorN = (1.0 / ColorDelta) + 1;
	colors_no = colorN * colorN * colorN;
	colors = new Color[colors_no];
	Color* c = colors;
	for (float r = 0; r <= 1; r += ColorDelta)
	{
		for (float g = 0; g <= 1; g += ColorDelta)
		{
			for (float b = 0; b <= 1; b += ColorDelta)
			{
				*c = Color{ D2D1::ColorF(r, g, b) };
				c++;
			}
		}
	}
	selected_color = colors;
}

bool Shape::Contains(D2D1_POINT_2F point)
{
	return point.x - posi.x < ShapeWidth && point.y - posi.y < ShapeHeigth;
}

ShapeModel::ShapeModel()
{
	shapes = new Shape[ShapesNo];
	for (size_t i = 0; i < ShapesNo; i++)
	{
		shapes[i] = Shape{ (ShapeType)i };
	}
	selected_shape = shapes;
}

inline int Paint::Width()
{
	return end.x - start.x;
}

inline int Paint::Height()
{
	return end.y - start.y;
}
