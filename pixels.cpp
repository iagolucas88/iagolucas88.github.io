#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;

  image = imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "Nao abriu bolhas.png" << endl;

  namedWindow("Bolhas",WINDOW_AUTOSIZE);

  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<uchar>(i,j)=200;
    }
  }
  
  imshow("Bolhas", image);  
  waitKey();

  image = imread("bolhas.png",CV_LOAD_IMAGE_COLOR);

  val[0] = 112;//B
  val[1] = 25; //G
  val[2] = 25; //R
  
  for(int i=100;i<210;i++){
    for(int j=10;j<250;j++){
      image.at<Vec3b>(i,j)=val;
    }
  }

  imshow("Bolhas", image);  
  waitKey();
  return 0;
}
