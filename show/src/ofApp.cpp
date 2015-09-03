#include "ofApp.h"

ofApp::ofApp(ofxArgs* args) {
    // handle args passed in
    args->printArgs();
    args->printOpts();
    bool logToFile  = args->getInt("-logToFile", false);
    bool appendToLog  = args->getInt("-appendToLog", false);
    int logLevel = args->getInt("-logLevel", 1); //OF_LOG_VERBOSE,OF_LOG_NOTICE,OF_LOG_WARNING,OF_LOG_ERROR,OF_LOG_FATAL_ERROR,OF_LOG_SILENT
    
    // TODO: store id and mode and do something with them
    // Numerical ID, will probabl come in handy at some point
    int id = args->getInt("-id", 0);
    
    // Mode can be "MASTER", "WINDOW", "SLAVE"
    string modeString = ofToUpper(args->getString("-mode", "MASTER"));
    appModel.setMode(modeString);
    
    // logging
    ofSetLogLevel((ofLogLevel)logLevel);
    string toFileString = (logToFile) ? "log to file" : "log to cout";
    ofLogNotice() << "Setting log level " << logLevel << ", " << toFileString;
    if (logToFile) {
        string format = "log-%Y-%m";
        string timestamp = ofGetTimestampString(format);
        string filename = "logs/" + timestamp + ".txt";
        ofLogToFile(filename, appendToLog);
    }
}

void ofApp::setup() {
    ofBackground(0);
    osc.setup();
    vision.setup();
    sceneManager.setup(&appModel, &osc, &vision);
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
    // OSC
    guiables.push_back(&osc);
    // scene manager
    guiables.push_back(&sceneManager);
    // vision
    guiables.push_back(&vision);
    
    // global panel
    panel.setDefaultWidth(250);
    parameters.setName("Global");
    parameters.add(debug.set("debug", true));
    for (auto guiable: guiables) {
        guiable->setupGui();
        parameters.add(guiable->guiEnabled.set(guiable->guiName, false));
        guiable->panel.setPosition(270, 10);
    }
    
    // Setup and load GUI
    panel.setup(parameters, "settings/global.xml");
    panel.loadFromFile("settings/global.xml");
    
    // setup the RemoteUIServer
    RUI_SETUP();
    ruiBridge.setup(parameters);
    
    
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