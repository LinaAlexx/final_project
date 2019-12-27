#pragma once
#include <windows.h>
#include <vector>
/// ��������� Box ��������� ���������� ��������� box-�
typedef struct Box
{
protected:
	double x_min;			// ����� ���������� �� ��� � box'a
	double y_min;			// ������ ����� ���������� �� ��� � box'a
	double width;			// ����� box'a
	double height;			// ������ box'a
public:
	void SetBoxParameters(double min_x, double min_y, double x_width, double y_height);
	void GetBoxParameters(double &min_x, double &min_y, double &x_width, double &y_height);
	void GetWidthHeight(double &x_width, double &y_height);
	double GetBoxDiagonal();
}box;


typedef std::pair<box, box> boxes_pair;
typedef std::pair<std::vector<double>, std::vector<double>> min_max_vectors;
