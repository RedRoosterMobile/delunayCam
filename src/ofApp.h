#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofVideoGrabber cam;
    ofxDelaunay delaunay;
    ofVboMesh mesh;
    //ofxCvHaarFinder finder;
    
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage, edgeImage;
    
    int  w,h;
    bool fullScreen;
    bool colorFx;
    int  hueAngle;
    int  triangleSize;
    
    
    bool isSleeping;
    bool takeScreenshots;
    unsigned int  timeStart;
    unsigned int  timeEnd;
    unsigned long counter;
		
};
