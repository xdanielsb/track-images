#include <opencv2/nonfree/features2d.hpp>

using namespace std;
void getDescriptor(Mat image) {

	Mat dsift;
	vkp kpObject;;

	SiftFeatureDetector detector(100);
	SiftDescriptorExtractor extractor;

	detector.detect(image, kpObject);
	extractor.compute(image, kpObject, dsift);

	int nrows = dsift.rows;
	int ncols = dsift.cols;

	show(nrows);
	show(ncols);

	unsigned char *input = (unsigned char*)(dsift.data);
	for (int row = 0; row < nrows; row++){
		for (int col = 0; col < ncols;  col++){
				int val = input[ncols * row + col];
			//cout << val << " ";
		}
		//cout << endl;
	}
}

