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
#include "orb.hpp" //depend on constants and util, matcher

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

unsigned int NUMPOINTPLOT= 	100;

int displayandReadOptions(Mat frame) {
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

void TestDescriptors(vm clusters, string name, bool showi){
	show(name);
	//Now Get the features of the train image to test
	Mat trainingImg = imread(name);

	//toHSV(trainingImg, trainingImg);
	cout << "SIFT" << endl;
	siftI(trainingImg, clusters[NSIFT], true);

	cout << "SURF" << endl;
	surfI(trainingImg, clusters[NSURF], true);

	cout << "ORB" << endl;
	orbI(trainingImg, clusters[NORB], true);

	if (showi){
			imshow("Image", trainingImg);
			waitKey(0);
	}
}

void ExtractDescriptors(){
	int numImagestoCover = 20;
	int dictionarySize = 100;
	vm descs = getDescriptor(numImagestoCover);
	vm clusters = createBagOfWords(descs, dictionarySize);
	show(dictionarySize);

	//Test training set
	for (int i = 1 ; i <=2; i++){
		string name = join("../datasets/images/", to_string(i), ".jpg");
		TestDescriptors(clusters, name, false);
		cout << endl;
	}

	//Test new images to label
	for (int i = 2 ; i <=2; i++){
		string name = join("../datasets/test/", to_string(i), ".jpg");
		TestDescriptors(clusters, name, true);
		cout << endl;
	}


}

void StartAnalysisOverCamera() {
	Mat frame1, frame2;
	vp2 points;

	VideoCapture capture(0);
	if (!capture.isOpened()) capture.open(CAMERA_EXTERN);
	if (!capture.isOpened()) cerr << "Failed to open camera" << endl;
	namedWindow(WINDOW_NAME, CV_WINDOW_KEEPRATIO);
	printf("%s", INSTRUCTIONS.c_str());
	fflush(stdout);


	int numImagestoCover = 10;
	int dictionarySize = 100;
	vm descs = getDescriptor(numImagestoCover);
	vm clusters = createBagOfWords(descs, dictionarySize);
	show(dictionarySize);

	for (;;) {
		capture.read(frame1);
		//Pause
		if (!PAUSED) {
			if (frame1.empty()) break;
			//Detect movement
			if (MOVEMENT) {
				capture.read(frame2);
				pair<vvp, v4> aux = getMovements(frame1, frame2);
				vvp contours = aux.first;
				v4 hierarchy = aux.second;
				drawContoursI(frame1, contours, contours.size(), CYAN, hierarchy);
			}
			//Method SURF
			//toHSV(frame1, frame1);
			if (SURFM) points = surfI(frame1, clusters[NSURF], true);
			displayPointsConvex(points, frame1, RED, RED, NUMPOINTPLOT);
			//Method SIFT
			if (SIFTM) points = siftI(frame1, clusters[NSIFT], true);
			displayPointsConvex(points, frame1, BLUE, BLUE, NUMPOINTPLOT);
			//Method ORB
			if (ORBM)	 points = orbI(frame1 , clusters[NORB], true);
	  	displayPointsConvex(points, frame1, YELLOW, YELLOW, NUMPOINTPLOT);
			showFrame(WINDOW_NAME, frame1);
		}
		displayandReadOptions(frame1);
	}

}
