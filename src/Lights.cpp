//
//  lights.cpp
//  mini_Door
//
//  Created by Caroline Record on 12/16/15.
//
//

#include "Lights.h"

void Lights::setup(int ID){
    light.setPointLight();
    //light.setDirectional();
    light.setPosition(0, 20, 20);
    //light.setAmbientColor(ofFloatColor(0,0,1));
    light.enable();
    
    uniqueID = ID;
    
    lightGui.setup();
    lightGui.add(label.setup("Light Controls " + ofToString(uniqueID), ""));
    
    lightGui.add(drawLight.setup("draw light", false));
    
    lightGui.add(posX.setup("pos x", 0, ofGetWidth(), 0));
    lightGui.add(posY.setup("pos y", 0, ofGetHeight(), 0));
    lightGui.add(posZ.setup("pos z", 0, 2000, 0));
    
    lightGui.add(colorAmb.setup("color ambient", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorSpec.setup("color Specular", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorDiff.setup("color Diffuse", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.loadFromFile("light_"+ofToString(uniqueID)+ ".xml");
    
}

void Lights::updateLights(){
    
    light.setPosition(posX, posY, posZ);
    ofColor cd = colorDiff;
    light.setDiffuseColor(ofFloatColor(float(cd.r)/255.f, float(cd.g)/255.f, float(cd.b)/255.f));
    ofColor cs = colorSpec;
    light.setDiffuseColor(ofFloatColor(float(cs.r)/255.f, float(cs.g)/255.f, float(cs.b)/255.f));
    ofColor ca = colorAmb;
    light.setDiffuseColor(ofFloatColor(float(ca.r)/255.f, float(ca.g)/255.f, float(ca.b)/255.f));
    
    if(drawLight){
        light.draw(); 
    }
    /*
    ofColor lights = color;
    //ofLog()<< int(lights.r)/255;
    light.setDiffuseColor(ofFloatColor(float(lights.r)/255.f, float(lights.g)/255.f, float(lights.b)/255.f));
    light.setPosition(posX, posY, posZ);
     */
}

void Lights::saveGui(){
    //lightGui.sa
    ofLog()<< "saving";
    ofLog()<< "light_"+ofToString(uniqueID)+ ".xml";
    
    lightGui.saveToFile("light_"+ofToString(uniqueID)+ ".xml");
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

