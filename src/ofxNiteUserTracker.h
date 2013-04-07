//
//  ofxNiteUserTracker.h
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"

#include "ofxNITE.h"

#include "ofxNiteUser.h"

// ofxNiteUserTracker

class ofxNiteUserTracker : protected ofThread
{
public:
    
    ofxNiteUserTracker();
    ~ofxNiteUserTracker();
    
    bool setup( string deviceUri = "", bool bUseRGB = true );
    void update();
    void draw( int x, int y);
    void close();
    
    // run NITE process
    void process();
    bool isValid();
    
    // hm... should this be a reference?
    map<int, ofxNiteUser> * getUsers();
    typedef map<int, ofxNiteUser>::iterator iterator;
    
    nite::UserTracker* getTracker();
    
protected:
    ofxOpenNI2GrabberSettings settings;
    ofxOpenNI2Grabber   oniGrabber;
    
	nite::UserTracker* m_pUserTracker;
    
	nite::UserId m_poseUser;
	uint64_t m_poseTime;
    
    map<int, ofxNiteUser>   current_users;
    vector<int>             toDelete;
    
    bool bOpen;
    
    void threadedFunction();
    
};

