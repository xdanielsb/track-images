#include <opencv2/nonfree/features2d.hpp>

using namespace std;
void getDescriptor(Mat image) {

	Mat dsift, dsurf;
	vkp kpsift, kpsurf;

	SiftFeatureDetector detectorSift(100);
	SiftDescriptorExtractor extractorSift;

	detectorSift.detect(image, kpsift);
	extractorSift.compute(image, kpsift, dsift);

	int nrows = dsift.rows;
	int ncols = dsift.cols;

	cout << "SIFT " << endl;
	show(nrows);
	show(ncols);

	SurfFeatureDetector detectorSurf(400);
	detectorSurf.detect(image, kpsurf);
	SurfDescriptorExtractor extractorSurf;
	extractorSurf.compute(image, kpsurf, dsurf);

	nrows = dsurf.rows;
	ncols = dsurf.cols;

	cout << "SURF " << endl;
	show(nrows);
	show(ncols);


	/*

	unsigned char *input = (unsigned char*)(dsift.data);
	for (int row = 0; row < nrows; row++){
		for (int col = 0; col < ncols;  col++){
				int val = input[ncols * row + col];
			//cout << val << " ";
		}
		//cout << endl;
	}*/
}

