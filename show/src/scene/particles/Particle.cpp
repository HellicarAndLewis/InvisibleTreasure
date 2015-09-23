#include "Particle.h"


Particle::Particle(){
	attractPoints = NULL;
    minUnique = -10000;
    maxUnique = 1000;
    minDrag = 0.90;
    maxDrag = 0.95;
    flock = false;
    mode = PARTICLE_MODE_NEAREST_POINTS;
    color.set(255,234,119);
    isFull = false;
}


void Particle::setMode(particleMode newMode){
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
    scale = ofRandom(0.5, 1.0);
    drag  = ofRandom(minDrag, maxDrag);
    isFull = false;
}


void Particle::update(){
    
    // attract / repel
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
        attractPt.z = -500;
		frc = attractPt-pos;
		frc.normalize();
		vel *= drag;
        frc.x += ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
        frc.y += ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
        frc.z += ofSignedNoise(uniqueVal, pos.z * 0.01, ofGetElapsedTimef()*0.2);
		vel += frc * 0.6;
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos;
		float dist = frc.length();
		frc.normalize();
		vel *= drag; 
		if( dist < 150 ){
			vel += -frc * 0.6;
		}
        else{
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
            frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
            frc.z = ofSignedNoise(uniqueVal, pos.z * 0.01, ofGetElapsedTimef()*0.8);
			vel += frc * 0.6;
		}
    }
    else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
        
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
            if( closest != -1 ){
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
                        isFull = true;
                        color.lerp( ofColor(67,224,109), 0.9 );
                    }
                    frc.x = ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
                    frc.y = ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
                    vel += frc * 0.4;
                }
                
            }
            else {
                frc.x = ofSignedNoise(uniqueVal, pos.y * 0.1, ofGetElapsedTimef()*0.2);
                frc.y = ofSignedNoise(uniqueVal, pos.x * 0.1, ofGetElapsedTimef()*0.2);
                vel += frc * 0.4;
                
                float maxVel = 6.0;
                vel.x = ofClamp(vel.x, -maxVel, maxVel);
                vel.y = ofClamp(vel.y, -maxVel, maxVel);
            }
            
        }
        
    }
	
    // update pos using velocity
	pos += vel;
    
    
}

void Particle::setNeighbours(vector <Particle> * neighbours){
    this->neighbours=neighbours;
}


void Particle::draw(){
    ofSetColor(color.r,color.g,color.b, 255);
    ofCircle(pos.x, pos.y, scale * 8.0);
}

