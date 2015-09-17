#include "ofApp.h"

ofApp::ofApp(ofxArgs* args) {
    // handle args passed in
    args->printArgs();
    args->printOpts();
    
    // logging
    // log level is enum:
    // OF_LOG_VERBOSE,OF_LOG_NOTICE,OF_LOG_WARNING,OF_LOG_ERROR,OF_LOG_FATAL_ERROR,OF_LOG_SILENT
    bool logToFile  = args->getBool("-logToFile", false);
    bool appendToLog  = args->getBool("-appendToLog", false);
    int logLevel = args->getInt("-logLevel", 1);
    ofSetLogLevel((ofLogLevel)logLevel);
    string toFileString = (logToFile) ? "log to file" : "log to cout";
    ofLogNotice() << "in ofApp::ofApp Setting log level " << logLevel << ", " << toFileString << "\n\n";
    if (logToFile) {
        string format = "log-%Y-%m";
        string timestamp = ofGetTimestampString(format);
        string filename = "logs/" + timestamp + ".txt";
        ofLogToFile(filename, appendToLog);
    }
    
    // TODO: store id and do something with it?
    // Numerical ID, will probabl come in handy at some point
    int id = args->getInt("-windowId", 0);
    appModel.windowId = id;
    
    // Mode is a string that can be "MASTER", "WINDOW", "SLAVE"
    // each string is matched to an enum in AppModel
    string modeString = ofToUpper(args->getString("-mode", "MASTER"));
    appModel.setMode(modeString);
}

void ofApp::setup() {
    ofBackground(0);
    osc.setup();
    vision.setup();
    sceneManager.setup(&appModel, &osc, &vision);
    // call this last!
    setupGui();
}

void ofApp::update() {
    osc.update();
    vision.update();
    sceneManager.update();
}

void ofApp::draw() {
    
    if (vision.debugDraw) vision.draw();
    sceneManager.draw();
    
    // debug draw
    if (debug) {
        osc.draw();
        string s = appModel.modeString;
        s += "\n" + ofToString(ofGetFrameRate());
        ofDrawBitmapStringHighlight(s, 10, ofGetHeight() - 20);
    }
    
    // GUI panels
    drawGui();
    
}

void ofApp::setupGui() {
    // setup panels
    guiables.push_back(&osc);
    guiables.push_back(&sceneManager);
    guiables.push_back(&vision);
    
    // global panel
    ofxGuiSetFont("fonts/Andale Mono.ttf", 12);
    panel.setDefaultWidth(250);
    panel.setup("Show", "settings/global.xml");
    panel.add(debug.set("debug", true));
    parameters.setName("More GUIs");
    for (auto guiable: guiables) {
        guiable->setupGui();
        parameters.add(guiable->guiEnabled.set(guiable->guiName, false));
        guiable->panel.setPosition(270, 10);
    }
    panel.add(parameters);
    
    // Setup and load GUI
    panel.loadFromFile("settings/global.xml");
    
    // setup the RemoteUIServer
    //RUI_SETUP();
    //ruiBridge.setup(parameters);
    
    
    if (appModel.mode == AppModel::MASTER) guiEnabled = true;
}

void ofApp::drawGui() {
    if (guiEnabled) {
        GuiableBase::drawGui();
        for (auto guiable: guiables) guiable->drawGui();
    }
}

void ofApp::keyPressed (int key) {
    osc.keyPressed(key);
    vision.keyPressed(key);
    sceneManager.keyPressed(key);
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'm':
            appModel.setMode("MASTER");
            break;
        case 'w':
            appModel.setMode("WINDOW");
            break;
        case 's':
            appModel.setMode("SLAVE");
            break;
        case ' ':
            guiEnabled = !guiEnabled;
            if (guiEnabled) ofShowCursor();
            else ofHideCursor();
            break;
    }
}

void ofApp::keyReleased (int key) {}

void ofApp::mouseMoved(int x, int y) {}

void ofApp::mouseDragged(int x, int y, int button) {}

void ofApp::mousePressed(int x, int y, int button) {}

void ofApp::mouseReleased(int x, int y, int button) {}

void ofApp::windowResized(int w, int h) {}