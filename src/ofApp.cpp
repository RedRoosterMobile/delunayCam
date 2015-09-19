#include "ofApp.h"
// https://www.honeycomb-lab.co.jp/blog/tech/?p=1038
// http://www.myu.ac.jp/~xkozima/lab/ofTutorial5.html
// https://raw.githubusercontent.com/Itseez/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml


// emscripten video grabber SUPPORTED!
// https://github.com/openframeworks/openFrameworks/pull/2992


#define RANDOMNESS 5
#define TRIANGLE_SIZE 40
#define FRAME_RATE 30
#define SCREEN_SIZE "test" // "test" for small
#define HUE_MAX 360

//--------------------------------------------------------------
void ofApp::setup(){
    
    if (SCREEN_SIZE=="test") {
        w=640;
        h=480;
    } else {
        // mac 15" low res
        w=1440;
        h=900;
    }
    colorFx = true;
    hueAngle = 90;
    
    ofSetWindowShape(w, h);
    

    //ofSetVerticalSync(true);
    ofSetFrameRate(FRAME_RATE);
    cam.setDesiredFrameRate(FRAME_RATE);
    cam.initGrabber(w, h);
    
    grayImage.allocate(w, h);
    edgeImage.allocate(w, h);
    
    fullScreen = false;
    
    //finder.setup("haarcascade_frontalface_alt.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if (cam.isFrameNew()) {
        delaunay.reset();
        mesh.clear();
        
        // get the image fromcam
        colorImage.setFromPixels(cam.getPixels(), w, h);
        // turn to grayscale
        grayImage = colorImage;
        grayImage.blur();
        
        // get edges from grayImage
        cvCanny(grayImage.getCvImage(), edgeImage.getCvImage(), 20, 100);
        edgeImage.flagImageChanged();
        // find some edges
        edgeImage.dilate();
        
        // alternative
        //finder.findHaarObjects(cam.getPixels());
        
        
        ofPixels edgeData = edgeImage.getPixels();
        
        // the bigger up , the bigger triangle size (lower value is more cpu intense)
        int up = TRIANGLE_SIZE;
        
        
        for (int i=0; i<w*h; i+=(up+ofRandom(0, RANDOMNESS)) ) {
            if (edgeData[i] == 0){ continue;}
            else{
                // add edge point
                int coordY = floor(i/w);
                delaunay.addPoint(ofVec2f(i-w*coordY, coordY));
            }
        }
        // add additional random points
        for (int i=0; i<400; i++) {
            delaunay.addPoint(ofPoint(ofRandom(0, w),ofRandom(0, h)));
        }
        // add corner points
        delaunay.addPoint(ofPoint(0,0));
        delaunay.addPoint(ofPoint(0,h));
        delaunay.addPoint(ofPoint(w,h));
        delaunay.addPoint(ofPoint(w,0));
        // calculate mesh
        delaunay.triangulate();
        
        ofPixels pixels = cam.getPixels();
        for (int i=0; i<delaunay.triangleMesh.getNumIndices()/3; i++) {
            int idx1 = delaunay.triangleMesh.getIndex(i*3);
            int idx2 = delaunay.triangleMesh.getIndex(i*3+1);
            int idx3 = delaunay.triangleMesh.getIndex(i*3+2);
            
            ofVec2f v1 = delaunay.triangleMesh.getVertex(idx1);
            ofVec2f v2 = delaunay.triangleMesh.getVertex(idx2);
            ofVec2f v3 = delaunay.triangleMesh.getVertex(idx3);
            
            ofVec2f gp = (v1+v2+v3)/3.0;
            
            ofColor color = pixels.getColor((int)gp.x, (int)gp.y);
            if (colorFx) {
                color.setHueAngle(hueAngle);
                //color.setSaturation(mouseY);
            }
            
            // todo: change saturationhere?
            
            mesh.addVertex(v1);
            mesh.addVertex(v2);
            mesh.addVertex(v3);
            
            mesh.addColor(color);
            mesh.addColor(color);
            mesh.addColor(color);
            
        }
    }
    
    //ofVbo vbo = mesh.getVbo();


}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    //ofSetColor(123,123,123);
    mesh.draw();
    //for(int i = 0; i < finder.blobs.size(); i++) {
    //    ofRect( finder.blobs[i].boundingRect );
    //}
    
    ofDrawBitmapString(ofToString(ofGetFrameRate())+" fps", 10, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'f':
            ofSetFullscreen(fullScreen);
            fullScreen = !fullScreen;
            break;
        case 'c':
            colorFx= !colorFx;
            break;
        case 'e':
            cam.videoSettings();
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    hueAngle = (int)(360/(float)w * y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
