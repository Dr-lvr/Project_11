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
	//int machine_view_test();
	cv::Mat machine_view_test(cv::Mat src);
	void start_view();
private:
	inline static const char* image_window = "Source Image";
	inline static const char* result_window = "Result window";
	/*
	inline static bool use_mask;
	inline static cv::Mat img;
	inline static cv::Mat templ;
	inline static cv::Mat mask;
	inline static cv::Mat result;
	inline static int match_method;
	inline static const char* image_window = "Source Image";
	inline static const char* result_window = "Result window";
	static void MatchingMethod(int, void*);

	int max_Trackbar = 5;
	*/
	cv::Mat hwnd2mat(HWND hwnd);
};

