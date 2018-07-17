#include <opencv2/nonfree/features2d.hpp>

void getKeyPointsSURF(Mat img, vkp &keyp, int numKeyPoints = 10) {
  SurfFeatureDetector detector(numKeyPoints);
  detector.detect(img, keyp);
}
void getDescriptorsSURF(Mat img, vkp &keyp, Mat &desc) {
  SurfDescriptorExtractor extractor;
  extractor.compute(img, keyp, desc);
}

vp2 surfI(Mat &frame1, Mat descObject, bool drawKpoints = false) {
  vkp kpScene;
  Mat descScene;
  getKeyPointsSURF(frame1, kpScene);
  getDescriptorsSURF(frame1, kpScene, descScene);
  vdm2 goodMatches = flannMatcher(descObject, descScene);
  vp2 points = bestMatches(goodMatches, kpScene);

  if (drawKpoints) drawKeyPoints(frame1, points, WHITE);
  return points;
}
