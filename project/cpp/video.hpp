#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/nonfree/features2d.hpp>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "constants.hpp"
#include "util.hpp"
#include "matchers.hpp"

#include "surf.hpp" //depend on constants util, matchers
#include "sift.hpp" //depend on constants, util, matchers
#include "orb.hpp" //depend on constants and util
#include "brief.hpp" //depend on constants and util
#include "descriptorExtractor.hpp" //depend on constants and util

#include "movement.hpp" //depend on util

using namespace cv;
using namespace std;
/*
 * Variables
 */
unsigned int NUM_IMAGE = 0;
bool DEBUG = false;
bool MOVEMENT = false;
bool PAUSED = false;

bool SURFM = true;
bool SIFTM = true;
bool ORBM = true;
bool BRIEFM = true;

unsigned int NUM_POINT_MAX_PLOT = 	3;

int displayOptions(Mat frame) {
	char filename[200];
	char key = (char) waitKey(30);

	if (key == 'q' || key == 'Q' || key == 27) {
		return 0;
	} else if (key == ' ') {
		sprintf(filename, "image%.3d.jpg", NUM_IMAGE++);
		imwrite(filename, frame);
		printf("Saved %s", filename);
		fflush(stdout);
	} else if (key == 'd' || key == 'D') {
		DEBUG = !DEBUG;
		printf(DEBUG ? "Debug mode Enable\n" : "DEBUG Mode Disabled\n");
		fflush(stdout);
	} else if (key == 'm' || key == 'M') {
		MOVEMENT = !MOVEMENT;
		printf(MOVEMENT ? "MOVEMENT Mode Enable\n" : "MOVEMENT Mode Disabled\n");
		fflush(stdout);
	} else if (key == 'p' || key == 'P') {
		PAUSED = !PAUSED;
		printf(PAUSED ? "The program is PAUSED\n" : "The program is running.\n");
		fflush(stdout);
	}

}

void showFrame(string WINDOW_NAME, Mat frame) {
	imshow(WINDOW_NAME, frame);
}

void displayPointsConvex(vp2 points, Mat &frame1, Scalar const color, Scalar const color2) {
	vp points2;
	int lim = NUM_POINT_MAX_PLOT;

	if (points.size() <= NUM_POINT_MAX_PLOT) {
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


void ExtractDescriptors(){
	Mat trainingImg = imread("../datasets/images/3.jpg");
	getDescriptor(trainingImg);
}

void StartAnalysisOverCamera() {

	Mat trainingImg = imread("img/trainingImg.png");
	imshow("TRAINING IMAGE", trainingImg);

	VideoCapture capture(CAMERA_EXTERN);

	if (!capture.isOpened()) {
		capture.open(CAMERA_EXTERN);
	}

	if (!capture.isOpened()) {
		cerr << "Failed to open camera" << endl;
	}

	namedWindow(WINDOW_NAME, CV_WINDOW_KEEPRATIO);

	Mat frame1, frame2;

	printf("%s", INSTRUCTIONS.c_str());
	fflush(stdout);

	/*
	 * Calc the key points and descriptors for the training image
	 */
	vkp kpsift, kpsurf;
	Mat  descsift, descsurf;
	getKeyPointsSIFT(trainingImg, kpsift);
	getDescriptorsSIFT(trainingImg, kpsift, descsift);

	getKeyPointsSURF(trainingImg, kpsurf);
	getDescriptorsSURF(trainingImg, kpsurf, descsurf);


	for (;;) {
		capture.read(frame1);
		capture.read(frame2);

		//Detect movement
		if (MOVEMENT) {
			pair<vvp, v4> aux = getMovements(frame1, frame2);
			vvp contours = aux.first;
			v4 hierarchy = aux.second;
			drawContoursI(frame1, contours, contours.size(), CYAN, hierarchy);
		}

		//Method SURF
		if (SURFM) {
			vp2 points = surfI(frame1,kpsurf, descsurf);
			displayPointsConvex(points, frame1, RED, RED);
		}

		//Method SIFT
		if (SIFTM) {
			vp2 points = siftI(frame1,kpsift, descsift);
			displayPointsConvex(points, frame1, BLUE, BLUE);
		}

		//Method ORB
		if (ORBM) {
		//	vp2 points =  orbI(trainingImg, frame1);
		//	displayPointsConvex(points, frame1, YELLOW, YELLOW);
		}

		//Pause
		if (!PAUSED) {
			if (frame1.empty())
				break;
			showFrame(WINDOW_NAME, frame1);
		}

		displayOptions(frame1);

	}

}
