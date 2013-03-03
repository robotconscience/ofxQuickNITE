//
//  ofxNiteHandTracker.cpp
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteHandTracker.h"

namespace ofxNITE {
    NiteQueue_ & niteQueue(){
        static NiteQueue_ * niteQueue = new NiteQueue_;
        return *niteQueue;
    }
    
    void NiteQueue_::addTracker( ofxNiteHandTracker & handTracker ){
        lock();
        trackers.push_back(&handTracker);
        unlock();
    }
    
    void NiteQueue_::removeTracker( ofxNiteHandTracker & handTracker ){
        lock();
        for (int i=0; i<trackers.size(); i++){
            if ( trackers[i] == &handTracker ){
                trackers.erase( trackers.begin() + i );
                return;
            }
        }
        unlock();
        
    }

    void NiteQueue_::start(){
        if ( isThreadRunning() ) return;
        startThread();
    }

    void NiteQueue_::stop(){
        stopThread();
    }

    void NiteQueue_::clear(){
        trackers.clear();
    }

    void NiteQueue_::threadedFunction(){
        while( isThreadRunning() ){
            for (int i=0; i<trackers.size(); i++){
                if ( trackers[i]->canProcess()) trackers[i]->process();
//                sleep(10);
            }
            yield();
        }
    }
};

//--------------------------------------------------------------
ofxNiteHandTracker::ofxNiteHandTracker(){
    historySize = 20;
    bOpen = false;
    m_pHandTracker = NULL;
    bCanProcess = false;
}

//--------------------------------------------------------------
ofxNiteHandTracker::~ofxNiteHandTracker(){
    close();
}

//--------------------------------------------------------------
openni::Status ofxNiteHandTracker::setup( string deviceUri ){
    // setup device
    openni::Status rc = OpenNIFeed::setup(deviceUri);
    if ( rc != openni::STATUS_OK ){
        return openni::STATUS_ERROR;
    }
    
    m_pHandTracker = new nite::HandTracker;
    
    if (m_pHandTracker->create( &m_device ) != nite::STATUS_OK)
    {
        return openni::STATUS_ERROR;
    }
    
    m_pHandTracker->startGestureDetection(nite::GESTURE_WAVE);
    m_pHandTracker->startGestureDetection(nite::GESTURE_CLICK);
    
    cameraWidth = getWidth();
    cameraHeight = getHeight();
    
    // init nite
    if (! ofxNITE::niteInitialized ){
        nite::NiTE::initialize();
        ofxNITE::niteInitialized = true;
    }
    
    bOpen = true;
    m_pHandTracker->addNewFrameListener(this);
}

//--------------------------------------------------------------
void ofxNiteHandTracker::close(){
    if ( bOpen ){
        bOpen = false;
        stop();
        ofxNITE::shutDownNite();
        if ( m_pHandTracker != NULL ){
            delete m_pHandTracker;
            m_pHandTracker = NULL;
        }
        OpenNIFeed::close();
    }
}

