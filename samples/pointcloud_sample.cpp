/*M///////////////////////////////////////////////////////////////////////////////////////
Copyright (c) 2013, Master in Computer Vision Project, France
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//M*/

// std //
#include <string>
#include <iostream>
#include <cmath>
// mcv //
#include "PointCloud.hpp"
#include "DrawingContext.hpp"
#include "PointCloudViewer.hpp"
// cv/gl //
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


using namespace cv;
using namespace std;


int main( int argc, char * argv[] ){


    mcv::Point3Cloud mypc;
    mcv::PointCloudViewer view("MCV AR", cv::Size(640,480));


    if (argc>1){
        mypc.readFrame(argv[1]);
    }else{
        VideoCapture capture( CV_CAP_OPENNI );
        if (capture.isOpened()){
            capture.set( CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ );
            cout<<"Registration: "<<capture.get( CV_CAP_PROP_OPENNI_REGISTRATION )<<endl;
            capture.set( CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION, CV_CAP_PROP_OPENNI_REGISTRATION_ON );
            cout<<"Registration: "<<capture.get( CV_CAP_PROP_OPENNI_REGISTRATION )<<endl;
            mypc.grabFrame(capture);
        }else{
            cout<<"ERROR, NO FILE GIVEN AND NO 3D CAMERA CONNECTED"<<endl;
            return -1;
        }
    }

    cv::Mat color;
    mypc.getBgr(color);
    view.updatePointCloud(mypc);
    cv::imshow("TEST",color);

    for (;;){
        view.updateWindow();

        int key = cv::waitKey(30);
        //if (key>0) cout<<"KEY: "<<key<<endl;
        const float speed = 0.25;
        switch (key){
        case 's':
        case 65362:
            view.translate(speed, 0, 0);
            break;
        case 65361:
        case 'a':
            view.translate(0, 0, speed);
            break;
        case 65364:
        case 'w':
            view.translate(-speed, 0, 0);
            break;
        case 65363:
        case 'd':
            view.translate(0, 0, -speed);
            break;
        case 't':
            view.translate(0, speed, 0);
            break;
        case 'y':
            view.translate(0, -speed, 0);
            break;
        case 'q':
            view.rotate(speed, 1,0,0);
            break;
        case 'e':
            view.rotate(-speed, 1,0,0);
            break;
        case 'z':
            view.rotate(speed, 0,1,0);
            break;
        case 'c':
            view.rotate(-speed, 0,1,0);
            break;
        case 'r':
            view.rotate(speed, 0,0,1);
            break;
        case 'f':
            view.rotate(-speed, 0,0,1);
            break;
        }

        if (key==27)
            break;
    }

    return 0;
}
