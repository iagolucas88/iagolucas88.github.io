#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

  Mat image, histR, histG, histB;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  int nbins = 128;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

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

//Determine the size of variables histogram showed 
  int histw = nbins, histh = nbins/3;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

//Infinite loop to capture the images
  while(1){
    //Storege the images
    cap >> image;

    //Split the planes in R, G and B
    split (image, planes);

    //Equalize the images by plane
    equalizeHist(planes[0],planes[0]);
    equalizeHist(planes[1],planes[1]);
    equalizeHist(planes[2],planes[2]);
    
    //Merge the planes
    merge(planes,image);

    //Calculate the histograms by color
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    //Normalize the histograms starting in 0 to number of rows
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    //Create the histogram images
    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgR,
           Point(i, histh),
           Point(i, histh-cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG,
           Point(i, histh),
           Point(i, histh-cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB,
           Point(i, histh),
           Point(i, histh-cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }

    //Copy the histogram images to the original image on the top left
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
    
    //Show image
    imshow("image", image);
    
    //Holds the image until press key
    if(waitKey(30) >= 0) break;
  }

  return 0;
}