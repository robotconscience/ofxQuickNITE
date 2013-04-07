//
//  ofxNiteUserTracker.cpp
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteUserTracker.h"

#pragma mark ofxNiteUserTracker

//--------------------------------------------------------------
ofxNiteUserTracker::ofxNiteUserTracker():
m_poseUser(0){
    m_pUserTracker = NULL;
    bOpen = false;
}

//--------------------------------------------------------------
ofxNiteUserTracker::~ofxNiteUserTracker(){
    close();
}

//--------------------------------------------------------------
bool ofxNiteUserTracker::setup( string deviceUri, bool bUseRGB ){
    // setup device
    settings.deviceURI  = deviceUri;
    settings.doColor    = bUseRGB;
    
    bool bSetup = oniGrabber.setup(settings);
    if ( !bSetup ){
        return false;
    }
    
    // init nite
    if (! ofxNITE::niteInitialized ){
        nite::NiTE::initialize();
        ofxNITE::niteInitialized = true;
    }
    
    // setup tracker
	m_pUserTracker = new nite::UserTracker;
    
    if (m_pUserTracker->create(&oniGrabber.deviceController.device) != nite::STATUS_OK){
		return false;
	}
    
    bOpen = true;
    
    startThread();
    return true;
}

//--------------------------------------------------------------
void ofxNiteUserTracker::update(){
    
    lock();
    for (int i=0; i<toDelete.size(); i++){
        current_users.erase(toDelete[i]);
    }
    unlock();
    toDelete.clear();
    
    for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); ++it){
        it->second.updateTexture();
    }
}

//--------------------------------------------------------------
void ofxNiteUserTracker::draw( int x, int y){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofPushMatrix();{
        ofTranslate(x, y);
        ofSetColor(255, 150);
        oniGrabber.draw();
        ofSetColor(255);
        for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); ++it){
            it->second.draw();
        }
        
    } ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxNiteUserTracker::close(){
    if ( bOpen ){
        stopThread();
        bOpen = false;
//        stop();
        m_pUserTracker->destroy();
        if ( m_pUserTracker != NULL ){
            delete m_pUserTracker;
            m_pUserTracker = NULL;
        }
//        ofxOpenNIFeed::close();
        oniGrabber.close();
        ofxNITE::shutDownNite();
    }
}


//--------------------------------------------------------------
void ofxNiteUserTracker::threadedFunction(){
    while (isThreadRunning() && isValid()){
        lock();
        process();
        unlock();
        yield();
    }
}

//--------------------------------------------------------------
void ofxNiteUserTracker::process(){
    if ( !isValid() ){
        return;
    }
    
    static nite::UserTrackerFrameRef userTrackerFrame;
	static openni::VideoFrameRef depthFrame;
	nite::Status rc = m_pUserTracker->readFrame(&userTrackerFrame);
	if (rc != nite::STATUS_OK){
		printf("GetNextData failed\n");
		return;
	}
    
    
	depthFrame = userTrackerFrame.getDepthFrame();
    // update pixels of feed
    if ( depthFrame.isValid() ){
        //oniGrabber.depthSource.updateFrame(depthFrame);
    }
    
    // current user pixels
	const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
    const nite::UserId * userPixels = userLabels.getPixels();
    
    static map<int,ofPixels> pix;
    pix.clear();
    
    for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); it++){
        pix[it->first].allocate(depthFrame.getWidth(), depthFrame.getHeight(), 4);
    }
    
    for (int y = 0; y < depthFrame.getHeight(); ++y)
    {
        for (int x = 0; x < depthFrame.getWidth(); ++x, ++userPixels)
        {
            for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); it++){
                pix[it->first][(x + y * depthFrame.getWidth())*4] = *userPixels == 0 ? 0 : ( *userPixels == it->first ? 255 : 0);
                pix[it->first][(x + y * depthFrame.getWidth())*4 + 1] = *userPixels == 0 ? 0 : ( *userPixels == it->first ? 0 : 0);
                pix[it->first][(x + y * depthFrame.getWidth())*4 + 2] = *userPixels == 0 ? 0 : ( *userPixels == it->first ? 0 : 0);
                pix[it->first][(x + y * depthFrame.getWidth())*4 + 3] = *userPixels == 0 ? 0 : ( *userPixels == it->first ? 255 : 0);
            }
        }
    }
    
    for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); ++it){
        it->second.setFromPixels(pix[it->first]);
    }
    
    // current users
    const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
	for (int i = 0; i < users.getSize(); ++i)
	{
		const   nite::UserData& user = users[i];
        int     id = user.getId();
        
        if ( user.isNew() ){
            current_users[id] = ofxNiteUser();
            
            m_pUserTracker->startSkeletonTracking(user.getId());
			m_pUserTracker->startPoseDetection(user.getId(), nite::POSE_CROSSED_HANDS);
            current_users[id].update(*this, user, userTrackerFrame.getTimestamp());
            
        } else if ( user.isLost() ){
            if ( current_users.count(id) > 0 ){
                toDelete.push_back(id);
                // can't erase here because of the 'ol textures-in-threads problem
                //current_users.erase(id);
            }
        } else {
            current_users[id].update(*this, user, userTrackerFrame.getTimestamp());
        }
    }
}


//--------------------------------------------------------------
bool ofxNiteUserTracker::isValid(){
    if ( !bOpen || m_pUserTracker == NULL || !m_pUserTracker->isValid() ) return false;
    return true;
}


//--------------------------------------------------------------
nite::UserTracker* ofxNiteUserTracker::getTracker(){
    return m_pUserTracker;
}

//--------------------------------------------------------------
map<int, ofxNiteUser> * ofxNiteUserTracker::getUsers(){
    return &current_users;
}

