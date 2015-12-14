//
//  Specimen.cpp
//  mysteryDoor1
//
//  Created by Caroline Record on 12/14/15.
//
//

#include "Specimen.h"


void Specimen::setup(string fileN){
    filename = fileN;
    ofxCsv csv;
    csv.loadFile(ofToDataPath(filename + ".csv"));
    commonName = csv.data[0][0];
    region = csv.data[0][1];
    date = ofToInt(csv.data[0][2]);
    

}


void Specimen::draw(){
    mesh.draw();
}


void Specimen::load(){
    model.loadModel(filename + ".ply");
	mesh = model.getMesh(0);
    mySound.load(filename +".mp3");
    mySound.setLoop(true);
    mySound.play();
}

void Specimen::unload(){
    model.clear();
    mesh.clear();
    mySound.unload();
}


