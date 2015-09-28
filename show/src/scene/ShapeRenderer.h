//
//  ShapeRenderers.h
//  show
//
//  Created by Chris Mullany on 04/09/2015.
//
//

#pragma once
#include "ofMain.h"
#include "Sequencable.h"
#define PINK 0xFF62C2
#define BLACK 0x222222

//////////////////////////////////////////////////////////////////////////////////
// Shape Renderers
//////////////////////////////////////////////////////////////////////////////////
class ShapeBase : public Sequencable {
public:
    ofRectangle bounds;
    bool filling = false;
    ofColor colour;
    
    ShapeBase() {
        bounds.setFromCenter(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()*.8, ofGetHeight()*.8);
    }
    void fill() {
        timeOut = 3;
        filling = true;
        Sequencable::stop();
    }
    virtual void draw() {
        if (filling) {
            ofSetColor(colour, 255);
        }
        else if (state == INTRO) {
            ofSetColor(colour, progress*255);
        }
        else if (state == OUTRO) {
            ofSetColor(colour, (1-progress)*255);
        }
        else if (state == INTERACTIVE) {
            ofSetColor(colour, 255);
        }
    }
    void play() {
        filling = false;
        Sequencable::play();
    }
    void stop() {
        timeOut = 0.5;
        filling = false;
        Sequencable::stop();
    }
};

//////////////////////////////////////////////////////////////////////////////////
// Triangle
//////////////////////////////////////////////////////////////////////////////////
class TriangleRenderer : public ShapeBase {
public:
    void draw() {
        ShapeBase::draw();
        ofTriangle(
                   ofPoint(bounds.getCenter().x, bounds.getTop()),
                   bounds.getBottomLeft(),
                   bounds.getBottomRight());
    }
};

//////////////////////////////////////////////////////////////////////////////////
// Star
//////////////////////////////////////////////////////////////////////////////////
class StarRenderer : public ShapeBase {
public:
    void draw() {
        ShapeBase::draw();
        
        float x = bounds.getCenter().x;
        float y = bounds.getCenter().y;
        int pointCount = 10;
        float outerRadius = bounds.getWidth()/2;
        float innerRadius = bounds.getWidth()/4;
        ofPath path;
        path.newSubPath();
        auto angleChangePerPt = ((pi) * 2) / pointCount;
        auto angle = pi * 1.5;
        path.moveTo(x + (outerRadius * cos(angle)), y + (outerRadius * sin(angle)));
        float time = ofGetElapsedTimef();
        ofPoint firstPoint;
        for (int i = 0; i < pointCount; i++) {
            auto rad = (i%2==0) ? outerRadius : innerRadius;
            auto vertX = x + (rad * cos(angle));
            auto vertY = y + (rad * sin(angle));
            path.lineTo(vertX, vertY);
            angle += angleChangePerPt;
        };
        path.curveTo(firstPoint);
        if (ofGetFill() == OF_FILLED) {
            path.setFilled(true);
            path.setFillColor(ofGetStyle().color);
        }
        else {
            path.setFilled(false);
        }
        path.setStrokeWidth(5);
        path.setStrokeColor(ofGetStyle().color);
        path.close();
        path.draw();
        
    }
};

//////////////////////////////////////////////////////////////////////////////////
// Square
//////////////////////////////////////////////////////////////////////////////////
class SquareRenderer : public ShapeBase {
public:
    void draw() {
        ShapeBase::draw();
        ofRectangle rect = bounds;
        rect.scaleFromCenter(1, 0.6);
        ofRect(rect);
    }
};

//////////////////////////////////////////////////////////////////////////////////
// Circle
//////////////////////////////////////////////////////////////////////////////////
class CircleRenderer : public ShapeBase {
public:
    void draw() {
        ShapeBase::draw();
        ofCircle(bounds.getCenter(), bounds.getWidth()/2);
    }
};


//////////////////////////////////////////////////////////////////////////////////
// Shape Renderer
//////////////////////////////////////////////////////////////////////////////////
class ShapeRenderer {
public:
    
    enum ShapeMode {
        CIRCLE=0,RECTANGLE,TRIANGLE,STAR
    } shapeMode;
    
    TriangleRenderer triangle;
    CircleRenderer circle;
    SquareRenderer square;
    StarRenderer star;
    vector<ShapeBase*> shapes;
    int shapeIndex;
    ShapeBase* shapeIn;
    ShapeBase* shapeOut;
    ofRectangle bg;
    
    ofFbo maskFbo;
    ofFbo fbo;
    ofFbo shaderFbo;
    ofShader shader;
    bool invert;
    
