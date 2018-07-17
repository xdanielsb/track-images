#include "video.hpp"
#include "time.hpp"

//#include "opencv_version.hpp"
/**
 * Program that implements methods of tracking
 * `pkg-config --libs opencv`
 * @version 1.0
 * @author : Daniel Santos
 */
int main() {
  // print_version();
  auto start = chrono::high_resolution_clock::now();

  StartAnalysisOverCamera();
  // ExtractDescriptors();

  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> diff = end - start;
  cout << "Turn around time : " << diff.count() << " seconds." << endl;

  return 0;
}
