#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  int nobjects, nholls;

  CvPoint p;

  //Read a image determined previously in gray scale
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    std::cout << "Imagem nao carregou corretamente!\n";
    return(-1);
  }

  //Num of rows and colums
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  //Count objects and fill with gray
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        nobjects++;
        p.x=j;
        p.y=i;
        floodFill(image,p,100);
      }
    }
  }

  //Remove objecs from the borders
  for(int i=0; i<width; i++){
    if(image.at<uchar>(0,i) == 0){
      p.x=i;
      p.y=0;
      floodFill(image,p,100);
    }
  }

  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,0) == 0){
      p.x=0;
      p.y=i;
      floodFill(image,p,100);
    }
  }

  for(int i=0; i<height; i++){
    if(image.at<uchar>(width-1,i) == 0){
      p.x=i;
      p.y=width-1;
      floodFill(image,p,100);
    }
  }

  for(int i=0; i<width; i++){
    if(image.at<uchar>(i,height-1) == 0){
      p.x=height-1;
      p.y=i;
      floodFill(image,p,100);
    }
  }

  //Count the num of holls
  nholls=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
    // achou um objeto
        nholls++;
        p.x=j;
        p.y=i;
        floodFill(image,p,255);
      }
    }
  }

  cout << "Numero de objetos: " << nobjects << endl;
  cout << "Numero de buracos: " << nholls << endl;
  
  //Show image
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}