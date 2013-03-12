//
//  ofxNiteUserTracker.cpp
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

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

#pragma mark ofxNiteSkeleton

//--------------------------------------------------------------
map<ofxNiteLimbType, ofxNiteLimb> ofxNiteSkeleton::getLimbs(){
    return limbs;
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
}

//--------------------------------------------------------------
void ofxNiteSkeleton::addVertices( ofxNiteLimb limb ){
    for (int i=0; i<limb.size(); i++){
        addVertex(limb[i]);
        addColor(ofFloatColor(1.0));
    }
}

#pragma mark ofxNiteUser

//--------------------------------------------------------------
ofxNiteUser::ofxNiteUser(){
    bHasSkeleton    = false;
    bIsVisible      = false;
}

//--------------------------------------------------------------
void ofxNiteUser::update( ofxNiteUserTracker & tracker, const nite::UserData& user, uint64_t ts ){
    bIsVisible = user.isVisible();
    niteSkeleton = user.getSkeleton();
    skeleton.clear();
    
    switch (user.getSkeleton().getState()) {
        case nite::SKELETON_NONE:
            bHasSkeleton = false;
            break;
		
        case nite::SKELETON_TRACKED:
            skeleton.update( tracker, niteSkeleton );
			break;
            
		case nite::SKELETON_CALIBRATING:
            ofLogVerbose()<<"Calibrating skeleton";
            break;
            
		case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
		case nite::SKELETON_CALIBRATION_ERROR_HANDS:
		case nite::SKELETON_CALIBRATION_ERROR_LEGS:
		case nite::SKELETON_CALIBRATION_ERROR_HEAD:
		case nite::SKELETON_CALIBRATION_ERROR_TORSO:
            bHasSkeleton = false;
            break;
    }
}

//--------------------------------------------------------------
void ofxNiteUser::draw(){
    skeleton.draw();
}

//--------------------------------------------------------------
ofxNiteSkeleton ofxNiteUser::getSkeleton(){
    return skeleton;
}

//--------------------------------------------------------------
nite::Skeleton ofxNiteUser::getNiteSkeleton(){
    return niteSkeleton;
}

//--------------------------------------------------------------
nite::SkeletonState ofxNiteUser::getNiteSkeletonState(){
    return niteSkeleton.getState();
}
    
//--------------------------------------------------------------
bool ofxNiteUser::isVisible(){
    return bIsVisible;
}

//--------------------------------------------------------------
void ofxNiteUser::updatePoints(){
    
}

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
openni::Status ofxNiteUserTracker::setup( string deviceUri ){
    // setup device
    openni::Status rc = ofxOpenNIFeed::setup(deviceUri);
    if ( rc != openni::STATUS_OK ){
        return openni::STATUS_ERROR;
    }
    
    // init nite
    if (! ofxNITE::niteInitialized ){
        cout <<"INIT"<<endl;
        nite::NiTE::initialize();
        ofxNITE::niteInitialized = true;
    }
    
    // setup tracker
	m_pUserTracker = new nite::UserTracker;
    
    if (m_pUserTracker->create(&m_device) != nite::STATUS_OK){
        cout << "ERRROR"<<endl;
		return openni::STATUS_ERROR;
	}
    
    bOpen = true;
}

//--------------------------------------------------------------
void ofxNiteUserTracker::draw( int x, int y){
    ofPushMatrix();{
        ofTranslate(x, y);
        ofxOpenNIFeed::draw(0,0);
        for (map<int, ofxNiteUser>::iterator it = current_users.begin(); it != current_users.end(); ++it){
            it->second.draw();
        }
        
    } ofPopMatrix();
}

//--------------------------------------------------------------
void ofxNiteUserTracker::close(){
    if ( bOpen ){
        bOpen = false;
        stop();
        m_pUserTracker->destroy();
        if ( m_pUserTracker != NULL ){
            delete m_pUserTracker;
            m_pUserTracker = NULL;
        }
        ofxOpenNIFeed::close();
        ofxNITE::shutDownNite();
    }
}


//--------------------------------------------------------------
void ofxNiteUserTracker::threadedFunction(){
    while (isThreadRunning()){
        process();
    }
}

//--------------------------------------------------------------
void ofxNiteUserTracker::process(){
    if ( !isValid() ){
        return;
    }
    
    nite::UserTrackerFrameRef userTrackerFrame;
	openni::VideoFrameRef depthFrame;
	nite::Status rc = m_pUserTracker->readFrame(&userTrackerFrame);
	if (rc != nite::STATUS_OK){
		printf("GetNextData failed\n");
		return;
	}
    
	depthFrame = userTrackerFrame.getDepthFrame();
    // update pixels of feed
    if ( depthFrame.isValid() ){
        updatePixels( depthFrame );
    }
    
    // current user pixels
	const nite::UserMap& userLabels = userTrackerFrame.getUserMap();
    
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
                current_users.erase(id);
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

