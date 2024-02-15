#include "Control.h"

bool ShapeControl::MouseClick(int x, int y)
{
	auto p = D2D1::Point2F(x, y);
	auto end = model.shapes + ShapesNo;
	for (Shape* s = model.shapes; s < end; s++)
	{
		if (s->Contains(p))
		{
			if (model.selected_shape != s)
			{
				model.selected_shape = s;
				return true;
			}
			return false;
		}
	}
	return false;
}

ShapeType ShapeControl::selected_type()
{
	return model.selected_shape->type;
}

bool ColorControl::MouseClick(int x, int y)
{
	auto p = D2D1::Point2F(x, y);
	auto end = model.colors + model.colors_no;
	for (Color* c = model.colors; c < end; c++)
	{
		if (c->Contains(p))
		{
			if (model.selected_color != c)
			{
				model.selected_color = c;
				return true;
			}
			return false;
		}
	}
	return false;
}

D2D1_COLOR_F ColorControl::selected_color()
{
	return model.selected_color->color;
}

bool PenControl::MouseClick(int x, int y)
{
	auto p = D2D1::Point2F(x, y);
	auto end = model.pens + PensNO;
	for (Pen* pn = model.pens; pn < end; pn++)
	{
		if (pn->Contains(p))
		{
			if (model.selected_pen != pn)
			{
				model.selected_pen = pn;
				return true;
			}
			return false;
		}
	}
	return false;
}

int PenControl::selected_stroke()
{
	return model.selected_pen->stroke;
}

PaintControl::PaintControl()
{
	shape_control = new ShapeControl();
	color_control = new ColorControl();
	pen_control = new PenControl();
	current_paint = nullptr;
}

void PaintControl::MouseDown(int x, int y)
{
	Paint p{ shape_control->selected_type(), color_control->selected_color(), pen_control->selected_stroke()};
	p.start = p.end = D2D1::Point2F(x, y);
	model.paints.push_back(p);
	current_paint = model.paints.data() + model.paints.size() - 1;
}

bool PaintControl::MouseMove(int x, int y)
{
	if(current_paint == nullptr)
		return false;
	current_paint->end = D2D1::Point2F(x, y);
	return true;
}

void PaintControl::MouseUp(int x, int y)
{
	current_paint = nullptr;
}
