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

using namespace wng;

#ifndef __mysteryDoor1__Specimen__
#define __mysteryDoor1__Specimen__

#include <iostream>

class Specimen {
    
public:
    void setup(string fileN);
    void load();
    void draw();
    void unload();
    string commonName;
    string region;
    int date;
private:
    ofxAssimpModelLoader model;
    ofMesh mesh;
    string filename;
    ofSoundPlayer mySound;

};

#endif /* defined(__mysteryDoor1__Specimen__) */
