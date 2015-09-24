#include "Particles.h"


Particles::Particles() {
}


void Particles::setup(int count) {
    setup(count, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
}

void Particles::setup(int count, ofRectangle bounds){
    this->bounds = bounds;
	p.assign(count, Particle());
    currentMode = Particle::EAT_GROW;
	resetParticles();
}


void Particles::setParticleNum(int num) {
    p.clear();
    p.assign(num, Particle());
    resetParticles();
}

void Particles::applyParticleProps() {
    for(int i = 0; i < p.size(); i++){
		p[i].drag = ofRandom(minDrag, maxDrag);
	}
}


void Particles::resetParticles(){
	for(int i = 0; i < p.size(); i++){
        p[i].id=i;
        //p[i].color=ofColor(255,234,119);
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPoints);;
		p[i].reset(bounds);
	}	
}

void Particles::updateAttractPoints(){
	for(int i = 0; i < p.size(); i++){	
		p[i].setAttractPoints(&attractPoints);;
	}
}


void Particles::update(){
    
    bool flock = false;
    if (attractPoints.size() == 0) {
        flock = true;
    }
    
    ofVec2f centre = bounds.getCenter();
    float radius = bounds.getHeight()/2;
    for(int i = 0; i < p.size(); ++i){
        // TODO: determine ambient behaviour
        if (flock) {
            /*
            ofPoint closestPt;
            int closest = -1; 
            float closestDist = 9999999;
            for(int j = 0; j < p.size(); ++j){
                float lenSq = p[j].pos.distanceSquared(p[i].pos);
                if( lenSq < closestDist && p[i].id!=p[j].id){
                    closestDist = lenSq;
                    closest = j;
                }
            }
            if (closest!=-1) {
                closestPt=p[closest].pos;
                p[i].attractPoints->clear();
                p[i].attractPoints->push_back(closestPt);
            }
             */
            /*
            if (i != 0) {
                p[i].attractPoints->clear();
                p[i].attractPoints->push_back(p[0].pos);
            }
             */
        }
        p[i].setMode(currentMode);
        p[i].update();
        
        // circular bounds check
        if (p[i].pos.distanceSquared(centre) > radius * radius) {
            p[i].vel += (centre - p[i].pos) * 0.01;
        }
	}
}


void Particles::draw(bool drawGrey){
    
	for(int i = 0; i < p.size(); i++){
        p[i].draw();
	}
    
//    int n = p.size();
//	glEnable(GL_POINT_SIZE);
//	glPointSize(10);
//	glBegin(GL_POINTS);
//    for(int i = 0; i < n; i++){
//        glVertex2f(p[i].pos.x, p[i].pos.y);
//    }
//	glEnd();
//	glDisable(GL_POINT_SIZE);
    
    /*
    ofSetColor(255, 255, 0);
    for(unsigned int i = 0; i < attractPoints.size(); i++){
        ofNoFill();
        ofCircle(attractPointsWithMovement[i], 10);
        ofFill();
        ofCircle(attractPointsWithMovement[i], 4);
    }
     */
    
    ofSetColor(255);
}

void Particles::resetEating() {
    for (auto & particle : p) {
        particle.setScale(ofRandom(0.5, 1.0));
    }
}


inline void Particles::fastNormalize(ofVec3f &vec){
    float length2 = vec.x*vec.x+vec.y*vec.y+vec.z*vec.z;//calculate length^2
	
    if(length2 != 0.0f){
        float inverseSqrt;
#ifdef SSE
        normalizeData[0] = length2;
        __m128 data = _mm_load_ss(normalizeData);
        __m128 result = _mm_rsqrt_ps(data);
        _mm_store_ps(normalizeData, result);
        inverseSqrt = normalizeData[0];
#else
		const float threehalfs = 1.5F;
		long i;
		float x2, y;
		x2 = length2 * 0.5F;
		y  = length2;
		i  = * ( long * ) &y;                       // evil floating point bit level hacking [sic]
		i  = 0x5f3759df - ( i >> 1 );               // what the fuck? [sic]
		y  = * ( float * ) &i;
		inverseSqrt = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
		//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
#endif
		
        vec.x *= inverseSqrt;
        vec.y *= inverseSqrt;
        vec.z *= inverseSqrt;
    }
}