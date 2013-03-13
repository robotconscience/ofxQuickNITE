//
//  ofxNiteProcess.h
//  This file exists so you can run multiple NiTE processes on multiple
//  cameras. If you try to just run them as threads, they tend to smash
//  into each other. This will probably eventually be deprecated...

#pragma once

#include "ofMain.h"

class ofxNiteProcess
{
public:
    
    virtual bool    isValid() = 0;
    virtual void    process() = 0;
    
    virtual bool open( string deviceUri = "" ) = 0;
    virtual void close() = 0;
    
    virtual void start();
    virtual void stop();
    
    virtual string getURI(){
        return deviceUri;
    }
    
protected:
    string deviceUri;
};

