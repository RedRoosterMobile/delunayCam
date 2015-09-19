#include "ofApp.h"
// https://www.honeycomb-lab.co.jp/blog/tech/?p=1038
//--------------------------------------------------------------
void ofApp::setup(){
    //w=1440;
    //h=900;
    w=640;
    h=480;
    ofSetWindowShape(w, h);
    
    cam.setDesiredFrameRate(30);
    cam.initGrabber(w, h);
    
    grayImage.allocate(w, h);
    edgeImage.allocate(w, h);
    
    fullScreen = false;
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
        ofPixels edgeData = edgeImage.getPixels();
        
        int up = 80;
        for (int i=0; i<w*h; i+=up) {
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
            //int hueAngle = (int)(360/(float)w * mouseX) - 180;
            int hueAngle = (int)(360/(float)w * mouseX) - 180;
            color.setHueAngle(hueAngle);
            color.setSaturation(mouseY);
            
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

void changeHue() {
    // START CALCULATE HUE (todo: stuff into method and refresh pointer kung fu :-)
    /*
    double cosv,sinv;
    cosv = cos(angle * PI / 180);
    sinv = sin(angle * PI / 180);
    double matrix [9] = {
        1, 0, 0,   // Reds
        0, 1, 0,   // Greens
        0, 0, 1    // Blues
    };
    
    
    // taken from webkit:
    // /Source/WebCore/platform/graphics/texmap/TextureMapperShaderProgram.cpp
    matrix[0] = 0.213 + cosv * 0.787 - sinv * 0.213;
    matrix[1] = 0.715 - cosv * 0.715 - sinv * 0.715;
    matrix[2] = 0.072 - cosv * 0.072 + sinv * 0.928;
    
    matrix[3] = 0.213 - cosv * 0.213 + sinv * 0.143;
    matrix[4] = 0.715 + cosv * 0.285 + sinv * 0.140;
    matrix[5] = 0.072 - cosv * 0.072 - sinv * 0.283;
    
    matrix[6] = 0.213 - cosv * 0.213 - sinv * 0.787;
    matrix[7] = 0.715 - cosv * 0.715 + sinv * 0.715;
    matrix[8] = 0.072 + cosv * 0.928 + sinv * 0.072;
    
    double r,g,b;
    r = (double)color.rgbRed;
    g = (double)color.rgbGreen;
    b = (double)color.rgbBlue;
    
    color.rgbRed = clamp(matrix[0] * r + matrix[1] * g + matrix[2] * b);
    color.rgbGreen = clamp(matrix[3] * r + matrix[4] * g + matrix[5] * b);
    color.rgbBlue = clamp(matrix[6] * r + matrix[7] * g + matrix[8] * b);
    */
    // END CALCULATE HUE
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    //ofSetColor(123,123,123);
    mesh.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'f':
            
            ofSetFullscreen(fullScreen);
            fullScreen = !fullScreen;
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
    
    mouseX = x;
    
    mouxeY = y;
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
