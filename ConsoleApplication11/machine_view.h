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
	void start_view();
private:
	inline static const char* image_window = "Source Image";
	inline static const char* result_window = "Result window";
	void show_match_histogram(std::string src_name, cv::Mat src);
	cv::Mat machine_view_test(cv::Mat src);
	cv::Mat hwnd2mat(HWND hwnd);
};

