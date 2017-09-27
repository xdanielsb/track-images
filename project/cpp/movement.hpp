using namespace std;

int THRESH_VALUE = 50;

pair < vvp, v4>  getMovements(Mat frame1, Mat frame2) {
	Mat grayImg1, grayImg2;
	Mat temp, diffImg, threshImg, blurImg;

	toGrayI(frame1, grayImg1);
	toGrayI(frame2, grayImg2);

	diffI(grayImg2, grayImg1, diffImg);
	threshI(diffImg, threshImg, THRESH_VALUE);
	blurI(threshImg, blurImg);

	vvp contours;
	v4 hierarchy;

	threshImg.copyTo(temp);
	findContoursI(temp, contours, hierarchy);
  int numContours = contours.size();
	if ( numContours > 0) {
		printf("movement: The number of contours detected is: %d\n", numContours);
	}
	return mp(contours, hierarchy);
}
