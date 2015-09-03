#include "VisionManager.h"
using namespace ofxCv;
using namespace cv;

VisionManager::VisionManager() {
}

void VisionManager::setup() {
    // inputs
    inputIndex = 0;
    ipcam.setup();
    inputs.push_back(&ipcam);
    video.setup();
    inputs.push_back(&video);
    input = inputs[inputIndex];
    input->start();
    
    // calibration
    FileStorage settings(ofToDataPath("camera/settings.yml"), FileStorage::READ);
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
    calibration.load("camera/dome.yml");
    isFirstImage = true;
}

void VisionManager::update() {
    input->update();
    if (input->getIsReady() && input->isFrameNew()) {
        inputImage.setFromPixels(input->getPixelsRef());
        if (isFirstImage) {
            isFirstImage = false;
            imitate(outputImage, inputImage);
            imitate(previous, inputImage);
            imitate(diff, inputImage);
        }
        
        Mat camMat = toCv(inputImage);
        Mat prevMat = toCv(previous);
        Mat diffMat = toCv(diff);
        
        absdiff(prevMat, camMat, diffMat);
        camMat.copyTo(prevMat);
        
        diffMean = mean(Mat(mean(diffMat)))[0];
        
        float curTime = ofGetElapsedTimef();
        if(ofGetKeyPressed('a')) {
            if(calibration.add(camMat)) {
                cout << "re-calibrating" << endl;
                calibration.calibrate();
                if(calibration.size() > 10) {
                    calibration.clean();
                }
                calibration.save("camera/calibration.yml");
                lastTime = curTime;
            }
        }
        
        calibration.undistort(toCv(inputImage), toCv(outputImage));
        outputImage.update();
    }
}

void VisionManager::draw() {
    ofSetColor(255);
    float w = ofGetWidth()/2;
    float h = inputImage.getHeight() * (w / inputImage.getWidth());
    inputImage.draw(0, 0, w, h);
    outputImage.draw(w, 0, w, h);
    
    stringstream intrinsics;
    intrinsics << "fov: " << toOf(calibration.getDistortedIntrinsics().getFov()) << " distCoeffs: " << calibration.getDistCoeffs();
    drawHighlightString(intrinsics.str(), 10, 20, yellowPrint, ofColor(0));
    drawHighlightString("movement: " + ofToString(diffMean), 10, 40, cyanPrint);
    drawHighlightString("reproj error: " + ofToString(calibration.getReprojectionError()) + " from " + ofToString(calibration.size()), 10, 60, magentaPrint);
    for(int i = 0; i < calibration.size(); i++) {
        //drawHighlightString(ofToString(i) + ": " + ofToString(calibration.getReprojectionError(i)), 10, 80 + 16 * i, magentaPrint);
    }
}

void VisionManager::exit() {
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void VisionManager::keyPressed (int key) {
    if(key == 'c') {
        calibration.reset();
    }
    if (key == OF_KEY_UP) {
        input->stop();
        if (++inputIndex > inputs.size()-1) inputIndex = 0;
        isFirstImage = true;
        input = inputs[inputIndex];
        input->start();
    }
}

void VisionManager::keyReleased (int key) {}

void VisionManager::mouseMoved(int x, int y) {}

void VisionManager::mouseDragged(int x, int y, int button) {}

void VisionManager::mousePressed(int x, int y, int button) {}

void VisionManager::mouseReleased(int x, int y, int button) {}

void VisionManager::windowResized(int w, int h) {}

void VisionManager::dragEvent(ofDragInfo dragInfo) {}

void VisionManager::gotMessage(ofMessage msg) {}
