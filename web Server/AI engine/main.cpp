<<<<<<< Updated upstream
// yolo_cpp.cpp
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace cv::dnn;
using namespace std;

int main() {
    // --- لود مدل ONNX ---
    Net net = readNetFromONNX("best.onnx");
    net.setPreferableBackend(DNN_BACKEND_CUDA);
    net.setPreferableTarget(DNN_TARGET_CUDA);

    // --- خواندن تصویر ---
    Mat img = imread("test4.jpg");
    Mat blob = blobFromImage(img, 1/255.0, Size(640,640), Scalar(0,0,0), true, false);
    net.setInput(blob);

    // --- تشخیص ---
    Mat output = net.forward();
    // خروجی: [1, 25200, 85] → باید parse بشه

    // --- نمایش ---
    imshow("YOLO C++", img);
    waitKey(0);
    return 0;
}
=======
#include <iostream>  // برای ورودی و خروجی

using namespace std; // برای ساده‌نویسی
int nam;

int main() {
     
    while (1){    
    nam++;
    cout << nam << "\n" ;// چاپ پیام}    
    }

    while (1){    
    }
    return 0; // پایان برنامه با خروجی صفر

}
>>>>>>> Stashed changes
