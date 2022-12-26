#include "machine_view.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/core.hpp>

#include <Windows.h>
#include <iostream>
#include <string>
#include <utility>

#include "kernel.h"
#include "db_test.h"

cv::Mat machine_view::hwnd2mat(HWND hwnd){

    //handle the stream from window to mat
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi{ 0 };

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
    width = windowsize.right / 1;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}
std::vector<std::pair<cv::Point, double>> machine_view::machine_vision(cv::Mat src, std::string path) {

    kernel k;
    std::vector<std::pair<cv::Point, double>> pseudo_tensor;
    //! [load_images]
    const std::string img2 = path; //image path
    cv::Mat screen=src;
    cv::Mat cvt = cv::imread(img2, cv::IMREAD_COLOR);
    //! [load_images]
    //! [convert2depth24]
    cv::Mat tmp;
    cv::cvtColor(cvt, tmp, cv::COLOR_BGR2BGRA);
    if (show_monitors) {
        //! [create_windows]
        cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
        cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);

        cv::moveWindow(image_window, 1918, 205);
        cv::moveWindow(result_window, 1918, 205);
        //! [create_windows]
    }
    cv::Mat img_display;
    screen.copyTo(img_display);
    //! [copy_source]

    //! [create_result_matrix]
    int result_cols = screen.cols - tmp.cols + 1;
    int result_rows = screen.rows - tmp.rows + 1;

    cv::Mat res;
    res.create(result_rows, result_cols, CV_32FC1);

    //! [template monitor]
    //show_match_histogram("template", tmp);
    for (int i = 0; i < 6; ++i) {
        cv::matchTemplate(screen, tmp, res, i);

        //! [normalize]
        cv::normalize(res, res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
        //! [normalize]

        //! [best_match]
        /// localizing the best match with minMaxLoc
        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;
        minMaxLoc(res, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

        if (std::cmp_less_equal(i, 1))
        {
            matchLoc = minLoc;
        }
        else
        {
            matchLoc = maxLoc;
        }
        //! [match_loc]

        //! [imshow]
        if (show_monitors) {
            rectangle(img_display, matchLoc, cv::Point(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows), cv::Scalar(0, 255, 9), 2, 8, 0);
            rectangle(res, matchLoc, cv::Point(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows), cv::Scalar(0, 255, 9), 2, 8, 0);
        }

        //processing match with histograms
        cv::Mat submat = cv::Mat(img_display, cv::Rect(matchLoc, cv::Point(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows)));
        //! [match monitor]
        //show_match_histogram(std::to_string(i), submat);
        //compare_histogram(tmp, submat);
        pseudo_tensor.push_back(
            std::make_pair(
                cv::Point((matchLoc.x + (matchLoc.x + tmp.cols)) / 2, (matchLoc.y + (matchLoc.y + tmp.rows)) / 2), 
                compare_histogram(tmp, submat)));
    }
    /*
    for (auto &a : pseudo_tensor) {
        //....if submat ok move
        std::cout << a.second << std::endl;
        if (a.second >= 0.5) {
            k.move(a.first.x, a.first.y);
        }
    }
    */
    if (show_monitors) {
        imshow(image_window, img_display);
        imshow(result_window, res);
    }
    //! [wait_key]
    //cv::waitKey(100);
    return pseudo_tensor;
    //! [wait_key]
}
void machine_view::show_match_histogram(std::string tmp_name, cv::Mat tmp) {
    cv::Mat src;
    cv::cvtColor(tmp, src, cv::COLOR_BGRA2BGR);
    if (src.empty())
    {
        return;
    }
    std::vector<cv::Mat> bgr_planes;
    cv::split(src, bgr_planes);
    int histSize = 256;
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    cv::Mat b_hist, g_hist, r_hist;
    cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
    cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
    cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
            cv::Scalar(255, 0, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
            cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
            cv::Scalar(0, 0, 255), 2, 8, 0);
    }
    if (show_monitors) {
        cv::imshow(tmp_name, histImage);
    }
    //cv::waitKey();
}
double machine_view::compare_histogram(cv::Mat template_hist, cv::Mat comparison) {

    cv::Mat src_base = template_hist;
    cv::Mat src_test1 = comparison;

    cv::Mat hsv_base, hsv_test1;
    cv::cvtColor(src_base, hsv_base, cv::COLOR_BGR2HSV);
    cv::cvtColor(src_test1, hsv_test1, cv::COLOR_BGR2HSV);

    int h_bins = 50, s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    const float* ranges[] = { h_ranges, s_ranges };
    // Use the 0-th and 1-st channels
    int channels[] = { 0, 1 };
    cv::Mat hist_base, hist_test1;
    cv::calcHist(&hsv_base, 1, channels, cv::Mat(), hist_base, 2, histSize, ranges, true, false);
    cv::normalize(hist_base, hist_base, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    cv::calcHist(&hsv_test1, 1, channels, cv::Mat(), hist_test1, 2, histSize, ranges, true, false);
    cv::normalize(hist_test1, hist_test1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double base_test1= compareHist(hist_base, hist_test1, 0);
    /*
    for (int compare_method = 0; compare_method < 4; compare_method++)
    {
        base_test1 = compareHist(hist_base, hist_test1, compare_method);
        std::cout << "Method " << compare_method << " Base-Test: " << " / " << base_test1 << " / " << std::endl;
    }
    std::cout << "Done \n";
    */
    return base_test1;
}
std::pair<int, int> machine_view::start_view(std::string path) {
    std::vector<std::pair<cv::Point, double>> pseudo_tensor;
    HWND hwndDesktop = GetDesktopWindow();
    if (show_monitors) {
        namedWindow("output", cv::WINDOW_FULLSCREEN);
        cv::moveWindow("output", 1366, 0);
    }
    int key = 0;
    //while (key != 30)
    //{
    cv::Mat src = hwnd2mat(hwndDesktop);
    // image processing here
    //if (key == 'a') {
    try {
        pseudo_tensor = machine_vision(src, path);
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    //}
    if (show_monitors) {
        imshow("output", src);
    }
        //key = cv::waitKey(30); // wait time
        //if (key == 0) {
        //    break;
        //}
    //}
    ReleaseCapture();
    if (show_monitors) {
        cv::destroyAllWindows();
    }
    //check elaboration
    for (auto& a : pseudo_tensor) {
        //....if submat ok move
        //std::cout << a.second << std::endl;
        if (a.second >= 0.5) {
            return std::make_pair(a.first.x, a.first.y);
        }
    }
    return std::make_pair(-1, -1);
}