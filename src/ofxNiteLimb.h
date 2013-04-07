//
//  ofxNiteLimb.h
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once

// ofxNiteLimb
#include "ofxNITE.h"

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

