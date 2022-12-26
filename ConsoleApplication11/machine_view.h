#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <Windows.h>
class machine_view
{
public:
	std::pair<int, int> start_view(std::string path);
private:
	bool show_monitors = false;
	inline static const char* image_window = "Source Image";
	inline static const char* result_window = "Result window";
	void show_match_histogram(std::string src_name, cv::Mat src);
	double compare_histogram(cv::Mat template_hist, cv::Mat comparison);
	std::vector<std::pair<cv::Point, double>> machine_vision(cv::Mat src, std::string path);
	cv::Mat hwnd2mat(HWND hwnd);
};

