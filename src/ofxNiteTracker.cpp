//
//  ofxNiteTracker.cpp
//  openTSPS
//
//  Created by BRenfer on 4/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteTracker.h"

//--------------------------------------------------------------
bool ofxNiteTracker::setup( string _deviceUri ){
    // setup device
    settings.width      = 320;
    settings.height     = 240;
    settings.deviceURI  = _deviceUri;
    settings.doColor    = false;
    
    bool bSetup = oniGrabber.setup(settings);
    if ( !bSetup ){
        return false;
    }
    pixels.allocate(320, 240, OF_PIXELS_MONO);
}

//--------------------------------------------------------------
void ofxNiteTracker::update(){
    oniGrabber.update();
    
    // invert the pixels
    
    ofPixels & pix = oniGrabber.getDepthPixels();
    
    for (int y = 0; y < pix.getHeight(); ++y)
    {   
        for (int x = 0; x < pix.getWidth(); ++x)
        {
            //depthPixelsRaw[ x + y * frameRef.getWidth() ] = *pDepth;
            
            unsigned char pixel = pix[ ( x + y * pix.getWidth()) * pix.getNumChannels()];
            
            pixels[ x + y * pix.getWidth()]     = pixel == 0 ? 0 : 255.0 - pixel;
        }
    }
}

//--------------------------------------------------------------
bool ofxNiteTracker::isFrameNew(){
    return true; //hmm
}

//--------------------------------------------------------------
void ofxNiteTracker::draw( int x, int y ){}

//--------------------------------------------------------------
void ofxNiteTracker::close(){
    oniGrabber.close();
}

//--------------------------------------------------------------
void ofxNiteTracker::setUseTexture( bool bUseTexture ){
    // hmm
}

//--------------------------------------------------------------
unsigned char*  ofxNiteTracker::getPixels(){
    if (settings.doColor){
        return oniGrabber.getRGBPixels().getPixels();
    } else {
        return pixels.getPixels();
    }
}

//--------------------------------------------------------------
ofPixels &      ofxNiteTracker::getPixelsRef(){
    if (settings.doColor){
        return oniGrabber.getRGBPixels();
    } else {
        return pixels;
    }
}

//--------------------------------------------------------------
unsigned char*  ofxNiteTracker::getDepthPixels(){
    return pixels.getPixels();
}

//--------------------------------------------------------------
unsigned short * ofxNiteTracker::getDepthPixelsRaw(){
    return oniGrabber.getDepthRawPixels().getPixels();
}

//--------------------------------------------------------------
ofShortPixels & ofxNiteTracker::getDepthPixelsRawRef(){
    return oniGrabber.getDepthRawPixels();
}

//--------------------------------------------------------------
int ofxNiteTracker::getWidth(){
    return oniGrabber.getDepthPixels().getWidth();
}

//--------------------------------------------------------------
int ofxNiteTracker::getHeight(){
    return oniGrabber.getDepthPixels().getHeight();
}

//--------------------------------------------------------------
float* ofxNiteTracker::getDistancePixels(){
    // tb implemented
}

//--------------------------------------------------------------
ofTexture& ofxNiteTracker::getDepthTextureReference(){
    return oniGrabber.getDepthTextureReference();
}

//--------------------------------------------------------------
openni::Device & ofxNiteTracker::getDevice(){
    return oniGrabber.deviceController.device;
}

//--------------------------------------------------------------
vector< openni::DeviceInfo > ofxNiteTracker::enumerateDevices(){
    return oniGrabber.deviceController.enumerateDevices();
}