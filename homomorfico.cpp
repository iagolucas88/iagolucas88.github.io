#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//Exchange the quadrants
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  //In case of odd pixels, a region is cuted for match the sizes
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  
  //Image center point
  int cx = image.cols/2;
  int cy = image.rows/2;

  //Rearrange the quadrants
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  //A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  //C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

//Set sliders initial values
int gammaL_slider = 2, gammaH_slider = 20, sharpC_slider = 1, cutoff_slider = 5;
const int gammaL_max = 10, gammaH_max = 50, sharpC_max = 100, cutoff_max = 200;
int gammaL, gammaH, sharpC, cutoff;
Mat im, imFiltered,padded;
int dft_M, dft_N;

//Defines filter's size
Mat homomorphicFilter(double gl, double gh, double c, double d0){
  Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0));
  Mat tmp = Mat(dft_M, dft_N, CV_32F);

  //Discrete Fourier Transform
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      tmp.at<float> (i,j) = (gh - gl)*(1 - exp(-c*(( (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) ) / (d0*d0) ))) + gl;
    }
  }

  Mat comps[]= {tmp,tmp};

  //Resizes filter image for better view
  Size fil = tmp.size();
  int fil_w = fil.width*0.5, fil_h = fil.height*0.5;
  Size new_size(fil_w, fil_h);
  resize(tmp, tmp, new_size, 1, 1 , CV_INTER_AREA);

  //Shows filter image
  imshow("Filter", tmp);  

  //Save new image
  //imwrite("/home/iago/Documents/Processamento Digital de Imagens/1_unidade/iagolucas88/iagolucas88.github.io/filter.png", tmp);

  //Merge the images
  merge(comps, 2, filter);  

  return filter;
}

void applyFilter(void){
  vector<Mat> planos; planos.clear();
  Mat zeros = Mat_<float>::zeros(padded.size());
  Mat realInput = Mat_<float>(padded);
  Mat complex;
  realInput += Scalar::all(1);
  log(realInput,realInput);

  planos.push_back(realInput);
  planos.push_back(zeros);
  merge(planos, complex);

  dft(complex, complex);
  deslocaDFT(complex);
  resize(complex,complex,padded.size());
  normalize(complex,complex,0,1,CV_MINMAX);

  Mat filter = homomorphicFilter(gammaL,gammaH,sharpC,cutoff);

  mulSpectrums(complex,filter,complex,0);
  deslocaDFT(complex);
  idft(complex, complex);


  planos.clear();
  split(complex, planos);
  exp(planos[0],planos[0]);

  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
  imFiltered = planos[0].clone();
}


void on_trackbar(int, void*){
  gammaL = (double) gammaL_slider/10;
  gammaH = (double) gammaH_slider/10;
  sharpC = (double) sharpC_slider;
  cutoff = (double) cutoff_slider;
  applyFilter();
  imshow("Homomorphic",imFiltered);

  //Save new image
  imwrite("/home/iago/Documents/Processamento Digital de Imagens/1_unidade/iagolucas88/iagolucas88.github.io/homomorfico.png", imFiltered);
}


int main(int argc, char* argv[]){
  
  //Load image
  im = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  namedWindow("Original",WINDOW_AUTOSIZE);
  namedWindow("Filter",WINDOW_AUTOSIZE);
  namedWindow("Homomorphic", WINDOW_AUTOSIZE);

  if(!im.data){
    cout<<"Image did not open!\n";
    return -1;
  }

  dft_M = getOptimalDFTSize(im.rows);
  dft_N = getOptimalDFTSize(im.cols);
  copyMakeBorder(im, padded, 0, dft_M - im.rows, 0, dft_N - im.cols, BORDER_CONSTANT, Scalar::all(0));
  imFiltered = padded.clone();

  cout<<"Original: "<<im.rows<<'x'<<im.cols<<endl;
  cout<<"Padded: "<<padded.rows<<'x'<<padded.cols<<endl;
  cout<<"Press a key to quit!"<<endl;

  //Resizes original image for better view
  Size ori = im.size();
  int ori_w = ori.width*0.5, ori_h = ori.height*0.5;
  Size new_size(ori_w, ori_h);
  resize(im, im, new_size, 1, 1 , CV_INTER_AREA);
  
  //Shows original image
  imshow("Original",im);

  char TrackbarName[50]; 

  //Set the values and names for the gains
  sprintf( TrackbarName, "Gamma L x %d", gammaL_max );
  createTrackbar( TrackbarName, "Homomorphic", &gammaL_slider, gammaL_max, on_trackbar);

  sprintf( TrackbarName, "Gamma H x %d", gammaH_max );
  createTrackbar( TrackbarName, "Homomorphic", &gammaH_slider, gammaH_max, on_trackbar);

  sprintf( TrackbarName, "C x %d", sharpC_max );
  createTrackbar( TrackbarName, "Homomorphic", &sharpC_slider, sharpC_max, on_trackbar);

  sprintf( TrackbarName, "Cutoff Frequency x %d", cutoff_max );
  createTrackbar( TrackbarName, "Homomorphic", &cutoff_slider, cutoff_max, on_trackbar);
 
  waitKey(0);
  return 0;
}