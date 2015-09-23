#include "Particles.h"


Particles::Particles() {
}

void Particles::setup(){
	int num = 1000;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;
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
        p[i].color=ofColor(60);
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

void Particles::updateAttractPoints(){
    attractPointsWithMovement = attractPoints;
	for(int i = 0; i < p.size(); i++){	
		p[i].setAttractPoints(&attractPointsWithMovement);;
	}
}


void Particles::update(){
	
//	for(int i = 0; i < attractPointsWithMovement.size(); i++){
//		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
//		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
//		attractPointsWithMovement[i].z = attractPoints[i].z + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
//	}
    
    // TODO: swap this out for flann nearest neigbour?
    for(int i = 0; i < p.size(); ++i){
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
        p[i].setMode(currentMode);
        p[i].update();
	}
}


void Particles::draw(bool drawGrey){
    
	for(int i = 0; i < p.size(); i++){
        p[i].draw();
	}
    
//    int n = p.size();
//	glEnable(GL_POINT_SIZE);
//	glPointSize(2);
//	glBegin(GL_POINTS);
//	for(int i = 0; i < n; i++)
//		p[i].draw();
//	glEnd();
//	glDisable(GL_POINT_SIZE);
    
    ofSetColor(150);
    string s = (p[0].flock) ? "flocking" : "not flocking";
    ofDrawBitmapString(s, 10, 10);
    
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