#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "192.168.1.128"
#define PORT 8001

//--------------------------------------------------------
class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont font;
		ofxOscSender sender;
        ofBuffer imgAsBuffer;
        ofImage img; 
};

