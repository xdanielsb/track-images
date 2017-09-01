#include <opencv2/nonfree/features2d.hpp>

vp2 surfI(Mat trainingImg, Mat frame1) {

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
	double maxDistance = 0, minDistance = 300;

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
	printf("The number of matches is %d \n", matches.size());
	vdm goodMatches;
	for (int i = 0; i < descObject.rows; i++) {
		double dist = matches[i].distance;
		if (dist < 3 * minDistance)
			goodMatches.pb(matches[i]);
	}
	vp2 points;
	for (DMatch aux : goodMatches) {
		Point2f a = kpScene[aux.trainIdx].pt;
		//Point2f b = kpScene[aux.queryIdx].pt;
		points.pb(a);
	}

	return points;
}
