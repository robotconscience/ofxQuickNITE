#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "DeviceController.h"

using namespace openni;

class DepthSource : public VideoStream::NewFrameListener
{
public:
	DepthSource();
	int width;
	int height;
	VideoStream videoStream;
	
	ofTexture texture;
	ofPixels pixels[2];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	
	ofShortPixels rawPixels[2];
	ofShortPixels* backRawPixels;
	ofShortPixels* currentRawPixels;
	
	VideoMode videoMode;
	
	VideoFrameRef videoFrameRef;
	bool setup(DeviceController& deviceController, bool bUseONIThread = true);
	void allocateBuffers();
	void update();
	void draw();
	void close();
	int	deviceMaxDepth;
    
    // called by openni;;VideoStream::NewFrameListener
	void onNewFrame(VideoStream&);
    
    // called by either onNewFrame or manually (if not using oniThread)
    void updateFrame( VideoFrameRef & frameRef);
    
	bool doDoubleBuffering;
	bool isOn;
	bool doRawDepth;
};

