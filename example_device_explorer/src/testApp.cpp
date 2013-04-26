#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofxOpenNI2Grabber oniGrabber;
    OpenNI::initialize();
    devices = oniGrabber.deviceController.enumerateDevices();
    
    cout << devices.size() << " devices found "<<endl;
    
    for ( int i=0; i<devices.size(); i++){
        grabbers.push_back( new ofxOpenNI2Grabber());
        ofxOpenNI2GrabberSettings settings;
        settings.doColor = false;
        settings.deviceURI = devices[i].getUri();
        grabbers.back()->setup(settings);
    }
    
    ofEnableAlphaBlending();
    textFade = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    for ( int i=0; i<grabbers.size(); i++){
        grabbers[i]->update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    float x, y;
    x = 0;
    y = 40;
    ofSetColor(255);
    ofDrawBitmapString("Click on any device to copy URI to clipboard!", 20, 20);
    for ( int i=0; i<grabbers.size(); i++){
        ofPushMatrix();
        ofTranslate(x, y);
        grabbers[i]->depthSource.texture.draw(0,0);
        ofPopMatrix();
        
        // draw settings
        string s = "index: " + ofToString( i ) + "\n";
        s += "uri: " + ofToString( devices[i].getUri()) + "\n";
        ofDrawBitmapString(s, ofPoint(x+20, y + 20));
        
        x += grabbers[i]->settings.width;
        if ( x + grabbers[i]->settings.width > ofGetWidth()){
            x = 0;
            y += grabbers[i]->settings.height;
        }
    }
    
    ofSetColor(255,textFade);
    ofDrawBitmapString("Copied "+clipboard+" to clipboard", 400, 20 );
    
    textFade *= .95;
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
    // set text to clipboard if you clicked on a screen
    float grid_x, grid_y;
    grid_x = grid_y = 0;
    for ( int i=0; i<grabbers.size(); i++){
        
        if ( x >= grid_x
            && x <= grid_x + grabbers[i]->settings.width
            && y >= grid_y
            && y <= grid_y + grabbers[i]->settings.height)
        {
            clipboard = ofToString( devices[i].getUri());
            textFade = 500;
            setTextToPasteboard( (char*) clipboard.c_str() );
        }
        
        grid_x += grabbers[i]->settings.width;
        if ( grid_x + grabbers[i]->settings.width > ofGetWidth()){
            grid_x = 0;
            grid_y += grabbers[i]->settings.height;
        }
    }
    
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