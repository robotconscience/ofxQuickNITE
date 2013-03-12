//
//  ofxNiteUserTracker.h
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenNIFeed.h"
#include "ofxNITE.h"

// ofxNiteLimb

class ofxNiteUserTracker;
class ofxNiteLimb : public ofPolyline
{
public:
    
    void update( ofxNiteUserTracker & tracker, const nite::SkeletonJoint & joint1, const nite::SkeletonJoint & joint2 );
};

// ofxNiteSkeleton
enum ofxNiteLimbType {
    HEAD_TO_NECK,
    
	L_SHOULDER_ELBOW,
    L_ELBOW_HAND,
	R_SHOULDER_ELBOW,
    R_ELBOW_HAND,
    SHOULDERS,
    
    L_TORSO_TOP,
    L_TORSO_BOTTOM,
    R_TORSO_TOP,
    R_TORSO_BOTTOM,
    
    HIPS,
    
    L_HIP_KNEE,
    L_KNEE_FOOT,
    
    R_HIP_KNEE,
    R_KNEE_FOOT
};

// ofxNiteSkeleton

class ofxNiteSkeleton : public ofMesh
{
public:
    ofxNiteSkeleton();
    
    void update( ofxNiteUserTracker & tracker, nite::Skeleton skeleton );
    
    map<ofxNiteLimbType, ofxNiteLimb> & getLimbs();
    ofxNiteLimb getLimb( ofxNiteLimbType type );
    typedef map<ofxNiteLimbType, ofxNiteLimb>::iterator limbIterator;
    
protected:
    map<ofxNiteLimbType, ofxNiteLimb> limbs;
    
    void addVertices( ofxNiteLimb limb );
};

// ofxNiteUser

class ofxNiteUser
{
public:
    ofxNiteUser();
    
    void updateTexture();
    void update( ofxNiteUserTracker & tracker, const nite::UserData& user, uint64_t ts );
    void draw();
    
    bool                isVisible();
    bool                hasSkeleton();
    
    ofPixelsRef         getUserPixelsRef();
    unsigned char *     getUserPixels();
    ofTexture           getTexture();
    
    ofxNiteSkeleton     getSkeleton();
    
    nite::Skeleton      getNiteSkeleton();
    nite::SkeletonState getNiteSkeletonState();
    
    void setFromPixels( ofPixels & pix );
    
private:
    ofPixels    pixels;
    ofTexture   tex;
    
    ofxNiteSkeleton     skeleton;
    nite::Skeleton      niteSkeleton;
    bool bIsVisible, bHasSkeleton;
    
    void updatePoints();
};

// ofxNiteUserTracker

class ofxNiteUserTracker : public ofxOpenNIFeed, public ofxNiteProcess
{
public:
    
    ofxNiteUserTracker();
    ~ofxNiteUserTracker();
    
    openni::Status setup( string deviceUri = "" );
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
	nite::UserTracker* m_pUserTracker;
    
	nite::UserId m_poseUser;
	uint64_t m_poseTime;
    
    map<int, ofxNiteUser>   current_users;
    vector<int>             toDelete;
    
    bool bOpen;
    
    void threadedFunction();
    
};

