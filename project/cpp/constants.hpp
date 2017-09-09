#include <string>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;
string INSTRUCTIONS =
    "    Hello User!!! \n \
    I'm daniel this software track an object in \n\
    video in real time. \n \
	\n \
    Commands \n \
    Key         Function\n\
    T           Tracking\n\
    D           Debug Activated\n\
    F           Finish the program\n\
    P           Pause the camera\n\
    W           Save the image\n\
    O           Activate tracking method 1\n\
    U           Activate tracking method 2\n\
    S           Activate tracking method 3\n\
    M           Visualize tracking methods\n\
    F           Follow the track object\n";

string const WINDOW_NAME = "Tracking";
int const CAMERA_LAPTOP = 0;
int const CAMERA_EXTERN = 1;
int const MAX_THRESH = 255;
int const BLUR_SIZE = 10 ;
Scalar const GREEN = Scalar(0,255,0);
Scalar const RED = Scalar(0,0,255);
Scalar const BLUE = Scalar(255,0,0);
Scalar const PURPLE = Scalar(255,0,255);
Scalar const WHITE = Scalar(255,255,255);
Scalar const CYAN = Scalar(255,255,0);
Scalar const YELLOW = Scalar(0,255,255);

#define pb push_back
#define mp make_pair
