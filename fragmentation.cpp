#include "fragmentation.h"

extern const double g_l1_max;
extern const double g_l2_max;
extern const double g_l1_min;
extern const double g_l2_min;
extern const double g_l0;

extern const double g_precision;

low_level_fragmentation::low_level_fragmentation(box &iBox)
{
	pending_box = iBox;
}

double low_level_fragmentation::g1(double x1, double x2)
{
	return (x1*x1 + x2 * x2 - l1_max * l1_max);
}

double low_level_fragmentation::g2(double x1, double x2)
{
	return (l1_min*l1_min - x1 * x1 - x2 * x2);
}

double low_level_fragmentation::g3(double x1, double x2)
{
	return (x1*x1 + x2 * x2 - l2_max * l2_max);
}

double low_level_fragmentation::g4(double x1, double x2)
{
	return (l2_min*l2_min - x1 * x1 - x2 * x2);
}

boxes_pair low_level_fragmentation::VerticalSplitter(box &obj)
{
	boxes_pair vertical_splitter_pair;
	double x_min, y_min, x_width, y_height;
	obj.GetBoxParameters(x_min, y_min, x_width, y_height);
	vertical_splitter_pair.first.SetBoxParameters(x_min, y_min, x_width * 0.5, y_height);
	vertical_splitter_pair.second.SetBoxParameters((x_min + x_width * 0.5), y_min, x_width*0.5, y_height);
	return vertical_splitter_pair;
}

boxes_pair low_level_fragmentation::HorizontalSplitter(box &obj)
{
	boxes_pair horizontal_splitter_pair;
	double x_min, y_min, x_width, y_height;
	obj.GetBoxParameters(x_min, y_min, x_width, y_height);
	horizontal_splitter_pair.first.SetBoxParameters(x_min, y_min, x_width, y_height*0.5);
	horizontal_splitter_pair.second.SetBoxParameters(x_min, (y_min + y_height * 0.5), x_width, y_height*0.5);
	return horizontal_splitter_pair;
}

boxes_pair low_level_fragmentation::GetNewBoxes(box &obj)
{
	boxes_pair newboxes;
	double width, height;
	obj.GetWidthHeight(width, height);
	if (width >= height)
		newboxes = VerticalSplitter(obj);
	else
		newboxes = HorizontalSplitter(obj);
	return newboxes;
}

int low_level_fragmentation::ClasifyBox(min_max_vectors &vects)
{
	if (vects.first[0] == 0 && vects.second[0] == 0)
	{
		return 0;
	}
	for (unsigned int i = 0; i < vects.first.size(); i++)
	{
		if (vects.first[i] > 0)							// проверка условия min gj(x) > 0 для любого j из 1,...,m
			return -1;									// в таком случае рассматриваемый box не входит во множество решений
	}
	bool flag_for_max = true;
	for (unsigned int j = 0; j < vects.second.size(); j++)
	{
		if (vects.second[j] > 0)						// проверка условия max gj(x) < 0 для всех j из 1,...,m	
			flag_for_max = false;
	}
	if (flag_for_max == true)
	{
		return 1;										// входит во множество решений
	}
	else
		return 2;										// подлежит разбинению и дальнейшему анализу
}

void low_level_fragmentation::GetBoxType(box &obj)
{
	int res;
	min_max_vectors min_and_max_vectors;
	boxes_pair another_boxes;
	static int my_lock;
	min_and_max_vectors = GetMinMax(obj);
	res = ClasifyBox(min_and_max_vectors);
	if (res == 0)
	{
		intersecting_boxes.push_back(obj);
	}
	else if (res == -1)
	{

		bad_boxes.push_back(obj);					// в таком случае рассматриваемый box не входит во множество решений			
	}
	else if (res == 1)
	{
		good_boxes.push_back(obj);					// входит во множество решений												
	}
	else
	{
		another_boxes = GetNewBoxes(obj);			// подлежит разбинению и дальнейшему анализу
		temporary_boxes.push_back(another_boxes.first);
		temporary_boxes.push_back(another_boxes.second);
	}
}

int low_level_fragmentation::FindTreeDepth()
{
	int tree_depth = 0;
	boxes_pair new_boxes;
	double box_diagonal = pending_box.GetBoxDiagonal();
	if (box_diagonal <= min_diagonal)
	{
		return tree_depth;
	}
	else
	{
		tree_depth = 1;
		new_boxes = VerticalSplitter(pending_box);
		box_diagonal = new_boxes.first.GetBoxDiagonal();
		if (box_diagonal <= min_diagonal)
		{
			return tree_depth;
		}
		else
		{
			for (;;)
			{
				new_boxes = GetNewBoxes(new_boxes.first);
				tree_depth++;
				box_diagonal = new_boxes.first.GetBoxDiagonal();
				if (box_diagonal <= min_diagonal)
					break;
				else
					continue;
			}
			return tree_depth;
		}
	}
}

