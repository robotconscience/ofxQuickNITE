//
//  ofxNiteHandTracker.h
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "ofxOpenNIFeed.h"

#include "ofxNITE.h"

class ofxNiteHandTracker : public ofxOpenNIFeed, public ofxNiteProcess //, public nite::HandTracker::NewFrameListener
{
public:
    ofxNiteHandTracker();
    ~ofxNiteHandTracker();
    
    typedef map<int,ofPoint>::iterator handIterator;
    
    bool setup( string deviceUri = "" );
    void draw( int x, int y);
    void close();
    
    // events
    ofEvent<ofxNiteCalibrationEvent> calibrationStarted;
    ofEvent<ofxNiteCalibrationEvent> calibrationComplete;
    ofEvent<ofxNiteHandEvent> handMoved;
    ofEvent<ofxNiteHandEvent> handLost;
    
    // smoothing 0 = none
    void setSmoothing( float smoothing = 0.0 );
    
    // add start gesture(s)
    // starts tracking immediately if tracker is already started
    void addStartGesture( nite::GestureType type );
    
    // remove start gesture
    void removeStartGesture( nite::GestureType type );
    
    // get low-level tracker
    nite::HandTracker* getTracker();
    
    map<int, ofPoint> getHands();
    map<int, ofPoint> getRawHands();
    ofPoint getHand( int id );
    ofPoint getRawHand( int id );
    
    // run NITE process
    void process();
    bool isValid();
    
//    void onNewFrame( nite::HandTracker& tracker);
    
//    bool canProcess();
    
protected:
    void threadedFunction();
    
private:
    vector<nite::GestureType>   trackingGestures;
    bool bCanProcess;
    map<int,vector<ofPoint> >   worldSpacePoints;
    map<int,vector<ofPoint> >   screenSpacePoints;
    map<int, ofPoint>           inProgressHands;
    map<int, ofPoint>           currentHands;
    map<int, ofPoint>           currentRawHands;
    int                         historySize;
    
    int cameraWidth, cameraHeight;
    nite::HandTracker* m_pHandTracker;
    
    bool                        bOpen;
};

