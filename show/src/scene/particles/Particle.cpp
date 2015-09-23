#include "Particle.h"


Particle::Particle(){
	attractPoints = NULL;
    minUnique = -10000;
    maxUnique = 1000;
    minDrag = 0.95;
    maxDrag = 0.97;
    flock = false;
    mode = PARTICLE_MODE_NEAREST_POINTS;
    color.set(255, 255, 0);
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
    uniqueVal = ofRandom(minUnique, maxUnique);
    pos.x = ofRandom(bounds.getLeft(), bounds.getRight());
    pos.y = ofRandom(bounds.getTop(), bounds.getBottom());
    vel.x = ofRandom(-1, 1);
    vel.y = ofRandom(-1, 1);
    frc   = ofPoint(0,0,0);
    scale = ofRandom(0.5, 1.0);
    drag  = ofRandom(minDrag, maxDrag);
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
                if( dist > 40 && !ofGetKeyPressed('l')){
                    frc *= 0.003;
                    frc.x += ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
                    frc.y += ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
                    vel += frc;
                }else{
                    frc.x = ofSignedNoise(uniqueVal,               pos.y * 0.01, ofGetElapsedTimef()*0.2);
                    frc.y = ofSignedNoise(ofGetElapsedTimef()*0.2, uniqueVal,    pos.x * 0.01);
                    vel += frc * 0.4;
                }
                
            }
            
        }
        
    }
	
    /*
	// attract to nearest neighbour
    if(flock && attractPoints->size()>0){
        closestPt = attractPoints->operator[](0);
        float closestDist = ( closestPt-pos ).length();
        float dist = InvSqrt(closestDist);
        frc = closestPt - pos;
        vel *= drag;
        float r=ofMap(dist, 1.5, 0, 255, 0);
        float b=ofMap(dist, 0, 1.5, 255, 0);
        float a=ofMap(dist, 0, 1.5, 50, 255);
        a=60;
        color.lerp( ofColor( 0, 40, b, a), 0.4 );
        if( dist < 1.5 && dist > .1 && !ofGetKeyPressed('l') ){
            vel += frc * 0.01;
        }
        else {
            vel += frc * .001;
        }
    }
     */
    
    // update pos using velocity
	pos += vel;
    
    // trails
    trail.push_back(pos);
    if (trail.size() > 10) trail.erase(trail.begin());
    if (trail.size() > 10) trail.erase(trail.begin());
    
}

void Particle::setNeighbours(vector <Particle> * neighbours){
    this->neighbours=neighbours;
}


void Particle::draw(){
    //glVertex2f(pos.x, pos.y);
//    ofSetColor(color.r,color.g,color.b, color.a);
    ofSetColor(color.r,color.g,color.b, 255);
    // draw trails?
    if (ofGetKeyPressed('t')) {
        for (int i=1; i<trail.size()-1; i++) ofLine(trail[i], trail[i-1]);
        //for (int i=1; i<trail.size()-1; i++) ofCircle(trail[i], scale * i);
    }
    else {
        ofCircle(pos.x, pos.y, pos.z, scale * 8.0);
    }
    // connect neighbours?
    if (flock) ofLine(closestPt, pos);
    
    
}

