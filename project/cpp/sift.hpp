#include <opencv2/nonfree/features2d.hpp>

void getKeyPointsSIFT(Mat img, vkp &keyp, int numKeyPoints = 100){
	SiftFeatureDetector detector(numKeyPoints);
	detector.detect(img, keyp);
}
void getDescriptorsSIFT(Mat img, vkp &keyp, Mat &desc){
	SiftDescriptorExtractor extractor;
	extractor.compute(img, keyp, desc);
}

vp2 siftI(Mat frame1, vkp kpObject, Mat descObject) {

	vkp kpScene;
	Mat  descScene;
	getKeyPointsSIFT(frame1, kpScene);
	getDescriptorsSIFT(frame1, kpScene, descScene);
	vdm2 goodMatches = flannMatcher(descObject, descScene);
	vp2 points = bestMatches(goodMatches, kpScene);
	return points;
}
