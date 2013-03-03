//
//  ofxNiteHandTracker.h
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "OpenNIFeed.h"

#include "NiTE.h"

class ofxNiteHandTracker;

namespace ofxNITE {
    static bool niteInitialized = false;
    
    static vector<ofxNiteHandTracker *> trackers;
    
    class NiteQueue_ : public ofThread {
    public:
        
        void addTracker( ofxNiteHandTracker & handTracker );
        void removeTracker( ofxNiteHandTracker & handTracker );
        void start();
        void stop();
        void clear();
        
    protected:
        vector<ofxNiteHandTracker *> trackers;
        
        void threadedFunction();
    };
    
    // queue singleton
    NiteQueue_ & niteQueue();
    
    // there must be a better place to clear this queue?
    static void shutDownNite(){
        if ( ofxNITE::niteInitialized ){
            niteQueue().clear();
            nite::NiTE::shutdown();
            ofxNITE::niteInitialized = false;
        }
    }
    
}

class ofxNiteHandTracker : public OpenNIFeed, public nite::HandTracker::NewFrameListener
{
public:
    ofxNiteHandTracker();
    ~ofxNiteHandTracker();
    
    openni::Status setup( string deviceUri = "" );
    void draw( int x, int y);
    void close();
    
    // smoothing 0 = none
    void setSmoothing( float smoothing = 0.0 );
    
    map<int, ofPoint> getHands();
    map<int, ofPoint> getRawHands();
    ofPoint getHand( int id );
    ofPoint getRawHand( int id );
    
    // run NITE process
    void process();
    
    void start();
    void stop();
    
    void onNewFrame( nite::HandTracker& tracker);
    
    bool canProcess(){
        return bCanProcess;
    }
    
protected:
    void threadedFunction();
    
private:
    bool bCanProcess;
    map<int,vector<ofPoint> >   worldSpacePoints;
    map<int,vector<ofPoint> >   screenSpacePoints;
    map<int, ofPoint>           currentHands;
    map<int, ofPoint>           currentRawHands;
    int                         historySize;
    
    int cameraWidth, cameraHeight;
    nite::HandTracker* m_pHandTracker;
    
    bool bOpen;
};

