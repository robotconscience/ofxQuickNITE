//
//  ofxNiteUser.h
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

// ofxNiteUser

#include "ofxNITE.h"
#include "ofxNiteSkeleton.h"

class ofxNiteUserTracker;
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