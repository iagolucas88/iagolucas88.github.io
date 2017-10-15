#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  int p_x, p_y;

  //Read the image
  image = imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "Nao abriu biel.png" << endl;

  //Create a black image with the same size of the load image 
  Mat troca(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
  
  //Size of image
  cout << "Rows = " << image.rows << "\nColums = " << image.cols << endl;

  //Center of image
  p_x = image.rows/2;
  p_y = image.cols/2;

  //Define the name of the window and ajust its size to the image's size
  namedWindow("Biel Troca",WINDOW_AUTOSIZE);

  //Exchange regions
  for(int i=0; i<p_x; i++){
      for(int j=0; j<p_y; j++){
        troca.at<uchar>(i,j) = image.at<uchar>(i, j);
        image.at<uchar>(i, j) = image.at<uchar>(i + p_x, j + p_y);
        image.at<uchar>(i + p_x, j + p_y) = troca.at<uchar>(i, j);

        troca.at<uchar>(i,j) = image.at<uchar>(i, j + p_y);
        image.at<uchar>(i, j + p_y) = image.at<uchar>(i + p_x, j);
        image.at<uchar>(i + p_x, j) = troca.at<uchar>(i, j);
      }
    }

  //Save new image
  imwrite("/home/iago/Documents/Processamento Digital de Imagens/1_unidade/iagolucas88/iagolucas88.github.io/troca.png", image);

  //Show image
  imshow("Biel Troca", image);
  waitKey();

  return 0;
}
