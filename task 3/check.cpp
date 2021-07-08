#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<bits/stdc++.h>
using namespace cv;
int main()
{
Mat img(100,100,CV_8UC3,Scalar(255,0,0));
namedWindow("Window1",WINDOW_NORMAL);
imshow("Window1",img);
waitKey(0);
}
