#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

const float diffThreshold = 2.5; // maximum amount of movement
const float timeThreshold = 1; // minimum time between snapshots
const int startCleaning = 10; // start cleaning outliers after this many samples


//    0 either load() from a yml file (skip to 5),
//    or do the calibration as follows
//    1 set the board and physical square size of the chess board. whatever
//    if your squares are in mm, your focal length will also be in mm.
//    2 add() each image containing a chess board
//    3 when all the images are added, call calibrate()
//    4 now you can save() a yml calibration file
//    5 now you can undistort() incoming images.

void ofApp::setup() {
	ofSetVerticalSync(true);
    isFirstImg = true;
    
    //videoGrabber.initGrabber(640, 480);
	
    // setup IP cam
    loadCamera();
    IPCameraDef& cam = getCamera();
    SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
    c->setCameraName(cam.getName());
    c->setURI(cam.getURL());
    c->connect();
    grabber = c;
    
    // calibration
	FileStorage settings(ofToDataPath("settings.yml"), FileStorage::READ);
	if(settings.isOpened()) {
		int xCount = settings["xCount"], yCount = settings["yCount"];
		calibration.setPatternSize(xCount, yCount);
		float squareSize = settings["squareSize"];
		calibration.setSquareSize(squareSize);
		CalibrationPattern patternType;
		switch(settings["patternType"]) {
			case 0: patternType = CHESSBOARD; break;
			case 1: patternType = CIRCLES_GRID; break;
			case 2: patternType = ASYMMETRIC_CIRCLES_GRID; break;
		}
		calibration.setPatternType(patternType);
    }
    calibration.setFillFrame(false);
    
    calibration.load("dome.yml");
	
	lastTime = 0;
	active = true;
}

void ofApp::update() {
	grabber->update();
	if(grabber->isFrameNew()) {
        img = *grabber->getFrame();
        
        if (isFirstImg) {
            imitate(undistorted, img);
            imitate(previous, img);
            imitate(diff, img);
            isFirstImg = false;
        }
        
        Mat camMat = toCv(img);
		Mat prevMat = toCv(previous);
		Mat diffMat = toCv(diff);
		
		absdiff(prevMat, camMat, diffMat);	
		camMat.copyTo(prevMat);
		
		diffMean = mean(Mat(mean(diffMat)))[0];
		
		float curTime = ofGetElapsedTimef();
		if(ofGetKeyPressed(' ')) {
			if(calibration.add(camMat)) {
				cout << "re-calibrating" << endl;
				calibration.calibrate();
				if(calibration.size() > startCleaning) {
					calibration.clean();
				}
				calibration.save("calibration.yml");
				lastTime = curTime;
			}
		}
		
		if(calibration.size() > 0) {
			calibration.undistort(toCv(img), toCv(undistorted));
			undistorted.update();
		}
	}
}

void ofApp::draw() {
	ofSetColor(255);
    float w = ofGetWidth()/2;
    float h = grabber->getHeight() * (w / grabber->getWidth());
    grabber->draw(0, 0, w, h);
	undistorted.draw(w, 0, w, h);
    
	stringstream intrinsics;
	intrinsics << "fov: " << toOf(calibration.getDistortedIntrinsics().getFov()) << " distCoeffs: " << calibration.getDistCoeffs();
	drawHighlightString(intrinsics.str(), 10, 20, yellowPrint, ofColor(0));
	drawHighlightString("movement: " + ofToString(diffMean), 10, 40, cyanPrint);
	drawHighlightString("reproj error: " + ofToString(calibration.getReprojectionError()) + " from " + ofToString(calibration.size()), 10, 60, magentaPrint);
	for(int i = 0; i < calibration.size(); i++) {
		//drawHighlightString(ofToString(i) + ": " + ofToString(calibration.getReprojectionError(i)), 10, 80 + 16 * i, magentaPrint);
	}
}

//------------------------------------------------------------------------------
IPCameraDef& ofApp::getCamera() {
    return ipcam;
}

//------------------------------------------------------------------------------
void ofApp::loadCamera() {
    ipcam = IPCameraDef("Dome", CAM_URL, "", "");
}

void ofApp::keyPressed(int key) {
	if(key == 'a') {
		active = !active;
	}
    else if(key == 'c') {
        calibration.reset();
    }
}
