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
cv::Mat machine_view::machine_view_test(cv::Mat src) {
    //! [load_image]
    /// Load image and template
    kernel k;
    const std::string img2 = "C:/Users/david/Desktop/test/undici.jpg";

    cv::Mat screen=src;
    //src.convertTo(screen, CV_8UC3);
    
    std::cout << screen.type() << std::endl;
    std::cout << screen.dims << std::endl;
    std::cout << screen.depth() << std::endl;

    cv::Mat cvt = cv::imread(img2, cv::IMREAD_COLOR);
    cv::Mat tmp;
    cv::cvtColor(cvt, tmp, cv::COLOR_BGR2BGRA);
    std::cout << tmp.type() << std::endl;
    std::cout << tmp.dims << std::endl;
    std::cout << tmp.depth() << std::endl;

    //! [load_image]

    //! [create_windows]
    /// Create windows
    cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);

    cv::moveWindow(image_window, 1918, 205);
    cv::moveWindow(result_window, 1918, 205);
    //! [create_windows]

    cv::Mat img_display;
    screen.copyTo(img_display);
    //! [copy_source]

    //! [create_result_matrix]
    /// Create the result matrix
    int result_cols = screen.cols - tmp.cols + 1;
    int result_rows = screen.rows - tmp.rows + 1;

    cv::Mat res;
    res.create(result_rows, result_cols, CV_32FC1);

    for (int i = 0; i < 6; ++i) {
        cv::matchTemplate(screen, tmp, res, i);

        //! [normalize]
        cv::normalize(res, res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
        //! [normalize]

        //! [best_match]
        /// Localizing the best match with minMaxLoc
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
        /// Show me what you got
        rectangle(img_display, matchLoc, cv::Point(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows), cv::Scalar(0, 255, 9), 2, 8, 0);
        rectangle(res, matchLoc, cv::Point(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows), cv::Scalar(0, 255, 9), 2, 8, 0);
        k.move((matchLoc.x+(matchLoc.x + tmp.cols))/2, (matchLoc.y+(matchLoc.y + tmp.rows))/2);
        //k.move(matchLoc.x + tmp.cols, matchLoc.y + tmp.rows);
    }
    imshow(image_window, img_display);
    imshow(result_window, res);

    //! [wait_key]
    cv::waitKey(100);
    return res;
    //! [wait_key]
}
void machine_view::start_view() {
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output", cv::WINDOW_FULLSCREEN);
    cv::moveWindow("output", 1366, 0);
    int key = 0;

    while (key != 30)
    {
        cv::Mat src = hwnd2mat(hwndDesktop);
        // you can do some image processing here
        if (key == 'a') {
            try {
                src = machine_view_test(src);
            }
            catch (const std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }
        imshow("output", src);
        key = cv::waitKey(30); // you can change wait time
        if (key == 0) {
            break;
        }
    }
    ReleaseCapture();
    cv::destroyAllWindows();
}