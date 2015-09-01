#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
    isFirstImg = true;
    
    // setup IP cam
    loadCamera();
    IPCameraDef& cam = getCamera();
    SharedIPVideoGrabber c = IPVideoGrabber::makeShared();
    c->setCameraName(cam.getName());
    c->setURI(cam.getURL());
    c->connect();
    grabber = c;
}

void ofApp::update() {
	grabber->update();
	if(grabber->isFrameNew()) {
        img = *grabber->getFrame();
        
        int w = grabber->getHeight();
        int h = grabber->getHeight();
        img.crop((img.width-w)/2, 0, w, h);
        src = toCv(img);
        
        if (isFirstImg) {
            dst.create(src.size(), src.type());
            buildMap(w, h, w, h, 180.0, 180.0);
            imitate(undistorted, img);
            imitate(previous, img);
            imitate(diff, img);
            isFirstImg = false;
        }
        
        //INTER_NEAREST=CV_INTER_NN, //!< nearest neighbor interpolation
        //INTER_LINEAR=CV_INTER_LINEAR, //!< bilinear interpolation
        //INTER_CUBIC=CV_INTER_CUBIC, //!< bicubic interpolation
        //INTER_AREA=CV_INTER_AREA, //!< area-based (or super) interpolation
        //INTER_LANCZOS4=CV_INTER_LANCZOS4, //!< Lanczos interpolation over 8x8 neighborhood
        //INTER_MAX=7,
        //WARP_INVERSE_MAP=CV_WARP_INVERSE_MAP
        cv::remap(src, dst, remapX, remapY, INTER_LINEAR);
        toOf(dst, imgOut);
        
	}
}

void ofApp::draw() {
	ofSetColor(255);
    //grabber->draw(0, 0);
    img.draw(0, 0);
    if (ofGetKeyPressed('d')) drawMat(dst, 0, 0);
    
}


// populates two maps which are used by cv::remap to convert from a circular coordinate system
// Based on http://paulbourke.net/dome/fish2/
// and http://www.kscottz.com/fish-eye-lens-dewarping-and-panorama-stiching/
//
void ofApp::buildMap(float Ws,float Hs,float Wd,float Hd,float hfovd,float vfovd) {
    // Build the fisheye mapping
    remapX.create( Hd, Wd, CV_32FC1 );
    remapY.create( Hd, Wd, CV_32FC1 );
    float vfov = (vfovd/180.0)*pi;
    float hfov = (hfovd/180.0)*pi;
    float vstart = ((180.0-vfovd)/180.00)*pi/2.0;
    float hstart = ((180.0-hfovd)/180.00)*pi/2.0;
    float count = 0;
    // scale to changed range from smaller cirlce traced by the fov
    float xmax = sin(pi/2.0)*cos(vstart);
    float xmin = sin(pi/2.0)*cos(vstart+vfov);
    float xscale = xmax-xmin;
    float xoff = xscale/2.0;
    float zmax = cos(hstart);
    float zmin = cos(hfov+hstart);
    float zscale = zmax-zmin;
    float zoff = zscale/2.0;
    // Fill in the map
    for (int y=0; y<Hd; y++){
        for (int x=0; x<Wd; x++){
            float count = count + 1;
            float phi = vstart+(vfov*((float(x)/float(Wd))));
            float theta = hstart+(hfov*((float(y)/float(Hd))));
            float xp = ((sin(theta)*cos(phi))+xoff)/zscale;
            float zp = ((cos(theta))+zoff)/zscale;
            float xS = Ws-(xp*Ws);
            float yS = Hs-(zp*Hs);
            remapX.at<float>(y,x) = xS;
            remapY.at<float>(y,x) = yS;
        }
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
}
