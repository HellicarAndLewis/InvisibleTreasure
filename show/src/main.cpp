#include "ofApp.h"
#include "ofxArgs.h"

int main(int argc, char *argv[]) {
    // read in args
    ofxArgs* args = new ofxArgs(argc, argv);
    bool fullScreen = args->getBool("-fullScreen", false);
    
    // setup window
    if (fullScreen) ofSetupOpenGL(640, 480, OF_FULLSCREEN);
    else ofSetupOpenGL(640, 480, OF_WINDOW);
    
	ofRunApp(new ofApp(args));
}