min_max_vectors high_level_analysis::GetMinMax(box &iBox)
{
	std::vector<double> g_min;
	std::vector<double> g_max;
	min_max_vectors result;
	double curr_box_diagonal = iBox.GetBoxDiagonal();
	double a1min, a2min;
	double a1max, a2max;
	double xmin, xmax, ymin, ymax;
	int count = 0;
	iBox.GetBoxParameters(xmin, ymin, xmax, ymax);
	xmax = xmin + xmax;
	ymax = ymin + ymax;

	if (curr_box_diagonal <= min_diagonal)
	{
		g_min.push_back(0);
		g_max.push_back(0);
		result.first = g_min;
		result.second = g_max;
		return result;
	}
	// MIN
	// функция g1(x1,x2)
	a1min = __min(abs(xmin), abs(xmax));
	a2min = __min(abs(ymin), abs(ymax));
	g_min.push_back(g1(a1min, a2min));
	// функция g2(x1,x2)
	a1min = __max(abs(xmin), abs(xmax));
	a2min = __max(abs(ymin), abs(ymax));
	g_min.push_back(g2(a1min, a2min));
	// функция g3(x1,x2)
	a1min = __min(abs(xmin - l0), abs(xmax - l0));
	a2min = __min(abs(ymin), abs(ymax));
	g_min.push_back(g3(a1min, a2min));
	// функция g4(x1,x2)
	a1min = __max(abs(xmin - l0), abs(xmax - l0));
	a2min = __max(abs(ymin), abs(ymax));
	g_min.push_back(g4(a1min, a2min));

	// MAX
	// функция g1(x1,x2)
	a1max = __max(abs(xmin), abs(xmax));
	a2max = __max(abs(ymin), abs(ymax));
	g_max.push_back(g1(a1max, a2max));
	// функция g2(x1,x2)
	a1max = __min(abs(xmin), abs(xmax));
	a2max = __min(abs(ymin), abs(ymax));
	g_max.push_back(g2(a1max, a2max));
	// функция g3(x1,x2)
	a1max = __max(abs(xmin - l0), abs(xmax - l0));
	a2max = __max(abs(ymin), abs(ymax));
	g_max.push_back(g3(a1max, a2max));
	// функция g4(x1,x2)
	a1max = __min(abs(xmin - l0), abs(xmax - l0));
	a2max = __min(abs(ymin), abs(ymax));
	g_max.push_back(g4(a1max, a2max));

	result.first = g_min;
	result.second = g_max;
	return result;
}

void high_level_analysis::GetSolution()
{
	//  initial box parameters
	pending_box.SetBoxParameters((-1)*l1_max, 0.0, l1_max + l0 + l2_max, __min(l1_max, l2_max));
	int tree_depth = FindTreeDepth();
	temporary_boxes.push_back(pending_box);
	//cilk::reducer <cilk::op_vector<box>> curr_boxes;
	for (int level = 0; level < (tree_depth + 1); level++)
	{
		int number_of_box_on_level = temporary_boxes.size();
		std::vector<box> curr_boxes(temporary_boxes);
		//cilk::reducer<box> curr_boxes(temporary_boxes);
		temporary_boxes.clear();
		for (int i = 0; i < number_of_box_on_level; i++)
		{
			GetBoxType(curr_boxes[i]);
		}
	}
}


void high_level_analysis::WriteResults(std::string istr)
{
	std::ofstream fout(istr);
	if (!fout.is_open())
	{
		printf("Cann't open file!\n");
		return;
	}
	else
	{

		double x_min, y_min, width, height;
		for (auto box : good_boxes) {
			box.GetBoxParameters(x_min, y_min, width, height);
			fout << x_min << " " << y_min << " " << width << " " << height;
			fout << "\n";
		}
		for (auto box : intersecting_boxes) {
			box.GetBoxParameters(x_min, y_min, width, height);
			fout << x_min << " " << y_min << " " << width << " " << height;
			fout << "\n";
		}
		for (auto box : bad_boxes) {
			box.GetBoxParameters(x_min, y_min, width, height);
			fout << x_min << " " << y_min << " " << width << " " << height;
			fout << "\n";
		}
		fout.close();
	}
}