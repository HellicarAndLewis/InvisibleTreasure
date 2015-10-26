#include "Particles.h"


Particles::Particles() {
}


void Particles::setup(int count) {
    setup(count, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
}

void Particles::setup(int count, ofRectangle bounds){
    this->bounds = bounds;
    background.allocate(bounds.width, bounds.height);
    background.begin();
    ofClear(0);
    background.end();
	p.assign(count, Particle());
    currentMode = Particle::EAT_GROW;
    eatBackground = false;
    allFull = false;
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
    
    if (eatBackground) {
        background.begin();
        ofSetColor(0);
        if (ofGetFrameNum() % (60*5) == 0) {
            addShapesToBg();
        }
    }
    
    int fullParticles = 0;
    for(int i = 0; i < p.size(); ++i){
        
        if (eatBackground) {
            ofPoint pos = p[i].pos;
            p[i].inShape = false;
            for (auto & bounds: backgroundShapes) {
                if (bounds.inside(pos)) {
                    if (p[i].isEating) ofCircle(p[i].pos.x, p[i].pos.y, p[i].scale * 16.0);
                    p[i].inShape = true;
                }
            }
            
        }
        
        p[i].setMode(currentMode);
        p[i].colorInhibitor = colorInhibitor;
        p[i].eatBlobs = !eatBackground;
        p[i].update();
        if (p[i].isFull) {
            fullParticles++;
        }
        
        // circular bounds check
        if (p[i].pos.distanceSquared(centre) > radius * radius) {
            p[i].vel += (centre - p[i].pos) * 0.01;
        }
	}
    
    if (eatBackground) {
        ofSetColor(255);
        background.end();
    }
    cout<<"Numer of Full Particles: "<<fullParticles<<endl;
    cout<<"Total Number of Particles: "<<p.size()<<endl;
    if ( ( (float) fullParticles / (float) p.size() ) > 0.9 ) { //arithmetic error here previously?
        allFull = true;
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
    ofSetColor(255, 0, 0);
    for(unsigned int i = 0; i < attractPoints.size(); i++){
        ofNoFill();
        ofRect(attractPoints[i]);
        ofFill();
        ofCircle(attractPoints[i].getCenter(), 4);
    }
     */
    
    ofSetColor(255);
}

void Particles::resetEating() {
    for (auto & particle : p) {
        particle.setScale(ofRandom(0.5, 1.0));
        particle.fullness = 0;
        particle.isFull = false;
    }
    allFull = false;
}

void Particles::setColour(ofColor colour){
    this->colour = colour;
    for (auto & particle : p) {
        particle.color = colour;
    }
}

void Particles::addShapesToBg() {
    background.begin();
    //ofClear(0, 0, 0, 0);
    ofSetColor(0, 0, 200);
    for (int i = 0; i < 6; i++) {
        generateShape();
    }
    ofSetColor(255);
    background.end();
}

void Particles::generateShape() {
    float minX = bounds.getLeft() + 300;
    float maxX = bounds.getRight() - 300;
    float minY = bounds.getTop() + 300;
    float maxY = bounds.getBottom() - 300;
    float width = ofRandom(100, 200);
    ofRectangle bounds = ofRectangle(ofRandom(minX, maxX), ofRandom(minY, maxY), width, width);
    ofCircle(bounds.getCenter(), bounds.getHeight()/2);
    backgroundShapes.push_back(bounds);
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