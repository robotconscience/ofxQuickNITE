//
//  ofxNiteSkeleton.cpp
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteSkeleton.h"

#pragma mark ofxNiteSkeleton

//--------------------------------------------------------------
ofxNiteSkeleton::ofxNiteSkeleton(){
    limbs[ HEAD_TO_NECK ].addVertex(ofPoint(0,0));
}

//--------------------------------------------------------------
map<ofxNiteLimbType, ofxNiteLimb> & ofxNiteSkeleton::getLimbs(){
    return limbs;
}

//--------------------------------------------------------------
ofxNiteLimb ofxNiteSkeleton::getLimb( ofxNiteLimbType type ){
    if ( limbs.count(type) > 0){
        return  limbs[type];
    } else {
        return ofxNiteLimb();
    }
}

//--------------------------------------------------------------
void ofxNiteSkeleton::update( ofxNiteUserTracker & tracker, nite::Skeleton skeleton ){
    clear();
    setupIndicesAuto();
    setMode(OF_PRIMITIVE_LINES);
    
    limbs.clear();
    
    limbs[ HEAD_TO_NECK ].update(tracker, skeleton.getJoint(nite::JOINT_HEAD), skeleton.getJoint(nite::JOINT_NECK));
    
    limbs[ L_SHOULDER_ELBOW ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), skeleton.getJoint(nite::JOINT_LEFT_ELBOW));
    limbs[ L_ELBOW_HAND ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_ELBOW), skeleton.getJoint(nite::JOINT_LEFT_HAND));
    
    limbs[ R_SHOULDER_ELBOW].update(tracker, skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER), skeleton.getJoint(nite::JOINT_RIGHT_ELBOW));
    limbs[ R_ELBOW_HAND].update(tracker, skeleton.getJoint(nite::JOINT_RIGHT_ELBOW), skeleton.getJoint(nite::JOINT_RIGHT_HAND));
    
    limbs[ SHOULDERS ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER));
    
    limbs[ L_TORSO_TOP ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_SHOULDER), skeleton.getJoint(nite::JOINT_TORSO));
    limbs[ R_TORSO_TOP ].update(tracker, skeleton.getJoint(nite::JOINT_RIGHT_SHOULDER), skeleton.getJoint(nite::JOINT_TORSO));
    limbs[ L_TORSO_BOTTOM ].update(tracker, skeleton.getJoint(nite::JOINT_TORSO), skeleton.getJoint(nite::JOINT_LEFT_HIP));
    limbs[ R_TORSO_BOTTOM ].update(tracker, skeleton.getJoint(nite::JOINT_TORSO), skeleton.getJoint(nite::JOINT_RIGHT_HIP));
    
    limbs[ HIPS ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_HIP), skeleton.getJoint(nite::JOINT_RIGHT_HIP));
    
    limbs[ L_HIP_KNEE ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_HIP), skeleton.getJoint(nite::JOINT_LEFT_KNEE));
    limbs[ L_KNEE_FOOT ].update(tracker, skeleton.getJoint(nite::JOINT_LEFT_KNEE), skeleton.getJoint(nite::JOINT_LEFT_FOOT));
    limbs[ R_HIP_KNEE ].update(tracker, skeleton.getJoint(nite::JOINT_RIGHT_HIP), skeleton.getJoint(nite::JOINT_RIGHT_KNEE));
    limbs[ R_KNEE_FOOT ].update(tracker, skeleton.getJoint(nite::JOINT_RIGHT_KNEE), skeleton.getJoint(nite::JOINT_RIGHT_FOOT));
    
    for (limbIterator it = limbs.begin(); it != limbs.end(); it++){
        addVertices( it->second );
    }
}

//--------------------------------------------------------------
void ofxNiteSkeleton::addVertices( ofxNiteLimb limb ){
    for (int i=0; i<limb.size(); i++){
        addVertex(limb[i]);
        addColor(ofFloatColor(1.0));
    }
}