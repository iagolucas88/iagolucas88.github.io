#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double alfa;

//Ranges of sliders
int center_slider = 0;
int center_slider_max = 100;

int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

//Matrixs
Mat image1, image2, blended;
Mat imageTop;

//Neme of the trackers bar
char TrackbarName[50];

//Track bar combination
void on_trackbar_blend(int, void*){
 alfa = (double) alfa_slider/alfa_slider_max ;
 addWeighted(image1, alfa, imageTop, 1-alfa, 0.0, blended);
 imshow("Tilt Shift", blended);
}


void on_trackbar_change(int, void*) {

  //Copy image
  image2.copyTo(imageTop);

  //Image size
  Size tamanho = image2.size();

  //Image width
  int largura = tamanho.width;

  //Image heigth
  int altura = tamanho.height;

  //Adapt the top slider range to the image width
  int limit = top_slider*largura/100;

  //Adapt the center slider range to the image width
  int base = center_slider*largura/100;

  //Only positive values
  if(limit > 0){

    //
    if(base >= 0 && base <= altura-limit){
      Mat tmp = image1(Rect(0, base, largura,limit));
      tmp.copyTo(imageTop(Rect(0, base, largura,limit)));
    }
    
    else{
      Mat tmp = image1(Rect(0, altura-limit, largura,limit));
      tmp.copyTo(imageTop(Rect(0, altura-limit, largura,limit)));
    }
  }

  on_trackbar_blend(alfa_slider,0);
}


int main(int argvc, char** argv){

  //Masks
  Mat aux, mask, mask1;
  float media[] = {1,1,1,1,1,
                     1,1,1,1,1,
                     1,1,1,1,1,
                     1,1,1,1,1,
                     1,1,1,1,1};

  //Read the image and save on image1
  image1 = imread("biel.png");

  //Clone the read image
  image2 = image1.clone();

  //Create an avarege mask                   
  mask = Mat(5, 5, CV_32F, media);

  //Divide by 25 and save on mask1
  scaleAdd(mask, 1/25.0, Mat::zeros(5,5,CV_32F), mask1);

  //Swap the masks
  swap(mask, mask1);

  //Convert the matrix values to float
  image2.convertTo(aux, CV_32F);

  //Loop to convolute 10x
  for (int i = 0; i < 10; i++) {
        filter2D(aux, aux, aux.depth(), mask, Point(1, 1), 0);
    }
  
  //Take the absolute value
  aux=abs(aux);

  //Convert the matrix values to int in 8 bits
  aux.convertTo(image2, CV_8UC3);

  //Copy image 
  image1.copyTo(imageTop);

  //Name of the image window
  namedWindow("Tilt Shift", 1);

  //Name of the alpha slider
  sprintf( TrackbarName, "Alpha ");

  //Creates the alfa slider tracker
  createTrackbar(TrackbarName, "Tilt Shift",
				  &alfa_slider,
				  alfa_slider_max,
				  on_trackbar_blend);
  on_trackbar_blend(alfa_slider, 0 );

  //Name of the heigth slider
  sprintf( TrackbarName, "Height ");

  //Creates the top slider tracker
  createTrackbar(TrackbarName, "Tilt Shift",
				  &top_slider,
				  top_slider_max,
				  on_trackbar_change );
  on_trackbar_change(top_slider, 0 );

  //Name of the position slider
  sprintf( TrackbarName, "Position ");

  //Creates the center slider tracker
  createTrackbar(TrackbarName, "Tilt Shift",
          &center_slider,
          center_slider_max,
          on_trackbar_change );
  on_trackbar_change(center_slider, 0 );

  //Press any key to close
  waitKey(0);

  return 0;
}