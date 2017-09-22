#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image, hist, hist_new;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  int nbins = 128;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  double limiar = 1, compare = 0;

//Set the first camera inicialized
  cap.open(0);

  if(!cap.isOpened()){
    cout << "Camera(s) indisponivel(is)";
    return -1;
  }

//Determine the size of images
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "Largura = " << width << endl;
  cout << "Altura  = " << height << endl;

  //Capture the a first image to compare
  cap >> image;

  //Split the planes in R, G and B
  split(image, planes);

  calcHist(&planes[0], 1, 0, Mat(), hist, 1,
             &nbins, &histrange,
             uniform, acummulate);

  //Normalize the histogram starting in 0 to 1
  normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat());


//Infinite loop to capture the images
  while(1){
    //Storege the images
    cap >> image;

    //Split the planes in R, G and B
    split (image, planes);

    //Calculate the histograms by color
    calcHist(&planes[0], 1, 0, Mat(), hist_new, 1,
             &nbins, &histrange,
             uniform, acummulate);
  
    //Normalize the new histogram starting in 0 to 1
    normalize(hist_new, hist_new, 0, 1, NORM_MINMAX, -1, Mat());

    //Set the last value to compare
    limiar = compare*1.25;

    //Return the value of new image
    compare = compareHist(hist, hist_new, CV_COMP_BHATTACHARYYA);

    //Alert signal if detect moviment
    if(compare >= limiar){
      putText(image, "Moviment Detected!", cvPoint(width*0.3,height*0.9), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(0,0,250), 1, CV_AA);
      cout << "("<<compare<<")"<<endl;
    }

    //Set the last image to compare
    hist = hist_new.clone();

    //Show image
    imshow("image", image);
    
    //Holds the image until press key
    if(waitKey(30) >= 0) break;
  }

  return 0;
}