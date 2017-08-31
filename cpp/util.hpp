#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

/*
 *  New types
 */
typedef vector < Point > vp;
typedef vector < vp  > vvp;
typedef vector < Vec4i > v4;


void toGrayI(Mat  src, Mat &dst){
	cvtColor(src, dst, COLOR_BGR2GRAY);
}

void diffI(Mat src1, Mat src2, Mat &dst){
	absdiff(src1, src2, dst);
}

void threshI(Mat src, Mat &dst, double thresh){
 threshold(src, dst, thresh, MAX_THRESH, THRESH_BINARY);
}

void blurI(Mat src, Mat &dst){
	blur(src, dst, Size(BLUR_SIZE, BLUR_SIZE))	;
}

void findContoursI(Mat image, vvp &contours, v4 hierarchy){
	//The image must be gray
	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
}

