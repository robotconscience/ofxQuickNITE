ofxQuickNITE
============

Very rough + nasty wrapper for openNI 2.1 + NITE. Use at your own risk... 

Setup
============
* Clone into openFrameworks/addons
* Open the example and run!

Creating a new project
============

OS X
-------------
* Create a new project with the projectGenerator, selecting "ofxQuickNITE" as one of your addons
* Open your new project and create a new group called "Resources"
* Add the openNI + NiTE dylibs to this group
  * ofxQuickNITE/libs/nite/lib/osx/libNiTE2.dylib
  * ofxQuickNITE/libs/openni/lib/osx/libOpenNI2.dylib
* Add the openNI drivers and initialization file to this group
  * ofxQuickNITE/libs/openni/drivers/OpenNI2
    * When you add this folder, make sure "Create folder references for any added folders" is selected in the Xcode dialog
  * ofxQuickNITE/libs/openni/drivers/OpenNI.ini
* Copy these resoures into your app on compile:
  * Go to the "Build Phases" menu in your target menu
  * Click the "Add Build Phase" icon on the bottom right; select "Add Copy Files"
  * In the new box that appears, select "Executables" from the drop down
  * Add all of the files and folders from your Resources group to this panel
  * Make sure OpenNI2 (the drivers folder) shows up as a folder!
* That's it! Xcode should now copy all of the dylibs into your app when you compile.
