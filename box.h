#pragma once
#include <windows.h>
#include <vector>
/// —труктура Box описывает конкретный экземпл€р box-а
typedef struct Box
{
protected:
	double x_min;			// лева€ координата по оси х box'a
	double y_min;			// нижн€€ лева€ координата по оси у box'a
	double width;			// длина box'a
	double height;			// высота box'a
public:
	void SetBoxParameters(double min_x, double min_y, double x_width, double y_height);
	void GetBoxParameters(double &min_x, double &min_y, double &x_width, double &y_height);
	void GetWidthHeight(double &x_width, double &y_height);
	double GetBoxDiagonal();
}box;


typedef std::pair<box, box> boxes_pair;
typedef std::pair<std::vector<double>, std::vector<double>> min_max_vectors;
