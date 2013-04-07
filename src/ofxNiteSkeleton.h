//
//  ofxNiteSkeleton.h
//  example_usermasks
//
//  Created by BRenfer on 4/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofxNITE.h"
#include "ofxNiteLimb.h"

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

