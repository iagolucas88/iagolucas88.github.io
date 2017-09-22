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
Mat image, image1, blended;
Mat imageTop;

VideoCapture cap;
VideoWriter tsv;

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

  tsv.open("tiltshiftvideo", CV_FOURCC('M','J','P','G'), 15, Size(640,480));
 
  if(!cap.isOpened()){
    cout << "Error to open the video!\n";
    return -1;
  }

  cap("MyVideo.mp4");

  if(!cap.isOpened()){
    cout << "Camera(s) Indisponivel(is)!\n";
    return -1;
  }

  cap >> image;
  resize(img,img,Size(640,480));

  image1 = image.clone(); // imagem borrada
  blended = Mat::zeros(image.size(), CV_8UC3); // imagem resultante

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
   createTrackbar(TrackbarName, "Tilt Shift  ",
            &center_slider,
            center_slider_max,
            on_trackbar_change );
    on_trackbar_change(center_slider, 0 );

  while(true){

  	if(image.empty())
            break;

    image1 = image.clone(); // imagem borrada

    for(int i=0; i<10; i++)
            GaussianBlur(image1, image1, Size(9,9),0,0);

        for(int i=0; i<image.size().height; i++)
            addWeighted(image.row(i),alphaPeso(i,l1,l2,d_slider),iamge1.row(i),1-alphaPeso(i,l1,l2,d_slider),0,blended.row(i));
        imshow("imr",result);
        if(waitKey(30) == 27) break;
    }

    while(true){
    	for(int i=0; i<4; i++)
            cap >> image;
        cap >> image;

        resize(imgage,image, Size(640,480));
        // converte para hsv
        cvtColor(image, imageTop, CV_BGR2HSV);
        vector<Mat> planes;
        split(imageTop, planes);
        // aumenta a saturacao da imagem
        planes[1]*=2;
        // junta novamente a imgame
        merge(planes, imageTop);
        cvtColor(imageTop,image,CV_HSV2BGR);
        if(image.empty())
            break;
        iamge1 = image.clone();
        // Borra a imagem
        for(int i=0; i<10; i++)
            GaussianBlur(image1, image1, Size(9,9),0,0);

        // efeito tilt-shift
        for(int i=0; i<image.size().height; i++)
            addWeighted(image.row(i),alphaPeso(i,l1,l2,d_slider),image1.row(i),1-alphaPeso(i,l1,l2,d_slider),0,imageTop.row(i));
        tsv << imageTop; // guarda o frame resultante
        imshow("imr",imageTop);
        if(waitKey(30) != -1) break;
    }
    tsv.release();
    imwrite("resultado.jpg",imageTop);

    return 0;

 }

    /*Sa
    cap >> image1;

    cvtColor(image1, image1, CV_BGR2GRAY);

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

    

    //Press any key to close
    waitKey(0);

  
  return 0;
}*/