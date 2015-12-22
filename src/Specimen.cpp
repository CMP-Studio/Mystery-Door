//
//  Specimen.cpp
//  mysteryDoor1
//
//  Created by Caroline Record on 12/14/15.
//
//

#include "Specimen.h"


float Specimen::vol= .5f;
bool Specimen::drawTex =true;
bool Specimen::drift =false;

void Specimen::setup(string fileN){
    filename = fileN;
    ofxCsv csv;
    csv.loadFile(ofToDataPath(filename + ".csv"));
    commonName = csv.data[0][0];
    region = csv.data[0][1];
    date = ofToInt(csv.data[0][2]);
    model.loadModel(filename + ".ply");
	mesh = model.getMesh(0);
    perlinSeed =0;
    
}


void Specimen::draw(){
    
    if(drawTex){
        mesh.enableColors();
    }
    else{
        mesh.disableColors();
    }
    
    if(drift){
        perlinSeed += .001f;
        float rotationX =ofMap(ofNoise(perlinSeed,1), 0,1,-10,10);
        ofRotate(rotationX, 1, 0, 0);
        ofRotate(ofMap(ofNoise(perlinSeed,500), 0,1,0,20), 0,-10,10);
        ofRotate(ofMap(ofNoise(perlinSeed,5000), 0,1,0,20), 0,-10,10);
    }
    mySound.setVolume(vol);
    mesh.draw();
    //mesh.norm
}


void Specimen::load(){
  
    mySound.loadSound(filename +".mp3");
    mySound.setLoop(true);
    mySound.play();
    
}

void Specimen::unload(){
    //model.clear();
    //mesh.clear();
    mySound.unloadSound(); 
}


