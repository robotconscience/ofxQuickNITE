//
//  ofxNITE.h
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "NiTE.h"
#include "ofxNiteProcess.h"

class ofxNiteHandTracker;

namespace ofxNITE {
    static bool niteInitialized = false;
    
    static vector<ofxNiteProcess *> trackers;
    
    class NiteQueue_ : public ofThread {
    public:
        
        void addTracker( ofxNiteProcess & handTracker );
        void removeTracker( ofxNiteProcess & handTracker );
        void start();
        void stop();
        void clear();
        
        vector<ofxNiteProcess *> & getTrackers();
        
    protected:
        vector<ofxNiteProcess *> trackers;
        
        void threadedFunction();
    };
    
    // queue singleton
    NiteQueue_ & niteQueue();
    
    // there must be a better place to clear this queue?
    static void shutDownNite(){
        if ( ofxNITE::niteInitialized ){
            nite::NiTE::shutdown();
            if ( niteQueue().isThreadRunning() && Poco::Thread::current() != &niteQueue().getPocoThread() ){
                niteQueue().waitForThread(true);
                niteQueue().clear();
                cout << "Cleared nite queue "<<endl;
            }
            ofxNITE::niteInitialized = false;
        }
    }
    
}

class ofxNiteCalibrationEvent
{
public:
    ofPoint             position;
    nite::GestureType   type;
};

class ofxNiteHandEvent
{
public:
    ofPoint position;
    ofPoint velocity;
    int     id;
};
