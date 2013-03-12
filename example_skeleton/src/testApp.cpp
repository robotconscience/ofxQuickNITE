#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    userTracker.setup();
    userTracker.start();
}

//--------------------------------------------------------------
void testApp::update(){
    userTracker.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofDrawBitmapString("Wave your hand to start hand tracking", 20,20);
    
    userTracker.draw(20, 40);
    
    // you can get the current users like this:
    map<int, ofxNiteUser> users = userTracker.getUsers();
    ofxNiteUserTracker::iterator it = users.begin(); // same as saying map<int, ofxNiteUser>::iterator
    for ( it; it != users.end(); it++){
        // do things with each user?
        // it->second is the user
        //it->second.hasSkeleton(); boolean
        //it->second.getUserPixelsRef(); user mask
        //it->second.getSkeleton() ofxNiteSkeleton (aka ofMesh)
    }
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