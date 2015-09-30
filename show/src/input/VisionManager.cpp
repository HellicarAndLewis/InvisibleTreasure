#include "VisionManager.h"
using namespace ofxCv;
using namespace cv;

VisionManager::VisionManager() {
    input = NULL;
    inputIndex = -1;
}

void VisionManager::setup() {
    // inputs
    // 0: Main/Dome IP cam
    ipcam.setup();
    inputs.push_back(&ipcam);
    // 1: Cassandra IP cam
    ipcamCassandra.setup();
    inputs.push_back(&ipcamCassandra);
    // 2: iSight/webcam/ofVideoGrabber
    inputs.push_back(&grabber);
    // 3: video file
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
}

void VisionManager::update() {
    if (input == NULL || !isEnabled) return;
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
    float w = MAX(ofGetWidth()/4, 256);
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

void VisionManager::setToIPCamMain() {
    inputSelector = 0;
}

void VisionManager::setToIPCamCassandra() {
    inputSelector = 1;
    
}

ContourTracker* VisionManager::getTracker(){
    return &contourTracker;
}

IVisionInput* VisionManager::getInput(){
    return input;
}

void VisionManager::setupGui() {
    // contour tracker gui first
    contourTracker.setupGui();
    // vision next
    guiName = "Vision";
    panel.setup(guiName, "settings/vision.xml");
    panel.add(isEnabled.set("enabled", false));
    panel.add(inputSelector.set("input", 0, 0, inputs.size()-1));
    panel.add(debugDraw.set("debug draw", false));
    panel.add(isCalibrating.set("calibrating", false));
    panel.add(ipCamURLMain.set("Main cam URL", "http://10.0.0.50/axis-cgi/mjpg/video.cgi"));
    panel.add(ipCamURLCassandra.set("Cassandra cam URL", "http://10.0.0.51/axis-cgi/mjpg/video.cgi"));
    panel.add(contourTracker.parameters);
    panel.loadFromFile("settings/vision.xml");
    
    // load camera URLs with XML settings
    ipcam.load(ipCamURLMain);
    ipcamCassandra.load(ipCamURLCassandra);
    
    // set the input
    int input = inputSelector.get();
    onInputChange(input);
    
    // Add a listener to change inputs using the inputSelector slider
    inputSelector.addListener(this, &VisionManager::onInputChange);
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
