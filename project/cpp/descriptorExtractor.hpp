#include <opencv2/nonfree/features2d.hpp>

using namespace std;

enum{NSIFT, NSURF, NORB};
static map <int , string> namesdic = {
		{0, "SIFT"},
		{1, "SURF"},
		{2, "ORB"}
};

vm createBagOfWords(vm vmfeatures, int dictionarySize =100){

	vm clusters;
	int MAX_ITER  = 100;
	float EPS = 0.001;
	TermCriteria tc(CV_TERMCRIT_ITER,MAX_ITER,EPS);
	int retries=1;
	int flags=KMEANS_PP_CENTERS;

	//cluster the feature vectors
	for (ui i =0; i < vmfeatures.size(); i++){
		BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
		Mat dictionary=bowTrainer.cluster(vmfeatures[i]);
		clusters.pb(dictionary);
		//store the vocabulary
	  //string source = join("dictionary", namesdic[i], ".yml");
	  //persistMatrix(dictionary, source);
	}
	return clusters;
}

vm getDescriptor(int numImages) {
	vm vmfeatures;
	Mat image;
	Mat dsift, dsurf, dorb, dmser;
	vkp kpsift, kpsurf, kporb, kpmser, kpfast, kpstar, kpbrisk, kpgftt, kpdense, kpsimple;
  const	int numKeyPoints = 1000;

  Mat fcsift, fcsurf, fcorb;



  for (int i = 1; i <= numImages; ++i){
    String name = join( "../datasets/images/", to_string(i), ".jpg");
    image = imread(name);
    //toHSV(image, image);


  	getKeyPointsSIFT(image, kpsift);
  	getDescriptorsSIFT(image, kpsift, dsift);
  	fcsift.pb(dsift);
    // show(fcsift.size());
  	//showDescriptors(dsift);

		getKeyPointsSURF(image, kpsurf);
		getDescriptorsSURF(image, kpsurf, dsurf);
		//show(dsurf.depth());
		fcsurf.pb(dsurf);
		//show(fcsurf.size());
		//showDescriptors(dsurf);

		getKeyPointsORB(image, kporb);
		getDescriptorsORB(image, kporb, dorb);
		//show(dorb.depth());
		dorb.convertTo(dorb, CV_32F);
		fcorb.pb(dorb);
		//show(fcorb.size());
		//showDescriptors(dorb);
  }
   vmfeatures.pb(fcsift);
   vmfeatures.pb(fcsurf);
   vmfeatures.pb(fcorb);

 //


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






	/*namedWindow("Key Points", CV_WINDOW_OPENGL);
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

	waitKey(0);*/

	return vmfeatures;

}

