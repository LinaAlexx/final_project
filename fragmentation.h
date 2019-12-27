#pragma once
#include "box.h"
#include <vector>
#include <fstream>

//#include <cilk/reducer_vector.h>
//#include <cilk/cilk.h>
//#include <cilk/cilk_api.h>

typedef std::pair<box, box> boxes_pair;
typedef std::pair<std::vector<double>, std::vector<double>> min_max_vectors;

extern const double l1_max;
extern const double l2_max;
extern const double l1_min;
extern const double l2_min;
extern const double l0;

extern const double min_diagonal;

class low_level_fragmentation
{
protected:
	box pending_box;
	std::vector<box> good_boxes;
	std::vector<box> bad_boxes;
	std::vector<box> intersecting_boxes;
	std::vector<box> temporary_boxes;
	//cilk::reducer<box> temporary_boxes;
	boxes_pair VerticalSplitter(box &obj);								// разбивает переданный в качестве параметра box по ширине
	boxes_pair HorizontalSplitter(box &obj);							// разбивает переданный в качестве параметра box по высоте
	boxes_pair GetNewBoxes(box &obj);									// разбивает переданный в качестве параметра box по большей стороне
																		// вызывая VerticalSplitter() или HorizontalSplitter()
	int FindTreeDepth();												// возвращает глубину двоичного дерева, которая соответствует количеству итераций алгоритма
	int ClasifyBox(min_max_vectors &vects);								// анализирует box и классифицирует его
	void GetBoxType(box &obj);                                          // добавляет классифицированный ранее box во множество решений, 
																		// или удаляет его из анализа, или добавляет его к граничной области, 
																		// или относит его к тем, что подлежат дальнейшему анализу

	double g1(double x1, double x2);
	double g2(double x1, double x2);
	double g3(double x1, double x2);
	double g4(double x1, double x2);
	virtual min_max_vectors GetMinMax(box &iBox) = 0;					// получение минимумов и максимумов для функций gj
public:
	low_level_fragmentation() {}
	~low_level_fragmentation() {}
	low_level_fragmentation(box &iBox);
};


class high_level_analysis : public low_level_fragmentation
{
protected:
	min_max_vectors GetMinMax(box &iBox);
public:
	high_level_analysis() {}
	high_level_analysis(box &iBox) : low_level_fragmentation(iBox) {}
	void GetSolution();
	void WriteResults(std::string istr);
};