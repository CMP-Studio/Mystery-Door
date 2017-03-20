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
    lightDouble.setPointLight();
    //light.setDirectional();
    light.setPosition(0, 20, 20);
    lightDouble.setPosition(0, 20, 20);
    //light.setAmbientColor(ofFloatColor(0,0,1));
    light.enable();
    lightDouble.enable();
    //light.disable();
    
    uniqueID = ID;
    
    lightGui.setup();
    lightGui.add(label.setup("Light Controls " + ofToString(uniqueID), ""));
    
    lightGui.add(drawLight.setup("draw light", false));
    
    lightGui.add(pos.setup("pos",ofVec3f(0,0,0),ofVec3f(ofGetWidth()*-2,ofGetHeight()*-02,-3000),ofVec3f(ofGetWidth()*2,ofGetHeight()*2,3000)));
    
    lightGui.add(smoothFlicker.setup("Smooth flicker", false));
    lightGui.add(minimumFlicker.setup("Min flicker",.5,0,1));
    lightGui.add(maximumFlicker.setup("Max flicker",.5,0,1));
    lightGui.add(speedOfFlicker.setup("Speed of flicker",.5,0,1));
    lightGui.add(speedOfFade.setup("Speed of fade",.2,0,.3));
    
    lightGui.add(colorAmb.setup("color ambient", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorSpec.setup("color Specular", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.add(colorDiff.setup("color Diffuse", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    lightGui.loadFromFile("light_"+ofToString(uniqueID)+ ".xml");
    
    isFlicker = true;
    isFadeOn = true;
    fadePercent = 0;

    noiseSeed =0; 
    targetBrightFadeToo = ofMap(ofNoise(noiseSeed),0,1,minimumFlicker, maximumFlicker);
    brightFactor =0; 
}

void Lights::updateLights(){
    
    ofColor cd = colorDiff;
    ofColor cs = colorSpec;
    ofColor ca = colorAmb;
    ofColor b = ofColor::black;
    
    
    if (isClosed){
        // the door is closed so turn off all the lights.
        ca.set(0);
        cs.set(0);
        cd.set(0);
        dmx.setLevel(uniqueID,  dmxLightMin);
        ofLog()<< "closed !";
    }
    else if(isFadeOn){
        //oflog()<< "fade is on before: " << fadePercent;
        if (fadePercent<1){
            float flickering=0;
            
            /*
            if (isFlicker){
                flickering = ofMap(ofNoise(noiseSeed),0,1,-.2f, .2f);
            }
             */
            float brightnessForNow = ofMap(fadePercent, 0,1,0,targetBrightFadeToo);
            ca.lerp(b, 1-brightnessForNow);
            cs.lerp(b, 1-brightnessForNow);
            cd.lerp(b, 1-brightnessForNow);
            fadePercent += speedOfFade;
            //ofLog()<< "brightness for now: " << brightnessForNow;
            
            float dmxLevel = ofMap(brightnessForNow ,1,0, dmxLightMax, dmxLightMin);
            //oflog()<< "dmx level during fade " << dmxLevel;
            dmx.setLevel(uniqueID, dmxLevel);
            //ofLog()<< "end: " << ;
        }
        else{
            fadePercent=0;
            isFadeOn= false;
            float brightnessForNow = ofMap(1, 0,1,0,targetBrightFadeToo);
            ca.lerp(b, 1-brightnessForNow);
            cs.lerp(b, 1-brightnessForNow);
            cd.lerp(b, 1-brightnessForNow);
        }
        //ofLog()<< "speed of fade: " << speedOfFade;

        
    }
    else if(isFlicker){
        
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
        ca.lerp(b, 1-brightFactor);
        cs.lerp(b, 1-brightFactor);
        cd.lerp(b, 1-brightFactor);
        
        float dmxLevel = ofMap(brightFactor , 1,0, dmxLightMax, dmxLightMin);
        //oflog()<< "dmxLightMax " << dmxLightMax;
        //oflog()<< "bright factor " << brightFactor;
        //oflog()<< "dmx level during flicker " << dmxLevel;
        dmx.setLevel(uniqueID, dmxLevel);
        
    }

    
    //ca=b;
    
    light.setPosition(pos);
    
    light.setDiffuseColor(ofFloatColor(float(cd.r)/255.f, float(cd.g)/255.f, float(cd.b)/255.f));
    light.setSpecularColor(ofFloatColor(float(cs.r)/255.f, float(cs.g)/255.f, float(cs.b)/255.f));
    light.setAmbientColor(ofFloatColor(float(ca.r)/255.f, float(ca.g)/255.f, float(ca.b)/255.f));
    
    lightDouble.setPosition(pos);
    
    lightDouble.setDiffuseColor(ofFloatColor(float(cd.r)/255.f, float(cd.g)/255.f, float(cd.b)/255.f));
    lightDouble.setSpecularColor(ofFloatColor(float(cs.r)/255.f, float(cs.g)/255.f, float(cs.b)/255.f));
    lightDouble.setAmbientColor(ofFloatColor(float(ca.r)/255.f, float(ca.g)/255.f, float(ca.b)/255.f));
    
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
    //oflog()<< "saving";
    //oflog()<< "light_"+ofToString(uniqueID)+ ".xml";
    
    lightGui.saveToFile("light_"+ofToString(uniqueID)+ ".xml");
}

void Lights::updateTarget(){
    targetBrightFadeToo = ofMap(ofNoise(noiseSeed),0,1,minimumFlicker, maximumFlicker);
}

void Lights::enable(){
    light.enable();
    lightDouble.enable();
}

void Lights::disable(){
    light.disable();
    lightDouble.disable();
}


void Lights::drawGui(){
    lightGui.draw();
}

