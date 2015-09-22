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

//////////////////////////////////////////////////////////////////////////////////
// Shape Renderers
//////////////////////////////////////////////////////////////////////////////////
class ShapeBase : public Sequencable {
public:
    ofRectangle bounds;
    bool success = false;
    ShapeBase() {
        bounds.setFromCenter(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()/3, ofGetHeight()/3);
    }
    virtual void draw() {
        if (state == INTRO) {
            ofSetColor(0,0,0, progress*255);
        }
        else if (state == OUTRO) {
            ofSetColor(0,0,0, (1-progress)*255);
        }
        else if (state == INTERACTIVE) {
            ofSetColor(0,0,0,255);
        }
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
        float outerRadius = bounds.getWidth();
        float innerRadius = bounds.getWidth()/2;
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
        path.setColor(ofColor(255));
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
        ofRect(bounds.getLeft(), bounds.getHeight()/4, bounds.getWidth(), bounds.getHeight()/2);
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
    }
    
    void update() {
        for (auto shape: shapes)
            if (shape->state != Sequencable::INACTIVE) {
                shape->update();
            }
    }
    
    void draw() {
        ofSetHexColor(0xFF62C2);
        ofRect(bg);
        for (auto shape: shapes)
            if (shape->state != Sequencable::INACTIVE) {
                shape->draw();
            }
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
            shapeIn->stop();
            shapeIn = NULL;
        }
        if (shapeOut != NULL) {
            shapeOut->stop();
            shapeOut = NULL;
        }
    }
};

