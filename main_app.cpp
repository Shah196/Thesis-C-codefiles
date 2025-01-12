#include "main.h"
#include <string>
#include <thread>
#include "utility.h"
#include "constants.h"
#include "cqueue.h"
#include "ImgSource.h"
#include "raw_edges.h"
#include "find_edges.h"
#include "grid_pos01.h"
#include "grid_pos02.h"
#include "grid_pos03.h"
#include "grid.h"
#include "Results.h"

//This is the main file for algorithm implementation where classes are executed one after other for processing
//the images in threads and results stored in a csv file


// This function creates a csv file which is used for storing the results of various images at the same location
void create_csv(const std::string& filename, const vector<std::string>& colname)
{
	// Create an output filestream object
	ofstream myFile(filename);

	for (int j = 0; j < colname.size(); ++j)
	{
		myFile << colname.at(j);
		if (j != colname.size() - 1) myFile << ","; // No comma at end of line
	}
	myFile << "\n";
}

using namespace cv;
using namespace std;

std::mutex output_mutex;

int main(int argc, char* argv[])
{

//pre processor ifdef is used for with_testing when testing was needed to be performed for 100 iterations for 
//getting the execution time data, test_count is the number of times testing needs to be done

#ifdef WITH_TESTING
	int test_count = 5;
	ofstream myFile("Timing_Results");
#endif

	string csvname = "Result";
	std::vector<std::string> colname = { "   xi    ","   zi    ","    k    "," index ","  orientation " };
	create_csv(csvname, colname);

	
#ifdef WITH_TESTING
	for (size_t test_i = 0; test_i < test_count; test_i++)
	{
#endif

//enter the path of the folder where images to be processed are stored
	auto t01 = std::chrono::high_resolution_clock::now();
	//string path = "D:\\Vaishnavi\\C++Trial\\Images";
	string path = "D:\\Vaishnavi\\Master Thesis\\BIMAQ\\New images\\not_matching";
	//string path = "E:\\Set3";

	if (argc > 1)
		path = argv[1];

#ifdef WITH_THREADING
		Source imgsrc(path,2);
#else
		Source imgsrc(path);
#endif	

		raw_edges edge0(imgsrc, freq_range);
		find_edges edge(edge0);
		grid_pos01 grid1(edge);
		grid_pos02 grid2(grid1);
		grid_pos03 grid3(grid2);
		Results output(grid3, t01);

#ifdef WITH_THREADING
	while (1);
#endif

#ifdef WITH_TESTING
	auto t00 = utility::get_time(t01, std::chrono::high_resolution_clock::now());
	myFile << t00;
	myFile << "\n";
	}
#endif
	return 0;
}
 
