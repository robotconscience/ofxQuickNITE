#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    userTracker.setup();
    userTracker.startThread();
}

//--------------------------------------------------------------
void testApp::update(){
    userTracker.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(255);
    userTracker.draw(0,0);
    
    // you can get the current users like this:
//    map<int, ofxNiteUser> users = userTracker.getUsers();
//    ofxNiteUserTracker::iterator it = users.begin(); // same as saying map<int, ofxNiteUser>::iterator
//    
//    ofPushStyle();
//    ofSetColor(0,255,0);
//    ofSetLineWidth(5.0);
//    
//    for ( it; it != users.end(); it++){
//        // do things with each user?
//        // it->second is the user
//        //it->second.hasSkeleton(); boolean
//        //it->second.getUserPixelsRef(); user mask
//        if ( it->second.hasSkeleton() ){
//            ofxNiteSkeleton skeleton = it->second.getSkeleton(); //ofxNiteSkeleton (aka ofMesh)
//            skeleton.getLimbs()[L_SHOULDER_ELBOW].draw();
//        }
//    }
    
    ofPopStyle();
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