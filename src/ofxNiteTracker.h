//
//  ofxNiteTracker.h
//  openTSPS
//
//  Created by BRenfer on 4/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxNITE.h"

class ofxNiteTracker
{
public:
    ofxNiteTracker(){};
    ~ofxNiteTracker(){};
    
    virtual bool setup( string deviceUri = "" );
    virtual void update();
    bool isFrameNew();
    virtual void draw( int x, int y );
    virtual void close();
    
    void setUseTexture( bool bUseTexture = true );
    
    unsigned char*  getPixels();
    ofPixels &      getPixelsRef();
    unsigned char*  getDepthPixels();
    
    unsigned short * getDepthPixelsRaw();
    ofShortPixels & getDepthPixelsRawRef();
    
    int getWidth();
    int getHeight();
    
    /// get the distance in millimeters to a given point as a float array
    float* getDistancePixels();
    
    /// get the grayscale depth texture
    ofTexture& getDepthTextureReference();
    
    openni::Device & getDevice();
    vector< openni::DeviceInfo > enumerateDevices();
    
protected:
    
    ofPixels                    pixels;
    
    ofxOpenNI2GrabberSettings   settings;
    ofxOpenNI2Grabber           oniGrabber;
};

