//
//  QuickOpenNI.h
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

#define MAX_DEPTH 10000

class ofxOpenNIFeed : public ofBaseVideo, public ofThread
{
public:
    
    ofxOpenNIFeed();
    ~ofxOpenNIFeed();
    
    virtual bool setup( string deviceUri = "" );
    virtual void update();
    bool isFrameNew();
    virtual void draw( int x, int y );
    void close();
    
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
    virtual void threadedFunction();
    
    void updatePixels( openni::VideoFrameRef & frameRef );
    
    openni::VideoStream         depth;
    openni::VideoFrameRef		depthFrame;
    
    ofTexture                   tex;
    
    unsigned int                width;
    unsigned int                height;
    
    bool                        bUseTexture;
    
    openni::Device              m_device;
    
    ofShortPixels               depthPixelsRaw;
    ofPixels                    depthPixels;
};



