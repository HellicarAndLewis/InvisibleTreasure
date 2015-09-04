#include "ofApp.h"
#include "ofxArgs.h"
#include "ofAppGLFWWindow.h"

int main(int argc, char *argv[]) {
    // read in args
    ofxArgs* args = new ofxArgs(argc, argv);
    bool fullScreen = args->getBool("-fullScreen", false);
    int width = args->getInt("-width", 1024);
    int height = args->getInt("-height", 768);
    
    // setup window
    ofAppGLFWWindow window;
    window.setMultiDisplayFullscreen(true);
    if (fullScreen) ofSetupOpenGL(&window, width, height, OF_FULLSCREEN);
    else ofSetupOpenGL(&window, width, height, OF_WINDOW);
    
	ofRunApp(new ofApp(args));
}
