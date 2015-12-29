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

using namespace wng;

#ifndef __mysteryDoor1__Specimen__
#define __mysteryDoor1__Specimen__

#include <iostream>

class Specimen {
    
public:
    static float vol;
    static bool drawTex;
    static bool drift;
    void setup(string fileN);
    void load();
    void draw();
    void unload();
    void drawText(bool isCentered, int x, int y);
    
    string commonName;
    string region;
    int date;
private:
    
    fadeTextIn dateFade;
    fadeTextIn nameFade;
    fadeTextIn regFade;
    
    
    ofxAssimpModelLoader model;
    ofMesh mesh;
    string filename;
    ofSoundPlayer mySound;
    float perlinSeed;
    
    int timeOpened;
    int waitTime1;
    int waitTime2;
    int waitTime3;
};

#endif /* defined(__mysteryDoor1__Specimen__) */
