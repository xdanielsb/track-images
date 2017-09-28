#include <opencv2/nonfree/features2d.hpp>

using namespace std;


Mat FeaturesUnclustered;

void createBagOfWords(const string& source = "dictionary.yml"){
	int dictionarySize = 1;
	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
	int retries=1;
	int flags=KMEANS_PP_CENTERS;

	BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);

	//cluster the feature vectors
	Mat dictionary=bowTrainer.cluster(FeaturesUnclustered);

	//store the vocabulary
	FileStorage fs(source, FileStorage::WRITE);
	fs << "vocabulary" << dictionary;
	fs.release();
}

void getDescriptor(Mat image) {

	Mat dsift, dsurf, dorb, dmser;
	vkp kpsift, kpsurf, kporb, kpmser, kpfast, kpstar, kpbrisk, kpgftt, kpdense, kpsimple;
  const	int numKeyPoints = 100;

	getKeyPointsSIFT(image, kpsift);
	getDescriptorsSIFT(image, kpsift, dsift);
	//showDescriptors(dsift);
	//FeaturesUnclustered.pb(dsift);

	getKeyPointsSURF(image, kpsurf);
	getDescriptorsSURF(image, kpsurf, dsurf);
	//showDescriptors(dsurf);
	FeaturesUnclustered.pb(dsurf);

	getKeyPointsORB(image, kporb);
	getDescriptorsORB(image, kporb, dorb);
	//showDescriptors(dorb);


	MserFeatureDetector detectorMser(numKeyPoints);
	detectorMser.detect(image, kpmser);

  FastFeatureDetector detectorFast(numKeyPoints);
  detectorFast.detect(image, kpfast);

  StarFeatureDetector detectorStar(numKeyPoints);
  detectorFast.detect(image, kpstar);

  BRISK  detectorBrisk(numKeyPoints);
  detectorBrisk.detect(image, kpbrisk);

  GoodFeaturesToTrackDetector detectorGftt(numKeyPoints);
  detectorBrisk.detect(image, kpgftt);

  DenseFeatureDetector detectorDense(numKeyPoints);
  detectorBrisk.detect(image, kpdense);

  SimpleBlobDetector detectorSimple();
  detectorBrisk.detect(image, kpsimple);



  createBagOfWords();



	namedWindow("Key Points", CV_WINDOW_OPENGL);
	//imshow("TRAINING IMAGE", image);

	drawKeyPoints(image, kpsift, GREEN);
	drawKeyPoints(image, kpsurf, PURPLE);
	drawKeyPoints(image, kporb, CYAN);
	drawKeyPoints(image, kpmser, WHITE);
	drawKeyPoints(image, kpfast, RED);
	drawKeyPoints(image, kpstar, BLUE);
	drawKeyPoints(image, kpbrisk, BLACK);
	drawKeyPoints(image, kpgftt, YELLOW);
	drawKeyPoints(image, kpgftt, OLIVE);
	drawKeyPoints(image, kpsimple, MAROON);

	imshow("Key Points", image);

	waitKey(0);


}

