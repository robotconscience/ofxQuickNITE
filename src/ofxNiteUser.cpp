//
//  ofxNiteUser.cpp
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteUser.h"

#pragma mark ofxNiteUser

//--------------------------------------------------------------
ofxNiteUser::ofxNiteUser(){
    bHasSkeleton    = false;
    bIsVisible      = false;
    pixels.allocate(320, 240, 4);
}

//--------------------------------------------------------------
void ofxNiteUser::updateTexture(){
    if ( !tex.bAllocated() || pixels.getWidth() != tex.getWidth() ){
        tex.allocate( pixels.getWidth(), pixels.getHeight(), GL_RGBA);
    }
    tex.loadData(pixels);
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
            bHasSkeleton = true;
            skeleton.update( tracker, niteSkeleton );
			break;
            
		case nite::SKELETON_CALIBRATING:
            ofLogVerbose()<<"Calibrating skeleton";
            bHasSkeleton = false;
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
    ofEnableAlphaBlending();
    tex.draw(0,0);
    if ( bHasSkeleton ){
        skeleton.draw();
    }
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
ofPixelsRef ofxNiteUser::getUserPixelsRef(){
    return pixels;
}

//--------------------------------------------------------------
unsigned char * ofxNiteUser::getUserPixels(){
    return pixels.getPixels();
}

ofTexture ofxNiteUser::getTexture(){
    return tex;
}

//--------------------------------------------------------------
bool ofxNiteUser::hasSkeleton(){
    return bHasSkeleton;
}

//--------------------------------------------------------------
bool ofxNiteUser::isVisible(){
    return bIsVisible;
}

//--------------------------------------------------------------
void ofxNiteUser::updatePoints(){
    
}

//--------------------------------------------------------------
void ofxNiteUser::setFromPixels( ofPixels & pix ){
    pixels = pix;
}
