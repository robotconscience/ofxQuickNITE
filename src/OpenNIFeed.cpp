//
//  QuickOpenNI.cpp
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "OpenNIFeed.h"

//--------------------------------------------------------------
OpenNIFeed::OpenNIFeed()
{
    bUseTexture = true;
}

//--------------------------------------------------------------
OpenNIFeed::~OpenNIFeed()
{
    close();
}

//--------------------------------------------------------------
openni::Status OpenNIFeed::setup( string uri )
{
    if (!bOpenNIInitialized){
        bOpenNIInitialized = true;
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
void OpenNIFeed::update(){
    if ( m_device.isValid() && bUseTexture ){
        tex.loadData(depthPixels);
    }
}

//--------------------------------------------------------------
void OpenNIFeed::close(){
    openni::OpenNI::shutdown();
}

//--------------------------------------------------------------
bool OpenNIFeed::isFrameNew(){
    return true;
}

//--------------------------------------------------------------
void OpenNIFeed::setUseTexture( bool useTexture ){
    bUseTexture = useTexture;
}

//--------------------------------------------------------------
void OpenNIFeed::draw( int x, int y ){
    tex.draw(x,y);
}

//--------------------------------------------------------------
void OpenNIFeed::start(){
    startThread();
}

//--------------------------------------------------------------
void OpenNIFeed::stop(){
    stopThread();
}

//--------------------------------------------------------------
unsigned char* OpenNIFeed::getDepthPixels(){
    return depthPixels.getPixels();
}

//--------------------------------------------------------------
float* OpenNIFeed::getDistancePixels(){
    
}

//--------------------------------------------------------------
unsigned short * OpenNIFeed::getDepthPixelsRaw(){
    return depthPixelsRaw.getPixels();
}

//--------------------------------------------------------------
ofShortPixels & OpenNIFeed::getDepthPixelsRawRef(){
    return depthPixelsRaw;
}

//--------------------------------------------------------------
ofTexture& OpenNIFeed::getDepthTextureReference(){
    return tex;
}

//--------------------------------------------------------------
unsigned char*  OpenNIFeed::getPixels(){
    return depthPixels.getPixels();
}

//--------------------------------------------------------------
ofPixels & OpenNIFeed::getPixelsRef(){
    return depthPixels;
}

//--------------------------------------------------------------
openni::Device & OpenNIFeed::getDevice(){
    return m_device;
}

//--------------------------------------------------------------
int OpenNIFeed::getWidth(){
    return width;
}

//--------------------------------------------------------------
int OpenNIFeed::getHeight(){
    return height;
}

//--------------------------------------------------------------
vector< openni::DeviceInfo > OpenNIFeed::enumerateDevices(){
    if (!bOpenNIInitialized){
        bOpenNIInitialized = true;
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
void OpenNIFeed::threadedFunction(){
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
void OpenNIFeed::updatePixels( openni::VideoFrameRef & frameRef ){
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