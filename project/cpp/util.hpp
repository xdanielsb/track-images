#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "types.hpp"
using namespace cv;
using namespace std;


void toGrayI(Mat src, Mat &dst) {
	cvtColor(src, dst, COLOR_BGR2GRAY);
}

void toHSV(Mat src, Mat &dst){
	cvtColor(src, dst, COLOR_BGR2HSV);
}


void diffI(Mat src1, Mat src2, Mat &dst) {
	absdiff(src1, src2, dst);
}

void threshI(Mat src, Mat &dst, double thresh) {
	threshold(src, dst, thresh, MAX_THRESH, THRESH_BINARY);
}

void blurI(Mat src, Mat &dst) {
	blur(src, dst, Size(BLUR_SIZE, BLUR_SIZE));
}

void findContoursI(Mat image, vvp &contours, v4 hierarchy) {
	//The image must be gray
	findContours(image, contours, hierarchy, CV_RETR_EXTERNAL,
	    CV_CHAIN_APPROX_SIMPLE);
}

void drawContoursI(Mat &image, InputArrayOfArrays contours, int numElements, const Scalar color , v4 hierarchy = vector<Vec4i>() ) {
	for (int countourId = 0; countourId < numElements; countourId++) {

		drawContours(image, contours, countourId, color, 2, 8, hierarchy, 0, Point());
	}
}

void drawCircle(Mat &dst, Point p, Scalar const color = GREEN, int radius=1, int thickness= 2){
	circle(dst, p, radius, color, thickness);
}

void convexHullI(vp points, vp &hull, bool clockwise = false){
	convexHull(points, hull, clockwise);

}

void drawKeyPoints(Mat & img, vkp kp, const Scalar color= GREEN){
	int sizek = kp.size();
		for (int i =0; i < sizek; ++i){
			Point2f a =  kp[i].pt;
			drawCircle(img,a, color);
		}
}

void showDescriptors(Mat desc){
	unsigned char *input = (unsigned char*)(desc.data);
	show(desc.cols);
	show(desc.rows);
	for (int row = 0; row < desc.rows; row++){
		for (int col = 0; col < desc.cols;  col++){
				int val = input[desc.cols * row + col];
			cout << val << " ";
		}
		cout << endl;
	}
}


void displayPointsConvex(vp2 points, Mat &frame1, Scalar const color, Scalar const color2, int lim ) {
	vp points2;
	if (points.size() <= lim) {
		lim = points.size();
	}
	for (int i = 0; i < lim; i++) {
		drawCircle(frame1, points[i], color2);
		points2.pb(Point((int) points[i].x, (int) points[i].y));
	}

	//get the convex hull
	vvp hull(1);
	if (points2.size() > 0) {
		convexHullI(points2, hull[0]);
	}

	drawContoursI(frame1, hull, hull.size(), color);

}
