#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <numeric>

struct peaks
{
	std::vector<int> index;
	std::vector<double> value;
};

class Evaluation
{
public:

	std::vector<double> static Mean0R(const cv::Mat &image2);
	std::vector<double> static Mean1R(const cv::Mat &image2);

	//Function for calculating mean value of given input vector of any data type T
	template<class T>
	double static MeanR(const std::vector<T>& mean0)
	{
		const T sum = std::accumulate(mean0.begin(), mean0.end(), (T)0);
		return (double)sum / mean0.size();
	}

	double static Mean(std::vector<double>::const_iterator start, std::vector<double>::const_iterator end);
	double static Median(std::vector<double> array);
	std::vector<int> static ArgSort(const std::vector<double>& s_dic);
	double static std_dev(const std::vector<double>& arr, int start, int stop);
	
	//Function for calculating the decumulating values of the input vector of any data type
	template<class T>
	static std::vector<T> decumulate(const std::vector<T>& x)
	{
		std::vector<T> xi;
		if (x.size() <= 1)
		{
			std::vector<T> xi;
		}
		
		else
		{
			const size_t n = x.size() - 1;
			xi.resize(n);
			for (int i = 0; i < n; i++)
			{
				xi[i] = x[i + 1] - x[i];
			}
		}
		return xi;
	}

	peaks static Find_Peaks(const std::vector<double>& arr, double height, double dist, double prom);
};

