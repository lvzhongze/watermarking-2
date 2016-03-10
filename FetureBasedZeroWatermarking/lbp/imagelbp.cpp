#include <cv.h>
#include <highgui.h>
#include "lbp.hpp"
#include "histogram.hpp"
#include <string.h>
#include <stdio.h>

using namespace cv;

int main(int argc, const char *argv[]) {
    if( argc != 4)
    {
     cout <<" Usage: ./app src dst lbp_operator" << endl;
     return -1;
    }
    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
	// initial values
    int radius = 2;
    int neighbors = 16;

    // windows
    namedWindow("original",CV_WINDOW_AUTOSIZE);
    namedWindow("lbp",CV_WINDOW_AUTOSIZE);

    // matrices used
    Mat dst; // image after preprocessing
    Mat lbp; // lbp image

    // just to switch between possible lbp operators
    vector<string> lbp_names;
    lbp_names.push_back("Extended LBP"); // 0
    lbp_names.push_back("Fixed Sampling LBP"); // 1
    lbp_names.push_back("Variance-based LBP"); // 2
    int lbp_operator;
    lbp_operator = atoi(argv[3]);

        bool running=true;
    	cvtColor(image , dst, CV_BGR2GRAY);
    	GaussianBlur(dst, dst, Size(7,7), 5, 3, BORDER_CONSTANT); // tiny bit of smoothing is always a good idea
    	// comment the following lines for original size
    	resize(image , image , Size(), 0.5, 0.5);
    	//resize(dst,dst,Size(), 0.5, 0.5);
    	resize(dst,dst,Size(), 0.125 , 0.125 );
        //
    	switch(lbp_operator) {
    	case 0:
    		lbp::ELBP(dst, lbp, radius, neighbors); // use the extended operator
    		break;
    	case 1:
    		lbp::OLBP(dst, lbp); // use the original operator
    		break;
    	case 2:
    		lbp::VARLBP(dst, lbp, radius, neighbors);
    		break;
    	}
    	// now to show the patterns a normalization is necessary
    	// a simple min-max norm will do the job...
    	normalize(lbp, lbp, 0, 255, NORM_MINMAX, CV_8UC1);

    
        int ch = lbp.channels();
    
        for(int y=0; y<lbp.rows; ++y) {
            for(int x=0; x<lbp.cols; ++x) {
                int a = lbp.step*y+(x*ch);
                
                if(lbp.data[a+0]>128) lbp.data[a+0] = 0;
                else lbp.data[a+0] = 255;
                if(lbp.data[a+1]>128) lbp.data[a+1] = 0;
                else lbp.data[a+1] = 255;
                if(lbp.data[a+2]>128) lbp.data[a+2] = 0;
                else lbp.data[a+2] = 255;
                
                //cout << "[" << (int)lbp.data[a+0] << "," << (int)lbp.data[a+1] << "," << (int)lbp.data[a+2] << "]" << endl;
            }
        }
    
    
        imshow(argv[1], image );
        imshow(argv[2], lbp);
    
        waitKey(0);
    
        //imwrite(argv[1], image);
        imwrite(argv[2], lbp);

    	return 0; // success
}
