vdm2 flannMatcher(Mat descObject, Mat descScene){
	//Matching the descriptors using FLANN matcher
		FlannBasedMatcher matcher;
		double dist, acum =0.0, acumbest=0.0;

		vdm1 matches;
		matcher.match(descObject, descScene, matches);
		double maxDistance = -INF, minDistance = INF;

		//Compute the max and the min distance
		for (int i = 0; i < descObject.rows; i++) {
			dist = matches[i].distance;
			acum +=dist;
			if (dist < minDistance) minDistance = dist;
			if (dist > maxDistance) maxDistance = dist;
		}

		vdm2 goodMatches;
		for (int i = 0; i < descObject.rows; i++) {
			double dist = matches[i].distance;
			if (dist < 3 * minDistance){
				acumbest += dist;
				goodMatches.push(DMatch2(matches[i], dist));
			}
		}


		double prob =  maxDistance / (acumbest/goodMatches.size());
	///	show(prob);
		return goodMatches;
}

vdm2 bfMatcher(Mat descObject, Mat descScene){
	//Matching the descriptors using FLANN matcher
		BFMatcher matcher;
		double dist, acum=0.0, acumbest=0.0;
		vdm1 matches;
		matcher.match(descScene, descObject, matches);
		double maxDistance = -INF, minDistance = INF;
		//Compute the max and the min distance
		for (int i = 0; i < descObject.rows; i++) {
			dist = matches[i].distance;
			acum +=dist;
			if (dist < minDistance && dist > 0.01) minDistance = dist;
			if (dist > maxDistance) maxDistance = dist;
		}
		vdm2 goodMatches;
		for (int i = 0; i < descObject.rows; i++) {
			float dist = matches[i].distance;
			if (!isnan(dist) && dist < 3 * minDistance){
				acumbest+= dist;
				goodMatches.push(DMatch2(matches[i], dist));
			}
		}

		double prob =  (acum/descObject.rows) / (acumbest/goodMatches.size());
	///	show(prob);

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
