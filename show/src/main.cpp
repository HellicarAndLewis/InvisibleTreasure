#include "ofApp.h"
#include "ofxArgs.h"
#include "ofAppGLFWWindow.h"

int main(int argc, char *argv[]) {
    // read in args
    ofxArgs* args = new ofxArgs(argc, argv);
    bool fullScreen = args->getBool("-fullScreen", false);
    
    // setup window
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    if (fullScreen) ofSetupOpenGL(&window, 640, 480, OF_FULLSCREEN);
    else ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
    
	ofRunApp(new ofApp(args));
}
