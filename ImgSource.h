#pragma once
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <string>
#include "cqueue.h"
#include "utility.h"
#include <thread>

/*These are two preprocessor directive used to showcase if threading and debugging are used or not
in the code execution, when with_threading is enabled, threads are implemented in each class for 
faster execution. When with_debugging is enabled, intermediate results are displayed on console*/

#define WITH_THREADING
//#define WITH_DEBUGGING

//This class has functions for sourcing images in parallel from input folder in FIFO manner
//reads the image in greyscale and stores in buffer 

class Source
{
	const std::string Path;
	cqueue<cv::Mat> Fifo;
	std::queue<std::string> Names;
	std::thread Task;
public:
	void fill()
	{
		for (const auto& entry : std::filesystem::directory_iterator(Path))
		{
#ifdef WITH_DEBUGGING
			std::cout << entry << std::endl;
#endif	
			if (entry.is_regular_file())
			{
				auto t00 = std::chrono::high_resolution_clock::now();
				//reads images from the path specified in grayscale format
				cv::Mat image = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
				//will push the image only if it is not empty
				if (image.data != NULL)
				{
					Fifo.push(image);
				}
#ifdef WITH_DEBUGGING
				utility::display_time(t00, std::chrono::high_resolution_clock::now());
#endif	
			}
		}
		Fifo.push(cv::Mat());

	}
	cv::Mat getNext()
	{
		cv::Mat img;
		Fifo.pop(img);
		return img;
	}
	Source(const std::string& path, int size = 10) : Path(path), Fifo(size)
	{
#ifdef WITH_THREADING
		Task = std::thread([&] {
#endif
			fill();
#ifdef WITH_THREADING
			});
		Task.detach();
#endif
	}
};
