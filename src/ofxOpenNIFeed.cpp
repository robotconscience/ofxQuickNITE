//
//  QuickOpenNI.cpp
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxOpenNIFeed.h"

//--------------------------------------------------------------
ofxOpenNIFeed::ofxOpenNIFeed()
{
    bUseTexture = true;
}

//--------------------------------------------------------------
ofxOpenNIFeed::~ofxOpenNIFeed()
{
    close();
}

//--------------------------------------------------------------
openni::Status ofxOpenNIFeed::setup( string uri )
{
    if (!ofxOpenNI::bInitialized){
        ofxOpenNI::bInitialized = true;
        openni::OpenNI::initialize();
    }
    const char* deviceUri = (uri == "" ? openni::ANY_DEVICE : uri.c_str());
    
    openni::Status rc = m_device.open(deviceUri);
    if (rc != openni::STATUS_OK)
    {
        printf("Open Device failed:\n%s\n", openni::OpenNI::getExtendedError());
        return rc;
    }
    rc = depth.create(m_device, openni::SENSOR_DEPTH);
    
    if (rc == openni::STATUS_OK)
    {
        rc = depth.start();
        if (rc != openni::STATUS_OK)
        {
            printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
            depth.destroy();
        }
    } else {
        printf("Create depth stream failed:\n%s\n", openni::OpenNI::getExtendedError());
        return rc;
    }
    
    if (!depth.isValid())
    {
        printf("SimpleViewer: No valid streams. Exiting\n");
        openni::OpenNI::shutdown();
        return openni::STATUS_ERROR;
    }
    
    // initialize pixels
    openni::VideoMode depthVideoMode;
    depthVideoMode = depth.getVideoMode();
    
    height = depthVideoMode.getResolutionY();
    width = depthVideoMode.getResolutionX();
    
    depthPixelsRaw.allocate(width, height, 1);
    depthPixels.allocate(width, height, 1);
    tex.allocate(width, height, GL_LUMINANCE);
    return openni::STATUS_OK;
}

//--------------------------------------------------------------
void ofxOpenNIFeed::update(){
    if ( m_device.isValid() && bUseTexture ){
        tex.loadData(depthPixels);
    }
}

//--------------------------------------------------------------
void ofxOpenNIFeed::close(){
    openni::OpenNI::shutdown();
}

//--------------------------------------------------------------
bool ofxOpenNIFeed::isFrameNew(){
    return true;
}

//--------------------------------------------------------------
void ofxOpenNIFeed::setUseTexture( bool useTexture ){
    bUseTexture = useTexture;
}

//--------------------------------------------------------------
void ofxOpenNIFeed::draw( int x, int y ){
    tex.draw(x,y);
}

//--------------------------------------------------------------
void ofxOpenNIFeed::start(){
    startThread();
}

//--------------------------------------------------------------
void ofxOpenNIFeed::stop(){
    stopThread();
}

//--------------------------------------------------------------
unsigned char* ofxOpenNIFeed::getDepthPixels(){
    return depthPixels.getPixels();
}

//--------------------------------------------------------------
float* ofxOpenNIFeed::getDistancePixels(){
    
}

//--------------------------------------------------------------
unsigned short * ofxOpenNIFeed::getDepthPixelsRaw(){
    return depthPixelsRaw.getPixels();
}

//--------------------------------------------------------------
ofShortPixels & ofxOpenNIFeed::getDepthPixelsRawRef(){
    return depthPixelsRaw;
}

//--------------------------------------------------------------
ofTexture& ofxOpenNIFeed::getDepthTextureReference(){
    return tex;
}

//--------------------------------------------------------------
unsigned char*  ofxOpenNIFeed::getPixels(){
    return depthPixels.getPixels();
}

//--------------------------------------------------------------
ofPixels & ofxOpenNIFeed::getPixelsRef(){
    return depthPixels;
}

//--------------------------------------------------------------
openni::Device & ofxOpenNIFeed::getDevice(){
    return m_device;
}

//--------------------------------------------------------------
int ofxOpenNIFeed::getWidth(){
    return width;
}

//--------------------------------------------------------------
int ofxOpenNIFeed::getHeight(){
    return height;
}

//--------------------------------------------------------------
vector< openni::DeviceInfo > ofxOpenNIFeed::enumerateDevices(){
    if (!ofxOpenNI::bInitialized){
        ofxOpenNI::bInitialized = true;
        openni::OpenNI::initialize();
    }
    
    static vector< openni::DeviceInfo > devices;
    devices.clear();
    static openni::Array< openni::DeviceInfo > oniDeviceArray;
    
    openni::OpenNI::enumerateDevices( &oniDeviceArray );
    for (int i=0; i<oniDeviceArray.getSize(); i++){
        devices.push_back( oniDeviceArray[i] );
    }
    return devices;
}

//--------------------------------------------------------------
void ofxOpenNIFeed::threadedFunction(){
    while ( isThreadRunning() ){
        int changedIndex;
        static openni::VideoStream ** streams = new openni::VideoStream*[1];
        streams[0] = &depth;
        openni::Status rc = openni::OpenNI::waitForAnyStream(streams, 1, &changedIndex);
        if (rc == openni::STATUS_OK)
        {
            depth.readFrame(&depthFrame);
            // check if we need to draw depth frame to texture
            if (depthFrame.isValid() && bUseTexture)
            {
                updatePixels(depthFrame);
            }
        }
        yield();
//        sleep(1);
    }
}

//--------------------------------------------------------------
void ofxOpenNIFeed::updatePixels( openni::VideoFrameRef & frameRef ){
    const openni::DepthPixel* pDepthRow = (const openni::DepthPixel*) frameRef.getData();
    int rowSize = frameRef.getStrideInBytes() / sizeof(openni::DepthPixel);
    memcpy((unsigned short *) pDepthRow, depthPixelsRaw.getPixels(), sizeof(depthPixelsRaw));
    
    for (int y = 0; y < height; ++y)
    {
        const openni::DepthPixel* pDepth = pDepthRow;
        
        for (int x = 0; x < width; ++x, ++pDepth)
        {
            //depthPixelsRaw[ x + y * frameRef.getWidth() ] = *pDepth;
            depthPixels[ x + y * width]     = *pDepth == 0 ? 0 : 255.0 - (*pDepth / 10000.0 * 255.0);
        }
        pDepthRow += rowSize;
    }
}