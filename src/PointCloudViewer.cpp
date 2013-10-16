/*****************************************************************************
*   Based partially on:
*   Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*****************************************************************************/

#include "PointCloudViewer.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

namespace mcv {
void PointCloudViewerDrawCallback(void* param){
    PointCloudViewer * ctx = static_cast<PointCloudViewer*>(param);
    if (ctx)
        ctx->draw();
}

PointCloudViewer::PointCloudViewer(std::string windowName, cv::Size frameSize)
  : m_isTextureInitialized(false)
  , m_windowName(windowName)
  , size(frameSize)
  , translate_(false)
  , rotate_(false){
    angle=0;
    // Create window with OpenGL support
    cv::namedWindow(windowName, CV_WINDOW_OPENGL);
    // Resize it exactly to video size
    cv::resizeWindow(windowName, frameSize.width, frameSize.height);
    // Initialize OpenGL draw callback:
    cv::setOpenGlContext(windowName);
    cv::setOpenGlDrawCallback(windowName, PointCloudViewerDrawCallback, this);
}

PointCloudViewer::~PointCloudViewer(){
    cv::setOpenGlDrawCallback(m_windowName, 0, 0);
}

void PointCloudViewer::updatePointCloud(const Point3Cloud &cloud){
    m_pointCloud = Point3Cloud(cloud);
}

void PointCloudViewer::translate(float x, float y, float z){
    translations[0]=x;
    translations[1]=y;
    translations[2]=z;
    translate_=true;
}

void PointCloudViewer::rotate(float _angle, float x, float y, float z){
    rotation[0]=x;
    rotation[1]=y;
    rotation[2]=z;
    angle=_angle;
    rotate_=true;
}

void PointCloudViewer::updateWindow(){
    cv::updateWindow(m_windowName);
}

void PointCloudViewer::draw(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear entire screen:
    drawScene(); // Draw PC
    glFlush();
}

void PointCloudViewer::drawScene(){

    glPolygonMode(GL_FRONT,GL_FILL);  // set the drawing mode to full rendering
    glEnable(GL_DEPTH_TEST);          //activate Z buffer (hide elements in the back)

    drawPointCloud();
}

void PointCloudViewer::drawPointCloud(){
    glPushMatrix();
    glViewport(0, 0, size.width, size.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double znear = m_pointCloud.bBDistance*0.1;
    double zfar = m_pointCloud.bBDistance*5;

    gluPerspective(
                    45, // 45 deg is ok
                    (float)size.width/size.height,
                    znear,
                    zfar // same
                  );

    cv::Vec3f cameraTarget = m_pointCloud.bBCenter;
    cv::Vec3f cameraPosition = cameraTarget + cv::Vec3f(m_pointCloud.bBDistance,-m_pointCloud.bBDistance,0);

    gluLookAt(
        cameraPosition[0], -cameraPosition[1], cameraPosition[2],
        cameraTarget[0], -cameraTarget[1], cameraTarget[2],
        0,-1,0); //Up Vector, do not change this

    glMatrixMode(GL_MODELVIEW);
    glPointSize(1.5);
    glBegin(GL_POINTS);

    //bool color_on=false;
    cv::Mat bgr, points;
    m_pointCloud.getBgr(bgr);
    m_pointCloud.getData(points);

    for( int i=0 ; i<points.rows; i++ ){
        for( int j=0 ; j<points.cols; j++ ){
            //if(color_on){
                cv::Vec3b bgrPixel = bgr.at<Vec3b>(i,j);
                glColor3b(bgrPixel[2],bgrPixel[1],bgrPixel[0]);
            //}
            cv::Vec3f theP = points.at<Vec3f>(i,j);
            glVertex3f(theP[0],-theP[1],theP[2]);
        }
    }

    glEnd();

    if (translate_){
        glTranslatef(translations[0],translations[1],translations[2]);
        translate_=false;
    }
    if (rotate_){
        glRotatef(angle, rotation[0],rotation[1],rotation[2]);
        rotate_=false;
    }
}

}//mcv
