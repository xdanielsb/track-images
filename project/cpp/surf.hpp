#include <opencv2/nonfree/features2d.hpp>

void getKeyPointsSURF(Mat img, vkp &keyp){
	SurfFeatureDetector detector(100);
	detector.detect(img, keyp);
}
void getDescriptorsSURF(Mat img, vkp &keyp, Mat &desc){
	SurfDescriptorExtractor extractor;
	extractor.compute(img, keyp, desc);
}

vp2 surfI(Mat frame1, vkp kpObject, Mat descObject) {

	vkp kpScene;
	Mat  descScene;
	getKeyPointsSURF(frame1, kpScene);
	getDescriptorsSURF(frame1, kpScene, descScene);

	vdm2 goodMatches = flannMatcher(descObject, descScene);

	vp2 points;
	while (!goodMatches.empty()) {
		DMatch2 aux = goodMatches.top();
		goodMatches.pop();
		Point2f a = kpScene[aux.dm.trainIdx].pt;

		//Point2f b = kpScene[aux.queryIdx].pt;
		points.pb(a);
	}

	return points;
}
