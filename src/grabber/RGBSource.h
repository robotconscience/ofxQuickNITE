#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "DeviceController.h"
using namespace openni;

class RGBSource : public VideoStream::NewFrameListener
{
public:
	RGBSource();
	int width;
	int height;
	VideoStream videoStream;
	
	ofTexture texture;
	ofPixels pixels[2];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	
	VideoMode videoMode;
	
	VideoFrameRef videoFrameRef;
	bool setup(DeviceController& deviceController, bool bUseONIThread = true);
	void allocateBuffers();
	
	void close();
	void update();
	void draw();
	
	bool doDoubleBuffering;

    // called by openni;;VideoStream::NewFrameListener
	void onNewFrame(VideoStream&);
    
    // called by either onNewFrame or manually (if not using oniThread)
    void updateFrame( VideoFrameRef & frameRef);
	
	bool isOn;
};

