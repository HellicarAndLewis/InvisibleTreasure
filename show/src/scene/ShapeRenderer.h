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
    ShapeBase() {
        bounds.setFromCenter(ofGetWidth()/2, ofGetHeight()/2, ofGetHeight()/3, ofGetHeight()/3);
    }
    virtual void draw() {
        if (state == INTRO) {
            ofSetColor(255,255,255, progress*255);
        }
        else if (state == OUTRO) {
            ofSetColor(255,255,255, (1-progress)*255);
        }
        else if (state == INTERACTIVE) {
            ofSetColor(255,255,255,255);
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
// Square
//////////////////////////////////////////////////////////////////////////////////
class SquareRenderer : public ShapeBase {
public:
    void draw() {
        ShapeBase::draw();
        ofRect(bounds);
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
    
    TriangleRenderer triangle;
    CircleRenderer circle;
    SquareRenderer square;
    vector<ShapeBase*> shapes;
    int shapeIndex;
    ShapeBase* shapeIn;
    ShapeBase* shapeOut;
    
    void setup() {
        shapeIndex = -1;
        shapes.push_back(&triangle);
        shapes.push_back(&circle);
        shapes.push_back(&square);
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
        for (auto shape: shapes)
            if (shape->state != Sequencable::INACTIVE) {
                shape->draw();
            }
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
    void showShape(int id){
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
};

