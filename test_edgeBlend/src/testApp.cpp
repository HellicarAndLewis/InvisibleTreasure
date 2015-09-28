#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    blender[0].setup(640, 480, 2, 20, ofxProjectorBlend_Horizontal);
    blender[0].setWindowToDisplaySize();
    
    blender[1].setup(640, 480, 2, 20, ofxProjectorBlend_Horizontal);
    blender[1].setWindowToDisplaySize();
    
    blender[2].setup(1280, 480, 2, 20, ofxProjectorBlend_Vertical);
    blender[2].setWindowToDisplaySize();
    
    radius = 40;
    pos.x = ofRandom(radius, blender[0].getCanvasWidth()-radius);
    pos.y = ofRandom(radius, blender[0].getCanvasHeight()-radius);
    vel.set(10, 10);
}

//--------------------------------------------------------------
void testApp::update(){

    pos += vel;
    if(pos.x > blender[0].getCanvasWidth()-radius) {
        pos.x = blender[0].getCanvasWidth()-radius;
        vel.x *= -1;
    }
    if(pos.x < radius) {
        pos.x = radius;
        vel.x *= -1;
    }
    if(pos.y > blender[0].getCanvasHeight()-radius) {
        pos.y = blender[0].getCanvasHeight()-radius;
        vel.y *= -1;
    }
    if(pos.y < radius) {
        pos.y = radius;
        vel.y *= -1;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    for (int i=0; i<2; i++) {
	blender[i].begin(); //call blender[i].begin() to draw onto the blendable canvas
    {
        //light gray backaground
        ofSetColor(100, 100, 100);
        ofRect(0, 0, blender[i].getCanvasWidth(), blender[i].getCanvasHeight());
        
        //thick grid lines for blending
        ofSetColor(255, 255, 255);
        ofSetLineWidth(3);
        
        //vertical line
        for(int i = 0; i <= blender[i].getCanvasWidth(); i+=40){
            ofLine(i, 0, i, blender[i].getCanvasHeight());
        }
        
        //horizontal lines
        for(int j = 0; j <= blender[i].getCanvasHeight(); j+=40){
            ofLine(0, j, blender[i].getCanvasWidth(), j);
        }
        
        ofSetColor(255, 0, 0);
        ofCircle(pos, radius);
        
        //instructions
        ofSetColor(255, 255, 255);
        ofRect(10, 10, 300, 100);
        ofSetColor(0, 0, 0);
        ofDrawBitmapString("SPACE - toggle show blend\n[g/G] - adjust gamma\n[p/P] - adjust blend power\n[l/L] adjust luminance", 15, 35);
	}
	blender[i].end(); //call when you are finished drawing
    }
    
	//this draws to the main window
    blender[2].begin();
    blender[0].draw();
    ofPushMatrix();
    ofTranslate(0, 480);
    blender[1].draw();
    ofPopMatrix();
    blender[2].end();
    blender[2].draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	//hit spacebar to toggle the blending strip
	if(key == ' '){
		//toggling this variable effects whether the blend strip is shown
		blender[0].showBlend = !blender[0].showBlend;
	}
	
	// more info here on what these variables do
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
	
	else if(key == 'g'){
		blender[0].gamma[0]  -= .05;
		blender[0].gamma[1] -= .05;
	}
	else if(key == 'G'){
		blender[0].gamma[0]  += .05;
		blender[0].gamma[1] += .05;
	}
	else if(key == 'l'){
		blender[0].luminance[0]  -= .05;
		blender[0].luminance[1] -= .05;
	}
	else if(key == 'L'){
		blender[0].luminance[0]  += .05;
		blender[0].luminance[1] += .05;
	}
	else if(key == 'p'){
		blender[0].blendPower[0]  -= .05;
		blender[0].blendPower[1] -= .05;
	}
	else if(key == 'P'){
		blender[0].blendPower[0]  += .05;
		blender[0].blendPower[1] += .05;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}