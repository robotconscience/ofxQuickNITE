#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    bool bOpen = handTracker.open();
    cout <<"Successfully opened? "<<bOpen<<endl;
    handTracker.start();
    
    ofAddListener(handTracker.calibrationStarted, this, &testApp::onCalibrationStarted);
    ofAddListener(handTracker.calibrationComplete, this, &testApp::onCalibrationComplete);
}

//--------------------------------------------------------------
void testApp::update(){
    handTracker.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString("Wave your hand to start hand tracking", 20,20);
    
    handTracker.draw(20, 40);
    
    // get hands and do stuff with them
    map<int, ofPoint> hands = handTracker.getHands();
    map<int, ofPoint>::iterator it = hands.begin();
    
    ofPushMatrix();
    
    // scale up to screen dimensions
    ofScale(ofGetWidth() / handTracker.getWidth(), ofGetHeight() / handTracker.getHeight());
    
    for ( it; it != hands.end(); ++it){
        ofCircle(it->second, 20);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::onCalibrationStarted( ofxNiteCalibrationEvent & e ){
    cout << "calibration started!" << endl;
}

//--------------------------------------------------------------
void testApp::onCalibrationComplete( ofxNiteCalibrationEvent & e ){
    cout << "calibration complete!" << endl;
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