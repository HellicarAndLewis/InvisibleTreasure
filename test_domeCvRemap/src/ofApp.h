#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "IPVideoGrabber.h"

#define CAM_URL "http://169.254.37.207/axis-cgi/mjpg/video.cgi"

class IPCameraDef
{
public:
    IPCameraDef()
    {
    }
    
    IPCameraDef(const std::string& url): _url(url)
    {
    }
    
    IPCameraDef(const std::string& name,
                const std::string& url,
                const std::string& username,
                const std::string& password):
    _name(name),
    _url(url),
    _username(username),
    _password(password)
    {
    }
    
    void setName(const std::string& name) { _name = name; }
    std::string getName() const { return _name; }
    
    void setURL(const std::string& url) { _url = url; }
    std::string getURL() const { return _url; }
    
    void setUsername(const std::string& username) { _username = username; }
    std::string getUsername() const { return _username; }
    
    void setPassword(const std::string& password) { _password = password; }
    std::string getPassword() const { return _password; }
    
    
private:
    std::string _name;
    std::string _url;
    std::string _username;
    std::string _password;
};
using ofx::Video::IPVideoGrabber;
using ofx::Video::SharedIPVideoGrabber;

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    ofVideoGrabber videoGrabber;
    
	ofImage undistorted;
	ofPixels previous;
	ofPixels diff;
	float diffMean;
	float lastTime;
	bool active;
    
    cv::Mat remapX, remapY;
    cv::Mat dst, src;
    void buildMap(float Ws,float Hs,float Wd,float Hd,float hfovd=160.0,float vfovd=160.0);
    
    // IP Cam
    ofImage img;
    ofImage imgOut;
    SharedIPVideoGrabber grabber;
    void loadCamera();
    IPCameraDef& getCamera();
    IPCameraDef ipcam;
    bool isFirstImg;
};
