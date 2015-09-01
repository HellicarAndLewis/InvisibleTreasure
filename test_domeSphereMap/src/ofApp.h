// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include "ofMain.h"
#include "IPVideoGrabber.h"

#define NUM_CAMERAS 1
#define NUM_ROWS 1
#define NUM_COLS 1
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


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    ofVec2f fishToSphere(ofVec2f destCoord);

    SharedIPVideoGrabber grabber;

    void loadCamera();
    IPCameraDef& getCamera();
    IPCameraDef ipcam; // a list of IPCameras

    // This message occurs when the incoming video stream image size changes. 
    // This can happen if the IPCamera has a single broadcast state (some cheaper IPCams do this)
    // and that broadcast size is changed by another user. 
    void videoResized(const void* sender, ofResizeEventArgs& arg);
    
};
