#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace cv;
using namespace std;

void toGray(Mat  src, Mat &dst){
	cvtColor(src, dst, COLOR_BGR2GRAY);
}

void diff(Mat src1, Mat src2, Mat &dst){
	absdiff(src1, src2, dst);
}
