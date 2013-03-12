//
//  ofxNiteProcess.cpp
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNiteProcess.h"
#include "ofxNITE.h"

void ofxNiteProcess::start(){
    ofxNITE::niteQueue().addTracker(*this);
    ofxNITE::niteQueue().start();
}

void ofxNiteProcess::stop(){
    ofxNITE::niteQueue().removeTracker(*this);
}