//--------------------------------------------------------------
void ofxNiteHandTracker::draw( int x, int y){
    OpenNIFeed::draw(x,y);
    
    ofPushMatrix();
    ofTranslate(x,y);
    
    ofPushStyle();
    ofSetColor(255,255,0);
    ofNoFill();
    
    for ( map<int, vector<ofPoint> >::iterator it = screenSpacePoints.begin(); it != screenSpacePoints.end(); ++it)
    {
        ofBeginShape();
        vector<ofPoint> hist = it->second;
        for (int i = 0; i < hist.size(); ++i)
        {
            ofVertex(hist[i]);
        }
        ofEndShape();
    }
    ofPopStyle();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofxNiteHandTracker::threadedFunction(){
    while ( isThreadRunning() ){
        process();
        sleep(10);
    }
}

//--------------------------------------------------------------
void ofxNiteHandTracker::process(){
    bCanProcess = false;
    nite::HandTrackerFrameRef handFrame;
    openni::VideoFrameRef depthFrame;
    if ( !m_pHandTracker->isValid() ) return;
    
    nite::Status rc = m_pHandTracker->readFrame(&handFrame);
    
    if (rc != nite::STATUS_OK)
    {
        printf("GetNextData failed\n");
        return;
    }
    depthFrame = handFrame.getDepthFrame();
    
    // update pixels of feed
    if ( depthFrame.isValid() ){
        updatePixels( depthFrame );
    }
    
    const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        if (gestures[i].isComplete())
        {
            const nite::Point3f& position = gestures[i].getCurrentPosition();
//            printf("Gesture %d at (%f,%f,%f)\n", gestures[i].getType(), position.x, position.y, position.z);
            
            nite::HandId newId;
            m_pHandTracker->startHandTracking(gestures[i].getCurrentPosition(), &newId);
        }
    }
    
    const nite::Array<nite::HandData>& hands= handFrame.getHands();
    for (int i = 0; i < hands.getSize(); ++i)
    {
        const nite::HandData& user = hands[i];
        
        if (!user.isTracking())
        {
            nite::HandId id = user.getId();
            lock();
            worldSpacePoints.erase(id);
            screenSpacePoints.erase(id);
            currentHands.erase(id);
            unlock();
        }
        else
        {
            lock();
            if (user.isNew())
            {
                worldSpacePoints[user.getId()] = vector<ofPoint>();
            }
            // Add to worldSpacePoints
            worldSpacePoints[user.getId()].push_back( ofPoint( user.getPosition().x, user.getPosition().y, user.getPosition().z ) );
            if ( worldSpacePoints[user.getId()].size() > historySize ){
                worldSpacePoints[user.getId()].erase(worldSpacePoints[user.getId()].begin());
            }
            
            // add to screen space points
            ofPoint converted;
            ofPoint p = worldSpacePoints[user.getId()].back();
            m_pHandTracker->convertHandCoordinatesToDepth(p.x, p.y, p.z, &converted.x, &converted.y);
            screenSpacePoints[user.getId()].push_back( converted );
            
            // add to current hand vector
            currentHands[user.getId()] = converted;
            currentRawHands[user.getId()] = worldSpacePoints[user.getId()].back();
            
            unlock();
        }
    }
}


//--------------------------------------------------------------
void ofxNiteHandTracker::start(){
    ofxNITE::niteQueue().addTracker(*this);
    ofxNITE::niteQueue().start();
}

//--------------------------------------------------------------
void ofxNiteHandTracker::stop(){
    ofxNITE::niteQueue().waitForThread();
    ofxNITE::niteQueue().removeTracker(*this);
}

//--------------------------------------------------------------
void ofxNiteHandTracker::setSmoothing( float smoothing ){
    m_pHandTracker->setSmoothingFactor(smoothing);
}

//--------------------------------------------------------------
map<int, ofPoint> ofxNiteHandTracker::getHands(){
    return currentHands;
}

//--------------------------------------------------------------
map<int, ofPoint> ofxNiteHandTracker::getRawHands(){
    return currentRawHands;
}

//--------------------------------------------------------------
void ofxNiteHandTracker::onNewFrame( nite::HandTracker& tracker){
    bCanProcess = true;
}

//--------------------------------------------------------------
ofPoint ofxNiteHandTracker::getRawHand( int id ){
    if ( currentRawHands.count(id) > 0 ){
        return currentRawHands[id];
    } else {
        ofLogWarning()<<"No hand at this id!";
        return ofPoint(0,0);
    }
}

//--------------------------------------------------------------
ofPoint ofxNiteHandTracker::getHand( int id ){
    if ( currentHands.count(id) > 0 ){
        return currentHands[id];
    } else {
        ofLogWarning()<<"No hand at this id!";
        return ofPoint(0,0);
    }
}
