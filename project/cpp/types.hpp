#include <vector>
#include <queue>

using namespace std;
/*
 *  New types
 */
typedef vector<Point> vp;
typedef vector<vp> vvp;
typedef vector<Vec4i> v4;
typedef vector<KeyPoint> vkp;
typedef vector <Mat> vm;
typedef vector<Point2f> vp2;
typedef unsigned int ui;

struct DMatch2{
    DMatch dm;
    float dis;
    DMatch2(){}
    DMatch2(DMatch _dm, float _dis){
        dm = _dm;
        dis = _dis;
    }
    bool operator < (DMatch2 e) const {
        return dis > e.dis;
    }
};

typedef priority_queue<DMatch2> vdm2;
typedef vector<DMatch> vdm1;
