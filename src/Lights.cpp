//
//  lights.cpp
//  mini_Door
//
//  Created by Caroline Record on 12/16/15.
//
//

#include "Lights.h"

void Lights::setup(){
    light.setPointLight();
    //light.setDirectional();
    light.setPosition(0, 20, 20);
    //light.setAmbientColor(ofFloatColor(0,0,1));
    light.enable();
    
    lightGui.setup();
    
    lightGui.add(posX.setup("pos x", 0, ofGetWidth(), 0));
    lightGui.add(posY.setup("pos y", 0, ofGetHeight(), 0));
    lightGui.add(posZ.setup("pos z", -500, 500, 0));
    
    lightGui.add(colorAmb.setup("color ambient", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorSpec.setup("color Specular", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorDiff.setup("color Diffuse", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
}

void Lights::updateLights(){
    /*
    ofColor lights = color;
    //ofLog()<< int(lights.r)/255;
    light.setDiffuseColor(ofFloatColor(float(lights.r)/255.f, float(lights.g)/255.f, float(lights.b)/255.f));
    light.setPosition(posX, posY, posZ);
     */
}

void Lights::enable(){
    light.enable();
}

void Lights::disable(){
    light.disable();
}


void Lights::drawGui(){
    lightGui.draw();
}

