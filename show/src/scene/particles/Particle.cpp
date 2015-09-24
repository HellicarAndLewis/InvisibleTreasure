#include "Particle.h"


Particle::Particle(){
	attractPoints = NULL;
    minUnique = -10000;
    maxUnique = 1000;
    minDrag = 0.82;
    maxDrag = 0.95;
    flock = false;
    mode = EAT_GREEN;
    color.set(255,234,119);
    isFull = false;
}


void Particle::setMode(Mode newMode){
	mode = newMode;
}


void Particle::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}


void Particle::reset(){
    reset(ofRectangle(0, 0, 100, 100));
}


void Particle::reset(ofRectangle bounds) {
    float maxVel = 0.5;
    uniqueVal = ofRandom(minUnique, maxUnique);
    pos.x = ofRandom(bounds.getLeft(), bounds.getRight());
    pos.y = ofRandom(bounds.getTop(), bounds.getBottom());
    vel.x = ofRandom(-maxVel, maxVel);
    vel.y = ofRandom(-maxVel, maxVel);
    frc   = ofPoint(0,0,0);
    setScale(ofRandom(0.5, 1.0));
    isFull = false;
}


void Particle::update(){
    
    if (mode == EAT_NOTHING || mode == EAT_GROW) {
        color.lerp( ofColor(255,234,119), 0.4 );
    }
    
    if( attractPoints ){
        // closest attractPoint
        ofPoint closestPt;
        int closest = -1;
        float closestDist = 9999999;
        for(unsigned int i = 0; i < attractPoints->size(); i++){
            float lenSq = ( attractPoints->at(i)-pos ).lengthSquared();
            if( lenSq < closestDist ){
                closestDist = lenSq;
                closest = i;
            }
        }
        // attract
        if( closest != -1 && mode != EAT_NOTHING){
            closestPt = attractPoints->at(closest);
            float dist = sqrt(closestDist);
            // force proportional to distance
            frc = closestPt - pos;
            vel *= drag;
            if( dist > 40 && dist < 500 && !ofGetKeyPressed('l')){
                frc *= 0.003;
                frc.x += ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
                frc.y += ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
                vel += frc;
            } else {
                if (dist < 40) {
                    eat();
                }
                frc.x = ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
                frc.y = ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
                vel += frc * 0.4;
            }
            
        }
        else {
            vel *= drag;
            frc.x = ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
            frc.y = ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
            vel += frc * 0.4;
        }
        
    }
	
    // update pos using velocity
	pos += vel;
    
    
}

void Particle::eat() {
    if (mode == EAT_GREEN) {
        color.lerp( ofColor(67,224,109), 0.9 );
        isFull = true;
    }
    else if (mode == EAT_GROW) {
        setScale(scale + 0.05);
        if (scale >= 2) {
            isFull = true;
        }
    }
}


void Particle::setScale(float scale) {
    this->scale = ofClamp(scale, 0, 2);
    drag = ofMap(this->scale, 2, 0.5, minDrag, maxDrag);
}

void Particle::setNeighbours(vector <Particle> * neighbours){
    this->neighbours=neighbours;
}


void Particle::draw(){
    ofSetColor(color.r,color.g,color.b, 255);
    ofCircle(pos.x, pos.y, scale * 8.0);
}

