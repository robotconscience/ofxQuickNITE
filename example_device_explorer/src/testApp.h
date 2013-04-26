#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        vector<ofxOpenNI2Grabber * > grabbers;
        vector<openni::DeviceInfo> devices;

        float textFade;
        string clipboard;

        // sweet little (OS X-only) snippet
        static OSStatus setTextToPasteboard(char* byteArrayIndex) {
            
            OSStatus err = noErr;
            static PasteboardRef    pasteboard = NULL;
            PasteboardCreate( kPasteboardClipboard, &pasteboard );
            
            err = PasteboardClear( pasteboard );
            require_noerr( err, PasteboardClear_FAILED );
            
            CFDataRef data;
            
            data = CFDataCreate(kCFAllocatorDefault, (UInt8*) byteArrayIndex, strlen(byteArrayIndex)+1);
            err = PasteboardPutItemFlavor( pasteboard, (PasteboardItemID)1, kUTTypeUTF8PlainText, data, 0);
            require_noerr( err, PasteboardPutItemFlavor_FAILED );
            
        PasteboardPutItemFlavor_FAILED:
        PasteboardClear_FAILED:
            return err;
        }

};
