//
//  Specimen.cpp
//  mysteryDoor1
//
//  Created by Caroline Record on 12/14/15.
//
//

#include "Specimen.h"


float Specimen::vol= .5f;

void Specimen::setup(string fileN){
    filename = fileN;
    ofxCsv csv;
    csv.loadFile(ofToDataPath(filename + ".csv"));
    commonName = csv.data[0][0];
    region = csv.data[0][1];
    date = ofToInt(csv.data[0][2]);
    model.loadModel(filename + ".ply");
	mesh = model.getMesh(0);

}


void Specimen::draw(){
    mySound.setVolume(vol);
    mesh.draw();
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


