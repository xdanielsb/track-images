vdm2 flannMatcher(Mat descObject, Mat descScene){
	//Matching the descriptors using FLANN matcher
		FlannBasedMatcher matcher;
		vdm1 matches;
		matcher.match(descObject, descScene, matches);
		double maxDistance = 0, minDistance = 500;

		//Compute the max and the min distance
		for (int i = 0; i < descObject.rows; i++) {
			double dist = matches[i].distance;
			if (dist < minDistance)
				minDistance = dist;
			if (dist > maxDistance)
				maxDistance = dist;
		}

		vdm2 goodMatches;
		for (int i = 0; i < descObject.rows; i++) {
			double dist = matches[i].distance;
			if (dist < 3 * minDistance)
				goodMatches.push(DMatch2(matches[i], dist));
		}

		return goodMatches;

}
