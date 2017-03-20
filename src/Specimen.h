//
//  Specimen.h
//  mysteryDoor1
//
//  Created by Caroline Record on 12/14/15.
//
//

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxCsv.h"
#include "fadeTextIn.h"
#include "ofxXmlSettings.h"

using namespace wng;

#ifndef __mysteryDoor1__Specimen__
#define __mysteryDoor1__Specimen__

#include <iostream>

class Specimen {
    
public:
    static float vol;
    static bool drawTex;
    static bool drift;
    static bool txtSize;
    void setup(string fileN);
    void load();
    void draw();
    void unload();
    void drawText(bool isCentered, int x, int y);
    void triggerStartText();
    void scaleModel(float scale);
    float getScale(); 
    void rotateModel(ofVec3f rot);
    void positionModel(ofVec2f modelPosition);
    void saveAnimalSettings();
    ofVec3f currRot;
    ofVec2f currPosition; 
    
    
    string commonName;
    string region;
    string date;
private:
    
    fadeTextIn dateFade;
    fadeTextIn nameFade;
    fadeTextIn regFade;
    ofxXmlSettings XMLSet;
    
    ofxAssimpModelLoader model;
    ofMesh mesh;
    string filename;
    ofSoundPlayer mySound;
    float perlinSeed;
    
    int timeOpened;
    int waitTime1;
    int waitTime2;
    int waitTime3;
    
    bool isOpened;
    bool xmlLoaded;
};

#endif /* defined(__mysteryDoor1__Specimen__) */
