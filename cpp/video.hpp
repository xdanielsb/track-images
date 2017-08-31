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

using namespace cv;
using namespace std;

/*
 * Variables
 */
int NUM_IMAGE = 0;
bool DEBUG = false;
bool MOVEMENT = true;
bool PAUSED = false;
bool SURFM = false;
int THRESH_VALUE = 50;


int displayOptions(Mat frame) {
	char filename[200];
	char key = (char) waitKey(30);

	if (key == 'q' || key == 'Q' || key == 27) {
		return 0;
	} else if (key == ' ') {
		sprintf(filename, "image%.3d.jpg", NUM_IMAGE++);
		imwrite(filename, frame);
		printf("Saved %c", filename);
		fflush(stdout);
	} else if (key == 'd' || key == 'D') {
		DEBUG = !DEBUG;
		printf(DEBUG ? "Debug mode Enable\n" : "DEBUG Mode Disabled\n");
		fflush(stdout);
	} else if (key == 't' || key == 'T') {
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

void Start() {

	Mat trainingImg = imread("img/training.png", CV_LOAD_IMAGE_GRAYSCALE);
	VideoCapture capture(CAMERA_LAPTOP);

	if (!capture.isOpened()) {
		capture.open(CAMERA_LAPTOP);
	}

	if (!capture.isOpened()) {
		cerr << "Failed to open camera" << endl;
	}

	namedWindow(WINDOW_NAME, CV_WINDOW_KEEPRATIO);

	Mat frame1, frame2, grayImg1, grayImg2;
	Mat temp, diffImg, threshImg, blurImg;
	;
	printf("%s", INSTRUCTIONS.c_str());
	fflush(stdout);
	for (;;) {
		capture.read(frame1);
		capture.read(frame2);

		//Detect movement
		if (MOVEMENT) {
			toGrayI(frame1, grayImg1);
			toGrayI(frame2, grayImg2);

			diffI(grayImg2, grayImg1, diffImg);
			threshI(diffImg, threshImg, THRESH_VALUE);
			blurI(threshImg, blurImg);

			vvp contours;
			v4 hierarchy;

			threshImg.copyTo(temp);
			findContoursI(temp, contours, hierarchy);

			if (contours.size() > 0) {
				printf("The number of contours detected is: %d\n", contours.size());
			}
			for (vp vaux : contours) {
				for (Point aux : vaux) {
					cout << aux.x << " " << aux.y << endl;
				}
			}

			drawContoursI(frame1, contours, hierarchy, YELLOW);

			//plot the points of match in the frame
		}

		if (SURFM) {
			SurfFeatureDetector detector(400);
			vkp kpObject, kpScene;

			//detect the key points
			detector.detect(trainingImg, kpObject);
			detector.detect(frame1, kpScene);

			//detect the descriptors
			SurfDescriptorExtractor extractor;
			Mat descObject, descScene;

			extractor.compute(trainingImg, kpObject, descObject);
			extractor.compute(frame1, kpScene, descScene);

			//Matching the descriptors using FLANN matcher
			FlannBasedMatcher matcher;
			vdm matches;
			matcher.match(descObject, descScene, matches);
			double maxDistance = 0, minDistance = 100;

			//Compute the max and the min distance
			for (int i = 0; i < descObject.rows; i++) {
				double dist = matches[i].distance;
				if (dist < minDistance)
					minDistance = dist;
				if (dist > maxDistance)
					maxDistance = dist;
			}

			//	printf("Max distance: %f  \n", maxDistance);
			//	printf("Min distance: %f  \n", minDistance);

			//Draw the good matches
			vdm goodMatches;
			for (int i = 0; i < descObject.rows; i++) {
				double dist = matches[i].distance;
				if (dist < 3 * minDistance)
					goodMatches.pb(matches[i]);
			}

		}

		if (!PAUSED) {
			if (frame1.empty())
				break;
			showFrame(WINDOW_NAME, frame1);
		}

		displayOptions(frame1);
	}

}
