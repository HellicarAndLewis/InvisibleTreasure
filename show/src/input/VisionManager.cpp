#include "VisionManager.h"
using namespace ofxCv;
using namespace cv;

VisionManager::VisionManager() {
    input = NULL;
    inputIndex = -1;
}

void VisionManager::setup() {
    // inputs
    ipcam.setup();
    inputs.push_back(&ipcam);
    video.setup();
    inputs.push_back(&video);
    
    // tracker
    contourTracker.setup();
    
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
    
    // gui
    inputSelector.addListener(this, &VisionManager::onInputChange);
    inputSelector.set("input", 0, 0, inputs.size()-1);
    debugDraw.set("debug draw", false);
    isCalibrating.set("calibrating", false);
}

void VisionManager::update() {
    if (input == NULL) return;
    input->update();
    if (input->getIsReady() && input->isFrameNew()) {
        inputImage.setFromPixels(input->getPixelsRef());
        if (isFirstImage) {
            isFirstImage = false;
            imitate(outputImage, inputImage);
            imitate(previous, inputImage);
            imitate(diff, inputImage);
            contourTracker.resetBg();
        }
        if (isCalibrating) {
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
        }
        
        // undistort input into the output image
        calibration.undistort(toCv(inputImage), toCv(outputImage));
        outputImage.update();
        
        // TODO: use outputImage with contour tracker
        contourTracker.image = &inputImage;
        contourTracker.update();
    }
}

void VisionManager::draw() {
    ofSetColor(255);
    float w = ofGetWidth()/2;
    float h = inputImage.getHeight() * (w / inputImage.getWidth());
    inputImage.draw(0, 0, w, h);
    outputImage.draw(w, 0, w, h);
    
    if (isCalibrating) {
        stringstream intrinsics;
        intrinsics << "fov: " << toOf(calibration.getDistortedIntrinsics().getFov()) << " distCoeffs: " << calibration.getDistCoeffs();
        drawHighlightString(intrinsics.str(), 10, 20, yellowPrint, ofColor(0));
        drawHighlightString("movement: " + ofToString(diffMean), 10, 40, cyanPrint);
        drawHighlightString("reproj error: " + ofToString(calibration.getReprojectionError()) + " from " + ofToString(calibration.size()), 10, 60, magentaPrint);
        for(int i = 0; i < calibration.size(); i++) {
            //drawHighlightString(ofToString(i) + ": " + ofToString(calibration.getReprojectionError(i)), 10, 80 + 16 * i, magentaPrint);
        }
    }
    
    // draw contour tracking
    ofPushMatrix();
    {
        ofTranslate(0, h);
        float scale = h/contourTracker.thresholded.height;
        ofScale(scale, scale);
        contourTracker.draw();
    }
    ofPopMatrix();
}

void VisionManager::exit() {
}

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

ContourTracker* VisionManager::getTracker(){
    return &contourTracker;
}

void VisionManager::setupGui() {
    // child guis
    contourTracker.setupGui();
    
    guiName = "Vision";
    panel.setup(guiName, "settings/vision.xml");
    panel.add(inputSelector);
    panel.add(debugDraw);
    panel.add(isCalibrating);
    panel.add(camURL.set("IP Cam URL", "http://169.254.37.207/axis-cgi/mjpg/video.cgi"));
    panel.add(contourTracker.parameters);
    panel.loadFromFile("settings/vision.xml");
    
    // TODO: add stop/start to GUI
    // TODO: only start by default if master?
    ipcam.load(camURL);
}

void VisionManager::drawGui() {
    GuiableBase::drawGui();
}
//////////////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// custom event handlers
//////////////////////////////////////////////////////////////////////////////////
void VisionManager::onInputChange(int & i) {
    if (i >= 0 && i < inputs.size() && i != inputIndex) {
        ofLogNotice() << "VisionManager::onInputChange to " << i;
        if (input != NULL) input->stop();
        inputIndex = i;
        isFirstImage = true;
        input = inputs[i];
        input->start();
    }
    
}
//////////////////////////////////////////////////////////////////////////////////
// oF event handlers
//////////////////////////////////////////////////////////////////////////////////

void VisionManager::keyPressed (int key) {
    if(key == 'c') {
        calibration.reset();
    }
    if (key == 'b') {
        contourTracker.resetBg();
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
