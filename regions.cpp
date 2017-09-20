#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int p1_x, p1_y, p2_x, p2_y;
  int p;

  //Read the image
  image = imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "Nao abriu biel.png" << endl;
  
  //Size of image
  cout<<"Rows = "<<image.rows<<"\nColums = "<<image.cols << endl;


  //Define the points coordinates
  cout << "Digite valores para das coordenadas dos pontos:\n";
  cout << "P1_x = ";
  cin >> p1_x;

  cout << "P1_y = ";
  cin >> p1_y;

  cout << "P2_x = ";
  cin >> p2_x;

  cout << "P2_y = ";
  cin >> p2_y;

  //Turn positive the coordinates's points
  p1_x = abs(p1_x);
  p1_y = abs(p1_y);
  p2_x = abs(p2_x);
  p2_y = abs(p2_y);

  //Put the values in order
  if(p1_x > p2_x){
    p = p1_x;
    p1_x = p2_x;
    p2_x = p;
  }
  if(p1_y > p2_y){
    p = p1_y;
    p1_y = p2_y;
    p2_y = p;
  }

  //Define the name of the window and ajust its size to the image's size
  namedWindow("Biel Negative",WINDOW_AUTOSIZE);

  //Oversize condition
  if(p2_x > image.rows || p2_y > image.cols)
    cout << "Coordenada maior do que o tamanho da imagem!\n";

  //Change the area to negative
  else{
    for(int i=p1_x;i<p2_x;i++){
      for(int j=p1_y;j<p2_y;j++){
        image.at<uchar>(i,j) = 255 - image.at<uchar>(i, j);
      }
    }
  }

  //Save new image
  imwrite("/home/iago/Documents/Processamento Digital de Imagens/1/negative.png", image);


  //Show image
  imshow("Biel Negative", image);
  waitKey();

  return 0;
}