    void setup() {
        bg.set(0, 0, ofGetWidth(), ofGetHeight());
        shapeIndex = -1;
        shapes.push_back(&circle);
        shapes.push_back(&square);
        shapes.push_back(&triangle);
        shapes.push_back(&star);
        shapeIn = NULL;
        shapeOut = NULL;
        for (auto shape: shapes) {
            ofAddListener(shape->stateChangeEvent, this, &ShapeRenderer::onShapeChange);
        }
        
        int width = ofGetWidth()-1;
        int height = ofGetHeight()-1;
        maskFbo.allocate(width,height);
        fbo.allocate(width,height);
        shaderFbo.allocate(width,height);
        
        if(ofGetGLProgrammableRenderer()){
            string vertex = "#version 150\n\
            \n\
            uniform mat4 projectionMatrix;\n\
            uniform mat4 modelViewMatrix;\n\
            uniform mat4 modelViewProjectionMatrix;\n\
            \n\
            \n\
            in vec4  position;\n\
            in vec2  texcoord;\n\
            \n\
            out vec2 texCoordVarying;\n\
            \n\
            void main()\n\
            {\n\
            texCoordVarying = texcoord;\
            gl_Position = modelViewProjectionMatrix * position;\n\
            }";
            string fragment = "#version 150\n\
            \n\
            uniform sampler2DRect tex0;\
            uniform sampler2DRect maskTex;\
            in vec2 texCoordVarying;\n\
            \
            out vec4 fragColor;\n\
            void main (void){\
            vec2 pos = texCoordVarying;\
            \
            vec3 src = texture(tex0, pos).rgb;\
            float mask = texture(maskTex, pos).r;\
            \
            fragColor = vec4( src , mask);\
            }";
            shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment);
            shader.bindDefaults();
            shader.linkProgram();
        }else{
            string shaderProgram = "#version 120\n \
            #extension GL_ARB_texture_rectangle : enable\n \
            \
            uniform sampler2DRect tex0;\
            uniform sampler2DRect maskTex;\
            \
            void main (void){\
            vec2 pos = gl_TexCoord[0].st;\
            \
            vec3 src = texture2DRect(tex0, pos).rgb;\
            float mask = texture2DRect(maskTex, pos).r;\
            \
            gl_FragColor = vec4( src , mask);\
            }";
            shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
            shader.linkProgram();
        }
    
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
        fbo.begin();
        ofClear(0,0,0,255);
        fbo.end();
        shaderFbo.begin();
        ofClear(0,0,0,255);
        shaderFbo.end();
    }
    
    void update() {
        for (auto shape: shapes)
            if (shape->state != Sequencable::INACTIVE) {
                shape->update();
            }
    }
    
    void draw() {
        ofPushStyle();
        if (invert) ofSetHexColor(BLACK);
        else ofSetHexColor(PINK);
        ofRect(bg);
        ofSetLineWidth(5);
        for (auto shape: shapes)
            if (shape->state != Sequencable::INACTIVE) {
                if (invert) shape->colour.setHex(PINK);
                else shape->colour.setHex(BLACK);
                ofFill();
                if (shape->filling) {
                    // MASK
                    maskFbo.begin();
                    ofClear(0,0,0,255);
                    ofSetColor(255);
                    ofRect(0, maskFbo.getHeight(), maskFbo.getWidth(), -maskFbo.getHeight()*shape->progress);
                    maskFbo.end();
                    
                    // Thing to mask: the filled shape
                    fbo.begin();
                    ofClear(0, 0);
                    if (invert) ofSetHexColor(BLACK);
                    else ofSetHexColor(PINK);
                    ofRect(bg);
                    shape->draw();
                    fbo.end();
                    
                    // mask shader
                    shader.begin();
                    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
                    fbo.draw(0, 0);
                    shader.end();
                }
                ofNoFill();
                shape->draw();
            }
        ofPopStyle();
    }
    
    ofRectangle getBgRect() {
        return bg;
    }
    
    void onShapeChange(Sequencable::State & state) {
        if (state == SceneBase::INACTIVE) {
            //ofLogVerbose() << "ShapeRenderer::onShapeChange: INACTIVE, play next shape";
            if (shapeIn != NULL) {
                shapeIn->play();
                shapeOut = NULL;
            }
        }
    }
    void showShape(ShapeMode shape){
        int id = (int)shape;
        if (id >= 0 && id < shapes.size() && id != shapeIndex) {
            shapeIndex = id;
            if (shapeIn == NULL) {
                shapeIn = shapes[id];
                shapeIn->play();
            }
            else {
                shapeOut = shapeIn;
                shapeIn = shapes[id];
                shapeOut->stop();
            }
        }
    }
    void hide(bool success = false) {
        shapeIndex = -1;
        if (shapeIn != NULL) {
            if (success) shapeIn->fill();
            else shapeIn->stop();
            shapeIn = NULL;
        }
        if (shapeOut != NULL) {
            shapeOut->stop();
            shapeOut = NULL;
        }
    }
};

