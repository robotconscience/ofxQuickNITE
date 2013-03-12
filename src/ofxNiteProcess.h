//
//  ofxNiteProcess.h
//  This file exists so you can run multiple NiTE processes on multiple
//  cameras. If you try to just run them as threads, they tend to smash
//  into each other. This will probably eventually be deprecated...

#pragma once

class ofxNiteProcess
{
public:
    
    virtual bool    isValid() = 0;
    virtual void    process() = 0;
    
    virtual void start();
    virtual void stop();
};

