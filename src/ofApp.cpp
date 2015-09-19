#include "ofApp.h"
// https://www.honeycomb-lab.co.jp/blog/tech/?p=1038
// http://www.myu.ac.jp/~xkozima/lab/ofTutorial5.html
// https://raw.githubusercontent.com/Itseez/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml


// emscripten video grabber SUPPORTED!
// https://github.com/openframeworks/openFrameworks/pull/2992


#define RANDOMNESS 5
#define TRIANGLE_SIZE_MIN 20
#define TRIANGLE_SIZE_MAX 200
#define TARGET_FRAME_RATE 30
#define SCREEN_SIZE "mac" // "test" for small
#define TARGET_FRAME_TOLERANCE 3

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
    triangleSize = 40;
    
    ofSetWindowShape(w, h);
    

    //ofSetVerticalSync(true);
    ofSetFrameRate(TARGET_FRAME_RATE);
    cam.setDesiredFrameRate(TARGET_FRAME_RATE);
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
        
        for (int i=0; i<w*h; i+=(triangleSize+ofRandom(0, RANDOMNESS)) ) {
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
    
    int frameRate = ofGetFrameRate();
    
    if ( frameRate < TARGET_FRAME_RATE-TARGET_FRAME_TOLERANCE) {
        ofSetColor(204,0,0);
    }
    ofDrawBitmapString("fps:           " + ofToString((int)ofGetFrameRate()), 10, 15);
    ofSetColor(255);
    ofDrawBitmapString("hue angle:     " + ofToString(hueAngle), 10, 30);
    ofDrawBitmapString("triangle size: " + ofToString(triangleSize), 10, 45);
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
    triangleSize = (int) TRIANGLE_SIZE_MIN + ofClamp(TRIANGLE_SIZE_MAX/(float)w *x,0,TRIANGLE_SIZE_MAX-TRIANGLE_SIZE_MIN);
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
