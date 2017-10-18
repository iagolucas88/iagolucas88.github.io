#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

//Default values
#define STEP 5
#define JITTER 3
#define RAIO 3

//Initialize the sliders values
int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, frame, points, border, aux, final;

void on_trackbar_canny(int, void*){
  //Canny function
  Canny(aux, border, top_slider, 3*top_slider);

  final = 0.8 * aux + 0.2 * border;

  //Shows filtered image
  imshow("Canny", final);
}

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  int width, height;
  int gray;
  //Vec3b gray;
  int x, y;

  image = imread("dog.png", CV_LOAD_IMAGE_GRAYSCALE);

  //image = imread("veneza.png", CV_LOAD_IMAGE_COLOR);

  srand(time(0));

  if(!image.data){
  cout << "Image did not open!" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width = image.size().width;
  height = image.size().height;

  //Determines the ranges for the step
  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));
  //points = Mat(height, width, CV_8UC3, Scalar(255,255,255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      //gray = image.at<Vec3b>(y,x);
      
      circle(points, Point(y,x), RAIO, CV_RGB(gray,gray,gray), -1, CV_AA);
      //circle(points, Point(x,y), RAIO, CV_RGB(gray[2],gray[1],gray[0]), -1, CV_AA);
    }
  }
  aux = points.clone();

  sprintf(TrackbarName, "Threshold inferior", top_slider_max);

  namedWindow("Canny", 1);
  createTrackbar(TrackbarName, "Canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny);

  on_trackbar_canny(top_slider, 0);

  imwrite("Canny1.png", final);
  waitKey(0);

  return 0;
}

