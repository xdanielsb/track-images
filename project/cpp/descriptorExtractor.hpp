#include <opencv2/nonfree/features2d.hpp>

using namespace std;
void getDescriptor(Mat image) {

	SiftFeatureDetector detector(100);
	vkp kpObject;;

	//detect the key points
	detector.detect(image, kpObject);

	//detect the descriptors
	SiftDescriptorExtractor extractor;
	Mat descObject;

	//Extract the feautures
	extractor.compute(image, kpObject, descObject);

	int nrows = descObject.rows;
	int ncols = descObject.cols;

	show(nrows);
	show(ncols);

	unsigned char *input = (unsigned char*)(descObject.data);

	for (int row = 0; row < nrows; row++){
		for (int col = 0; col < ncols;  col++){
				int val = input[ncols * row + col];
			cout << val << " ";
		}
		cout << endl;
	}
}

