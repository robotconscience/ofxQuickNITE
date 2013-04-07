//
//  ofxNiteLimb.cpp
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteLimb.h"
#include "ofxNiteUserTracker.h"

#pragma mark ofxNiteLimb

//--------------------------------------------------------------
void ofxNiteLimb::update( ofxNiteUserTracker & tracker, const nite::SkeletonJoint & joint1, const nite::SkeletonJoint & joint2){
    clear();
    static ofPoint p1;
    static ofPoint p2;
    
	tracker.getTracker()->convertJointCoordinatesToDepth(joint1.getPosition().x, joint1.getPosition().y, joint1.getPosition().z, &p1.x, &p1.y);
	tracker.getTracker()->convertJointCoordinatesToDepth(joint2.getPosition().x, joint2.getPosition().y, joint2.getPosition().z, &p2.x, &p2.y);
    
    addVertex(p1);
    addVertex(p2);
}