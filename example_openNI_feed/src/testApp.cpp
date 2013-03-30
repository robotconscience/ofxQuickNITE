#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    openNIFeed.setUseTexture(true);
    openNIFeed.setup();
    openNIFeed.startThread();
}

//--------------------------------------------------------------
void testApp::update(){
    openNIFeed.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    openNIFeed.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}