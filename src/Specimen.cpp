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
    
    ofLog()<< "1";
    regFade.setup(region, 8000, 30,"PT.ttf" );
    ofLog()<< "2";
    dateFade.setup(ofToString(date), 8000, 30,"PT.ttf" );
    ofLog()<< "3";
    nameFade.setup(commonName, 8000, 30,"PT.ttf" );
    
    waitTime1 = 2000;
    waitTime2 = 2500;
    waitTime3 = 3000;
    
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
    
    
    int timeDiff = ofGetElapsedTimeMillis() - timeOpened;
    
        if(!nameFade.isFadeIn & (timeDiff >= waitTime1)){
            nameFade.triggerStart();
        }
        else if(!regFade.isFadeIn & (timeDiff >= waitTime2)){
            regFade.triggerStart();
        }
        else if(!dateFade.isFadeIn & (timeDiff >= waitTime3)){
            dateFade.triggerStart();
        }
  
    
    regFade.update();
    dateFade.update();
    nameFade.update();
}


void Specimen::load(){
  
    mySound.loadSound(filename +".mp3");
    mySound.setLoop(true);
    mySound.play();
    
    
    timeOpened = ofGetElapsedTimeMillis();
}

void Specimen::unload(){
    //model.clear();
    //mesh.clear();
    mySound.unloadSound();
    regFade.resetText();
    dateFade.resetText();
    nameFade.resetText();
}

void Specimen::drawText(bool isCentered, int x, int y){

    nameFade.draw(x,y);
    regFade.draw(x + nameFade.textHeight,y);
    dateFade.draw(x + nameFade.textHeight + regFade.textHeight,y);
   
}


