vdm2 flannMatcher(Mat descObject, Mat descScene){
	//Matching the descriptors using FLANN matcher
		FlannBasedMatcher matcher;
		vdm1 matches;
		matcher.match(descObject, descScene, matches);
		double maxDistance = -INF, minDistance = INF;

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

vdm2 bfMatcher(Mat descObject, Mat descScene){
	//Matching the descriptors using FLANN matcher
		BFMatcher matcher;

		vdm1 matches;
		matcher.match(descScene, descObject, matches);
		double maxDistance = -INF, minDistance = INF;
		//Compute the max and the min distance
		for (int i = 0; i < descObject.rows; i++) {
			double dist = matches[i].distance;
			if (dist < minDistance && dist > 0.01)
				minDistance = dist;
			if (dist > maxDistance)
				maxDistance = dist;
		}
		vdm2 goodMatches;
		for (int i = 0; i < descObject.rows; i++) {
			float dist = matches[i].distance;
			if (!isnan(dist) && dist < 3 * minDistance)
				goodMatches.push(DMatch2(matches[i], dist));

		}
		return goodMatches;

}


vp2 bestMatches(vdm2 goodMatches, vkp kpScene){
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
