#include <opencv2/nonfree/features2d.hpp>

vp2 orbI(Mat trainingImg, Mat frame1) {

	OrbFeatureDetector detector;
	vkp kpObject, kpScene;

	//detect the key points
	detector.detect(trainingImg, kpObject);
	detector.detect(frame1, kpScene);

	//detect the descriptors
	OrbDescriptorExtractor extractor;
	Mat descObject, descScene;

	extractor.compute(trainingImg, kpObject, descObject);
	extractor.compute(frame1, kpScene, descScene);

	//Matching the descriptors using FLANN matcher
	BFMatcher matcher;

	vdm1 matches;
	matcher.match(descScene, descObject, matches);
	double maxDistance = 0, minDistance = 100;

	//Compute the max and the min distance
	for (int i = 0; i < descObject.rows; i++) {
		double dist = matches[i].distance;
		if (dist < minDistance && dist > 0.01)
			minDistance = dist;
		if (dist > maxDistance)
			maxDistance = dist;
	}

	//printf("Max distance: %f  \n", maxDistance);
	//printf("Min distance: %f  \n", minDistance);

	//Draw the good matches
	//printf("orb: The number of matches is %d \n", matches.size());
	vdm2 goodMatches;
	for (int i = 0; i < descObject.rows; i++) {
		float dist = matches[i].distance;
		if (!isnan(dist) && dist < 3 * minDistance)
			goodMatches.push(DMatch2(matches[i], dist));

	}
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
