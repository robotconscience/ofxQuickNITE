//
//  ofxNITE.cpp
//  example_skeleton
//
//  Created by BRenfer on 3/11/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ofxNITE.h"

namespace ofxNITE {
    NiteQueue_ & niteQueue(){
        static NiteQueue_ * niteQueue = new NiteQueue_;
        return *niteQueue;
    }
    
    void NiteQueue_::addTracker( ofxNiteProcess & handTracker ){
        lock();
        trackers.push_back(&handTracker);
        unlock();
    }
    
    void NiteQueue_::removeTracker( ofxNiteProcess & handTracker ){
        lock();
        for (int i=0; i<trackers.size(); i++){
            if ( trackers[i] == &handTracker ){
                trackers.erase( trackers.begin() + i );
                unlock();
                return;
            }
        }
        unlock();
        
    }
    
    vector<ofxNiteProcess *> & NiteQueue_::getTrackers(){
        return trackers;
    }
    
    void NiteQueue_::start(){
        if ( isThreadRunning() ) return;
        startThread();
    }
    
    void NiteQueue_::stop(){
        stopThread();
    }
    
    void NiteQueue_::clear(){
        trackers.clear();
    }
    
    void NiteQueue_::threadedFunction(){
        while( isThreadRunning() ){
            for (int i=0; i<trackers.size(); i++){
                trackers[i]->process();
                sleep(1);
            }
        }
    }
};

