#include <iostream>
#include "fragmentation.h"
#include <chrono>



using namespace std;
using namespace std::chrono;

// initial box parameters
const double l1_max = 12.0;
const double l2_max = l1_max;
const double l1_min = 8.0;
const double l2_min = l1_min;
const double l0 = 5.0;
// predefined presicion
const double min_diagonal = 0.25;

int main()
{
	//	__cilkrts_end_cilk();
	high_level_analysis main_object;
	//	__cilkrts_set_param("nworkers", "1");
	//	cout << "Number of workers " << __cilkrts_get_nworkers() << endl;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	main_object.GetSolution();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	cout << "Elapsed time is: " << time_span.count() << " seconds" << endl;

	string good_boxes_file = "D:/robot_output/GoodBoxes.txt";
	string bad_boxes_file = "D:/robot_output/BadBoxes.txt";
	string intersecting_boxes_file = "D:/robot_output/IntersectingBoxes.txt";
	main_object.WriteResults(good_boxes_file);
	main_object.WriteResults(intersecting_boxes_file);
	main_object.WriteResults(bad_boxes_file);

	return 0;
}