#include "box.h"
#include <cmath>


void Box::SetBoxParameters(double min_x, double min_y, double x_width, double y_height)
{
	x_min = min_x;
	y_min = min_y;
	width = x_width;
	height = y_height;
}

void Box::GetBoxParameters(double &min_x, double &min_y, double &x_width, double &y_height)
{
	min_x = x_min;
	min_y = y_min;
	x_width = width;
	y_height = height;
}

void Box::GetWidthHeight(double &x_width, double &y_height)
{
	x_width = width;
	y_height = height;
}

double Box::GetBoxDiagonal()
{
	double x_width, y_height;
	GetWidthHeight(x_width, y_height);
	return sqrt(x_width*x_width + y_height * y_height);
}
