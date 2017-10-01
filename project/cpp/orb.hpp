#include <opencv2/nonfree/features2d.hpp>

void getKeyPointsORB(Mat img, vkp &keyp){
	OrbFeatureDetector detector;
	detector.detect(img, keyp);
}
void getDescriptorsORB(Mat img, vkp &keyp, Mat &desc){
	OrbDescriptorExtractor extractor;
	extractor.compute(img, keyp, desc);
	desc.convertTo(desc, CV_32F);
}

vp2 orbI(Mat &frame1, Mat descObject, bool drawKpoints=false) {
	vkp kpScene;
	Mat  descScene;
	getKeyPointsORB(frame1, kpScene);
	getDescriptorsORB(frame1, kpScene, descScene);
	vdm2 goodMatches = bfMatcher(descObject, descScene);
	vp2 points = bestMatches(goodMatches, kpScene);
	if (drawKpoints) drawKeyPoints(frame1, points, CYAN);
	return points;
}
