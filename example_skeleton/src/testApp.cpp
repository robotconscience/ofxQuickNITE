#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    userTracker.setup();
    userTracker.start();
}

//--------------------------------------------------------------
void testApp::update(){
    userTracker.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString("Wave your hand to start hand tracking", 20,20);
    
    userTracker.draw(20, 40);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

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