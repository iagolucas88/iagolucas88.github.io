#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//Show mask matrix
void printmask(Mat &m){
  for(int i=0; i<m.size().height; i++){
    for(int j=0; j<m.size().width; j++){
      cout << m.at<float>(i,j) << ",";
    }
    cout << endl;
  }
}

//Commands
void menu(){
  cout << "\nPressione a tecla para ativar o filtro: \n\n"
	"a - Calcular modulo\n"
  "m - Media\n"
  "g - Gauss\n"
  "v - Vertical\n"
	"h - Horizontal\n"
  "l - Laplaciano\n"
  "s - Laplaciano do Gaussiano\n"
  "r - Marr–Hildreth\n\n"
	"esc - sair\n";
}

//Masks 
int main(int argvc, char** argv){
  VideoCapture video;
  float media[] = {1,1,1,
				           1,1,1,
				           1,1,1};

  float horizontal[]={-1,0,1,
					            -2,0,2,
					            -1,0,1};

  float vertical[]={-1,-2,-1,
					           0,0,0,
					           1,2,1};

  float laplacian[]={0,-1,0,
					          -1,4,-1,
					           0,-1,0};

  float gauss[] = {1,2,1,
                   2,4,2,
                   1,2,1};

  float laplaciangauss[] = {1,2,1,
                            2,-12,2,
                            1,2,1};

  float marr[]=  {0,  0,  1,  0,  0,
                0,  1,  2,  1,  0,
                1,  2,-16,  2,  1,
                0,  1,  2,  1,  0,
                0,  0,  1,  0,  0}; 

  float sharpening[] = {-1,-1,-1,
                        -1,8,-1,
                        -1,-1,-1};                  
//x^2+y^2
  float prewitt_X[] = {1,2,1,
                       2,-12,2,
                       1,2,1};

  float prewitt_Y[] = {1,0,-1,
                       1,0,-1,
                       1,0,-1};

  float sobel_X[] = {1,2,1,
                     0,0,0,
                    -1,-2,-1};

  float sobel_Y[] = {-1,0,1,
                     -2,0,2,
                     -1,0,1};


  Mat cap, frame, frame32f, frameFiltered;
  Mat mask(3,3,CV_32F), mask1(3,3,CV_32F), mask2(3,3,CV_32F), mask3(3,3,CV_32F), mask4(3,3,CV_32F);
  Mat result, result1;
  double width, height, min, max;
  int absolut;
  char key;
  
  //Open the first camera
  video.open(0); 
  if(!video.isOpened()) 
    return -1;
  //Get size of images
  width=video.get(CV_CAP_PROP_FRAME_WIDTH);
  height=video.get(CV_CAP_PROP_FRAME_HEIGHT);
  cout << "Width = " << width << "\n";;
  cout << "Height = " << height<< "\n";;

  namedWindow("Spacial Filter",1);
  //Inicialize the filtered image with average mask
  mask = Mat(3, 3, CV_32F, media); 

  //Divide the values of the mask and save on a nex mask matrix
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  
  //Swap masks
  swap(mask, mask1);
  absolut=1; // calcs abs of the image

  menu();
  //Infinite loop
  for(;;){
    
    //Get images
    video >> cap; 
    //Convert the image to gray scale
    cvtColor(cap, frame, CV_BGR2GRAY);
    
    //Flip image horizontaly (mirror efect)
    flip(frame, frame, 1);
    //Show original image
    imshow("Original", frame);
    
    //
    frame.convertTo(frame32f, CV_32F);
    
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    if(absolut){
      frameFiltered=abs(frameFiltered);
    }
    
    frameFiltered.convertTo(result, CV_8U);
    //Show filtered image
    imshow("Spacial Filter", result);
    
    //Get inserted key, Esc = close
    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
    
    switch(key){
    case 'a':
	    menu();
      absolut=!absolut;
      break;

    //Average
    case 'm':
	    menu();
      mask = Mat(3, 3, CV_32F, media);
      scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask); 
      break;
    
    //Gauss
    case 'g':
	    menu();
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      break;

    //Horizontal
    case 'h':
	    menu();
      mask = Mat(3, 3, CV_32F, horizontal);
      printmask(mask);
      break;

    //Vertical  
    case 'v':
	    menu();
      mask = Mat(3, 3, CV_32F, vertical);
      printmask(mask);
      break;

    //Laplacian  
    case 'l':
	    menu();
      mask = Mat(3, 3, CV_32F, laplacian);
      printmask(mask);
      break;

    //Lapacian Gauss  
    case 's':
      menu();
      mask = Mat(3,3, CV_32F, laplaciangauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      break;

    //Marr-Hildreth  
    case 'r':
    menu();
      mask = Mat(5, 5, CV_32F, marr);
      printmask(mask);
      break;  
/*
    //Prewitt  
    case 'p':
          menu();
      mask1 = Mat(3,3, CV_32F, prewitt_X);
      mask2 = Mat(3,3, CV_32F, prewitt_Y);
      printmask(mask1);

      mask3 = mask1.inv();
      mask2 = mask4.inv();

      int aux = 0;

      for(int j=0; j<3; j++){
        for(int i=0; i<3; i++){
          for(int n=0; n<3; n++){

          mask1.at<float>(i,j) = mask1.at<float>(n,j)*mask3.at<float>(n,j);


          }
          mask1.at<float>(i,j)




          mask1.at<float>(i,) = mask1.at<float>(i,j)*mask3.at<float>(i,j) +
                                 mask1.at<float>(i+1,j)*mask3.at<float>(i+1,j) +
                                 mask1.at<float>(i,j)*mask3.at<float>(i,j); 
          aux++;
        }
      }

      cout << endl;
      mask1 = mask1*mask1;
      mask2 = mask2*mask2;

      
      printmask(mask1);
      cout << endl;
      printmask(mask2);
      cout << endl;


      scaleAdd(mask1, 1, mask2, mask);
      printmask(mask);
      break;

    //Sobel  
    case 's':
          menu();
      mask = Mat(3,3, CV_32F, laplaciangauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      break;
      */

    default:
      break;
    }
  }
  return 0;
}
