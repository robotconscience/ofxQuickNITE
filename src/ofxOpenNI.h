//
//  ofxQuickNite
//
//  Created by Brett Renfer on 4/7/13.
//

#pragma once

#include <OpenNI.h>

using namespace openni;

namespace ofxOpenNI {
    static bool openNIInitialized = false;
        
    Status initialize(){
        if ( openNIInitialized ){
            return STATUS_OK;
        }
        
        Status status = OpenNI::initialize();
        status = OpenNI::initialize();
        if (status == STATUS_OK)
        {
            ofLogVerbose() << "initialize PASS";
            openNIInitialized = true;
            return status;
        } else
        {
            ofLog(OF_LOG_ERROR, "initialize FAIL:\n%s\n", OpenNI::getExtendedError());
            return status;
        }
    };

    void shutdown(){
        if ( !openNIInitialized ){
            return;
        }
        
        openni::OpenNI::shutdown();
        openNIInitialized = false;
    }
}