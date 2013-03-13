//
//  ofxNiteHandTracker.cpp
//  NiteHandDemo
//
//  Created by BRenfer on 2/28/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteHandTracker.h"

//--------------------------------------------------------------
ofxNiteHandTracker::ofxNiteHandTracker(){
    historySize = 20;
    bOpen = false;
    m_pHandTracker = NULL;
    bCanProcess = false;
    addStartGesture( nite::GESTURE_WAVE );
}

//--------------------------------------------------------------
ofxNiteHandTracker::~ofxNiteHandTracker(){
    close();
}

//--------------------------------------------------------------
bool ofxNiteHandTracker::open( string _deviceUri ){
    cout <<"OPENING! "<<_deviceUri<<endl;
    // setup device
    deviceUri = _deviceUri;
    openni::Status rc = ofxOpenNIFeed::setup(deviceUri);
    if ( rc != openni::STATUS_OK ){
        return false;
    }
    
    m_pHandTracker = new nite::HandTracker;
    
    if (m_pHandTracker->create( &m_device ) != nite::STATUS_OK)
    {
        return false;
    }
    
    for (int i=0; i<trackingGestures.size(); i++){
        m_pHandTracker->startGestureDetection(trackingGestures[i]);
    }
    
    cameraWidth = getWidth();
    cameraHeight = getHeight();
    
    // init nite
    if (! ofxNITE::niteInitialized ){
        nite::NiTE::initialize();
        ofxNITE::niteInitialized = true;
    }
    
    bOpen = true;
//    if we want to use the nite thread. maybe this is a good idea?
//    m_pHandTracker->addNewFrameListener(this);
    return true;
}

//--------------------------------------------------------------
void ofxNiteHandTracker::close(){
    cout <<"CLOSE"<<endl;
    if ( bOpen ){
        bOpen = false;
        //stop();
        m_pHandTracker->destroy();
        if ( m_pHandTracker != NULL ){
            delete m_pHandTracker;
            m_pHandTracker = NULL;
        }
        ofxOpenNIFeed::close();
        ofxNITE::shutDownNite();
    }
}

//--------------------------------------------------------------
void ofxNiteHandTracker::draw( int x, int y){
    ofxOpenNIFeed::draw(x,y);
    
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
        yield();
    }
}

//--------------------------------------------------------------
bool ofxNiteHandTracker::isValid(){
    if ( !bOpen || m_pHandTracker == NULL || !m_pHandTracker->isValid() ) return false;
    return true;
}

//--------------------------------------------------------------
void ofxNiteHandTracker::process(){
    if (!isValid()) return;
    
    bCanProcess = false;
    nite::HandTrackerFrameRef handFrame;
    openni::VideoFrameRef depthFrame;
    
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
    
    // get current gestures
    const nite::Array<nite::GestureData>& gestures = handFrame.getGestures();
    for (int i = 0; i < gestures.getSize(); ++i)
    {
        ofxNiteCalibrationEvent args;
        const nite::Point3f& position = gestures[i].getCurrentPosition();
        m_pHandTracker->convertHandCoordinatesToDepth(position.x, position.y, position.z, &args.position.x, &args.position.y);
        args.type = gestures[i].getType();
        
        if (gestures[i].isComplete())
        {
            ofNotifyEvent(calibrationComplete, args, this);
            nite::HandId newId;
            m_pHandTracker->startHandTracking(position, &newId);
            
        // NOTE: this doesn't work yet with OpenNI...
        // not complete + not in progress = new
        } else if (gestures[i].isInProgress() && !gestures[i].isComplete()){
            ofNotifyEvent(calibrationStarted, args, this);
        }
    }
    
    // update current hands
    const nite::Array<nite::HandData>& hands= handFrame.getHands();
    for (int i = 0; i < hands.getSize(); ++i)
    {
        const nite::HandData& user = hands[i];
        
        if (!user.isTracking())
//        if (user.isLost())
        {
            nite::HandId id = user.getId();
            
            ofxNiteHandEvent args;
            args.id         = id;
            ofNotifyEvent(handLost, args, this);
            
            lock();
            worldSpacePoints.erase(id);
            screenSpacePoints.erase(id);
            currentHands.erase(id);
            unlock();
        } else {
            lock();
            if (user.isNew()){
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
            
            ofxNiteHandEvent args;
            args.position   = converted;
            args.id         = user.getId();
            if ( screenSpacePoints.size() > 1 ){
                args.velocity   = args.position - screenSpacePoints[user.getId()][screenSpacePoints.size()-1];
            }
            ofNotifyEvent(handMoved, args, this);
            
            unlock();
        }
    }
}

//--------------------------------------------------------------
void ofxNiteHandTracker::setSmoothing( float smoothing ){
    m_pHandTracker->setSmoothingFactor(smoothing);
}

//--------------------------------------------------------------
void ofxNiteHandTracker::addStartGesture( nite::GestureType type ){
    // are we already tracking this gesture?
    for (int i=0; i<trackingGestures.size(); i++){
        if ( trackingGestures[i] == type ) return;
    }
    
    trackingGestures.push_back(type);
    
    if ( m_pHandTracker != NULL ){
        m_pHandTracker->startGestureDetection(type);
    }
}

//--------------------------------------------------------------
void ofxNiteHandTracker::removeStartGesture( nite::GestureType type ){
    bool bFound = false;
    for (int i=0; i<trackingGestures.size(); i++){
        if ( trackingGestures[i] == type ){
            trackingGestures.erase( trackingGestures.begin() + i );
            bFound = true;
            break;
        }
    }
    
    if (!bFound) return;
    
    if ( m_pHandTracker != NULL ){
        m_pHandTracker->stopGestureDetection(type);
    }
}

//--------------------------------------------------------------
nite::HandTracker* ofxNiteHandTracker::getTracker(){
    
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
//void ofxNiteHandTracker::onNewFrame( nite::HandTracker& tracker){
//    bCanProcess = true;
//}

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
