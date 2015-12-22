//
//  lights.cpp
//  mini_Door
//
//  Created by Caroline Record on 12/16/15.
//
//

#include "Lights.h"


ofxDmx Lights::dmx;
int Lights::dmxLightMin =0;
int Lights::dmxLightMax =255;

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
    
    
    lightGui.add(smoothFlicker.setup("Smooth flicker", false));
    lightGui.add(minimumFlicker.setup("Min flicker",.5,0,1));
    lightGui.add(maximumFlicker.setup("Max flicker",.5,0,1));
    lightGui.add(speedOfFlicker.setup("Speed of flicker",.5,0,1));
    lightGui.add(speedOfFade.setup("Speed of fade",.5,0,1));
    
    lightGui.add(colorAmb.setup("color ambient", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorSpec.setup("color Specular", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorDiff.setup("color Diffuse", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.loadFromFile("light_"+ofToString(uniqueID)+ ".xml");
    
    isFlicker = true;
    isFadeOn = true;
    fadePercent = 0;
}

void Lights::updateLights(){
    
    ofColor cd = colorDiff;
    ofColor cs = colorSpec;
    ofColor ca = colorAmb;
    ofColor b = ofColor::black;
    
    if(isFadeOn){
        if (fadePercent<1){
            float flickering=0;
            
            if (isFlicker){
                flickering = ofMap(ofNoise(noiseSeed),0,1,-.2f, .2f);
            }
        
            ca.lerp(b, 1-fadePercent + flickering);
            cs.lerp(b, 1-fadePercent + flickering);
            cd.lerp(b, 1-fadePercent + flickering);
            fadePercent += speedOfFade;
            
            dmx.setLevel(uniqueID, float(ofMap(1-fadePercent + flickering, 0, 1, dmxLightMax, dmxLightMin)));
        }
        else{
            fadePercent=0;
            isFadeOn= false;
        }
        
    }
    else if(isFlicker){
        float brightFactor;
        if(smoothFlicker){
            brightFactor = ofMap(ofNoise(noiseSeed),0,1,minimumFlicker, maximumFlicker);
            noiseSeed += speedOfFlicker;
        }
        else{
            brightFactor = ofRandom(minimumFlicker, maximumFlicker);
        }
        
        //noiseSeed
        //cd.setBrightness(brightFactor);
        //cs.setBrightness(brightFactor);
        ca.lerp(b, brightFactor);
        cs.lerp(b, brightFactor);
        cd.lerp(b, brightFactor);
        dmx.setLevel(uniqueID, float(ofMap(brightFactor, 0, 1,dmxLightMax, dmxLightMin)));
    }
    
    //ca=b;
    light.setPosition(posX, posY, posZ);
    
    light.setDiffuseColor(ofFloatColor(float(cd.r)/255.f, float(cd.g)/255.f, float(cd.b)/255.f));
    light.setSpecularColor(ofFloatColor(float(cs.r)/255.f, float(cs.g)/255.f, float(cs.b)/255.f));
    light.setAmbientColor(ofFloatColor(float(ca.r)/255.f, float(ca.g)/255.f, float(ca.b)/255.f));
    
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

