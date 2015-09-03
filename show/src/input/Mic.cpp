#include "Mic.h"

void Mic::setup(){
	
    fftLive.setMirrorData(false);
    fftLive.setup();
    
    fftFile.setMirrorData(false);
    fftFile.setup();
    
    //soundPlayer.loadSound("sound/test.mp3");
    //soundPlayer.setLoop(true);
    //soundPlayer.play();
    
}


void Mic::update(){
    fftLive.update();
    fftFile.update();
}


void Mic::draw(){
    // debug only
    ofSetColor(255);
    ofDrawBitmapString("AUDIO FROM MIC", 10, 20);
    ofDrawBitmapString("AUDIO FROM FILE", 10, 310);
    fftLive.draw(10, 30);
    fftFile.draw(10, 320);
}

void Mic::start(){
    fftLive.soundStream->start();
}
void Mic::stop(){
    fftLive.soundStream->stop();
}


void Mic::keyPressed  (int key){
	//if( key == 's' ){
    //    stop();
	//}
	
	//if( key == 'S' ){
    //    start();
	//}
}


void Mic::keyReleased(int key){
	
}


void Mic::mouseMoved(int x, int y ){
	
}


void Mic::mouseDragged(int x, int y, int button){
	
}


void Mic::mousePressed(int x, int y, int button){
	
}


void Mic::mouseReleased(int x, int y, int button){
    
}


void Mic::windowResized(int w, int h){
    
}


void Mic::gotMessage(ofMessage msg){
    
}


void Mic::dragEvent(ofDragInfo dragInfo){ 
    
}