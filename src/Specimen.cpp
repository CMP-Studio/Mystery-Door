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
bool Specimen::txtSize =20;

void Specimen::setup(string fileN){
    filename = fileN;
    ofxCsv csv;
    csv.loadFile(ofToDataPath(filename + ".csv"));
    commonName = csv.data[0][0];
    region = csv.data[0][1];
    date = ofToString(csv.data[0][2]);
    //oflog() << filename; 
    model.loadModel(filename + ".dae");
	//mesh = model.getMesh(0);
    perlinSeed =0;
    
    regFade.setup(region, 9000, 25,"fontBody.ttc" );
    dateFade.setup(date, 9000, 25,"fontBody.ttc" );
    nameFade.setup(commonName, 9000, 40,"fontTitle.otf" );
    
    waitTime1 = 2000;
    waitTime2 = 3000;
    waitTime3 = 4000;
    isOpened = false;
    model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    currRot = ofVec3f(0,0,0);
    currPosition = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
   
    
    
    xmlLoaded = XMLSet.loadFile(filename+".xml");
    if (xmlLoaded) {
    
        float scle = XMLSet.getValue("SCALE", 1.00);
        scaleModel(scle);
        float rx = XMLSet.getValue("ROT:X", 0);
        float ry = XMLSet.getValue("ROT:Y", 0);
        float rz = XMLSet.getValue("ROT:Z", 0);
        currRot = ofVec3f(rx,ry,rz);
        rotateModel(currRot);
        float px = XMLSet.getValue("POS:X", ofGetWidth()/2);
        float py = XMLSet.getValue("POS:Y", ofGetHeight()/2);
        positionModel(ofVec2f(px,py));
        currPosition = ofVec2f(px,py);
    }
    
   // mySound.get

    
    
}

void Specimen::scaleModel(float scale){
    model.setScale(scale, scale, scale);
    
    
}

float Specimen::getScale(){
    ofVec3f scl = model.getScale();
    return scl.x;
}




void Specimen::rotateModel(ofVec3f rot){
    
    model.setRotation(0, rot.x, 1, 0, 0);
    model.setRotation(1, rot.y, 0, 1, 0);
    model.setRotation(2, rot.z, 0, 0, 1);
    currRot = rot;

}

void Specimen::positionModel(ofVec2f modelPosition){
    model.setPosition(modelPosition.x,modelPosition.y,0);
    currPosition = ofVec2f(modelPosition.x,modelPosition.y);
}

/*
void Specimen::setTextSize(int siz){
    bool diff =false;
    diff = regFade.changeSize(siz);
    dateFade.changeSize(siz);
    nameFade.changeSize(siz*1.5);
    if(diff){
        txtSize = siz ;
    }
}
*/

void Specimen::draw(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableDepthTest();
    
    model.update();
    /*
    if(drawTex){
        mesh.enableColors();
    }
    else{
        mesh.disableColors();
    }
     */ 
    
    if(drift){
        ofPushMatrix();
        perlinSeed += .0025f;
        int driftAmount =5;
        float rotationX = ofMap(ofNoise(perlinSeed,1), 0,1,-driftAmount,driftAmount);
        model.setRotation(0, currRot.x + rotationX, 1, 0, 0);
        //ofRotate(rotationX, 1, 0, 0);
        float rotationY = ofMap(ofNoise(perlinSeed,500), 0,1,-driftAmount*1.5,driftAmount*1.5);
        model.setRotation(1, currRot.y +rotationY, 0, 1, 0);
        //ofRotate(rotationY, 0, 1, 0);
        float rotationZ = ofMap(ofNoise(perlinSeed,5000), 0,1,-driftAmount,driftAmount);
        model.setRotation(2,currRot.z +rotationZ, 0, 0, 1);
        //ofRotate(rotationZ, 0, 0, 1);
        
        model.drawFaces();
        ofPushMatrix();
    }
    else{
        model.drawFaces();
    }
    mySound.setVolume(vol);
    //mesh.draw();
    
    
    if(isOpened){
        int timeDiff = ofGetElapsedTimeMillis() - timeOpened;
        if(!nameFade.isFadeIn & (timeDiff >= waitTime1)){
            nameFade.triggerStart();
            //oflog()<< "ping ping";
        }
       
        else if(!regFade.isFadeIn & (timeDiff >= waitTime2)){
            regFade.triggerStart();
        }
        else if(!dateFade.isFadeIn & (timeDiff >= waitTime3)){
            dateFade.triggerStart();
        }
    }
  
    
    regFade.update();
    dateFade.update();
    nameFade.update();
}


void Specimen::triggerStartText(){
    timeOpened = ofGetElapsedTimeMillis();
    isOpened = true;
    // nameFade.triggerStart();
}

void Specimen::load(){
    string bp = ofSystem("/usr/local/bin/SwitchAudioSource -s 'HDMI'");
    ofLog() << bp;
    mySound.load(filename +".mp3");
    mySound.setLoop(true);
    mySound.play();
    
    
}

void Specimen::unload(){
    //model.clear();
    //mesh.clear();
    mySound.unload(); 
    regFade.resetText();
    dateFade.resetText();
    nameFade.resetText();
    isOpened = false;
}


void Specimen::saveAnimalSettings(){
    float scl = getScale();
    ofxXmlSettings set;
    set.setValue("SCALE", scl);
    set.setValue("ROT:X", currRot.x);
    set.setValue("ROT:Y", currRot.y);
    set.setValue("ROT:Z", currRot.z);
    set.setValue("POS:X", currPosition.x);
    set.setValue("POS:Y", currPosition.y);

    set.saveFile(filename + ".xml");
}

void Specimen::drawText(bool isCentered, int x, int y){

    //nameFade.draw(x,y - nameFade.textWidth/2);
    //regFade.draw(x + nameFade.textHeight +17, y -regFade.textWidth/2);
    //dateFade.draw(x + nameFade.textHeight +19 + regFade.textHeight,y-dateFade.textWidth/2);
   

    nameFade.draw(x,y);
    regFade.draw(x + nameFade.textHeight +17, y );
    dateFade.draw(x + nameFade.textHeight +19 + regFade.textHeight,y);
    
}


