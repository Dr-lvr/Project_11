#include "machine_view.h"

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/core.hpp>

#include <Windows.h>
#include <iostream>
#include <string>

int machine_view::machine_view_test()
{
    //! [load_image]
    /// Load image and template
    const std::string img1 = "C:/Users/david/Desktop/test/uno.jpg";
    const std::string img2 = "C:/Users/david/Desktop/test/otto.jpg";

    img = cv::imread(img1, cv::IMREAD_COLOR);
    templ = cv::imread(img2, cv::IMREAD_COLOR);
    /*
    if (argc > 3) {
        use_mask = true;
        //mask = imread(img3, IMREAD_COLOR);
    }*/

    if (img.empty() || templ.empty() || (use_mask && mask.empty()))
    {
        std::cout << "Can't read one of the images" << std::endl;
        return EXIT_FAILURE;
    }
    //! [load_image]

    //! [create_windows]
    /// Create windows
    cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);
    //! [create_windows]

    //! [create_trackbar]
    /// Create Trackbar
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    cv::createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
    //! [create_trackbar]

    MatchingMethod(0, 0);

    //! [wait_key]
    cv::waitKey(0);
    return EXIT_SUCCESS;
    //! [wait_key]
}
void machine_view::MatchingMethod(int, void*)
{
    //! [copy_source]
    /// Source image to display
    cv::Mat img_display;
    img.copyTo(img_display);
    //! [copy_source]

    //! [create_result_matrix]
    /// Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);
    //! [create_result_matrix]

    //! [match_template]
    /// Do the Matching and Normalize
    bool method_accepts_mask = (cv::TM_SQDIFF == match_method || match_method == cv::TM_CCORR_NORMED);
    if (use_mask && method_accepts_mask)
    {
        matchTemplate(img, templ, result, match_method, mask);
    }
    else
    {
        matchTemplate(img, templ, result, match_method);
    }
    //! [match_template]

    //! [normalize]
    normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    //! [normalize]

    //! [best_match]
    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    //! [best_match]

    //! [match_loc]
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED)
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
    rectangle(img_display, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 255, 9), 2, 8, 0);
    rectangle(result, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 255, 9), 2, 8, 0);

    imshow(image_window, img_display);
    imshow(result_window, result);
    //! [imshow]

    return;
}

cv::Mat machine_view::hwnd2mat(HWND hwnd)
{
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

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

void machine_view::start_view() {
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output", cv::WINDOW_NORMAL);
    int key = 0;

    while (key != 30)
    {
        cv::Mat src = hwnd2mat(hwndDesktop);
        // you can do some image processing here
        imshow("output", src);
        key = cv::waitKey(30); // you can change wait time
    }
    ReleaseCapture();
    cv::destroyAllWindows();
}