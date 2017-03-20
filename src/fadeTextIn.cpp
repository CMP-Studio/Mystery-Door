//
//  fadeTextIn.cpp
//  textFadeIn
//
//  Created by Caroline Record on 12/28/15.
//
//

#include "fadeTextIn.h"

void fadeTextIn::setup(string text, int millis, int size, string fontFilename){
    
    brightenUp.setEmissiveColor(ofFloatColor(1,1,1));
    
    fontFilename = fontFilename;
    txt = text;
    
    padding = 1.4;
    float paddingY = 1.2;
    //oflog()<< fontFilename;
    fadeTargetDur = millis;
    
    ofTrueTypeFont fontSize;
    fontSize.setLetterSpacing(1.2);
    siz = size;
    fontSize.load(fontFilename, siz);
    ofRectangle boxSize = fontSize.getStringBoundingBox(text, 0, 0);
    textWidth = boxSize.width*paddingY;
    textHeight = boxSize.height*padding;
    correctSize.allocate(textHeight,textWidth);
    testImage.allocate(textHeight,textWidth);
    
    ofTrueTypeFont fnt;

    fnt.setLetterSpacing(1.2);
    //oflog()<< "yipee";
    //oflog()<< fontFilename;
    fnt.load(fontFilename, 100);
    ofRectangle box = fnt.getStringBoundingBox(text, 0, 0);
    
    
    //int width = box.width*paddingY;
    //int height = box.height*padding;
    int width = box.width*paddingY;
    int height = box.height*padding;
    drawTextin.allocate(3000, 3000, GL_RGB);
    tempPix.allocate(3000, 3000,OF_PIXELS_RGB);
    original.allocate( height, width);
    toBlur.allocate( height , width);
    //oflog()<< "what";
    drawTextin.begin();
    ofClear(0);
    ofBackground(0);
    brightenUp.begin();
    int padPix= (padding -1)*box.height;
    ofPushMatrix();
    ofTranslate( padPix + fnt.getLineHeight()/2, 0);
    ofRotate(-90, 0, 0, 1);
    ofScale(-1, 1,1);
    
    fnt.drawString(text,0, 0);
    ofPopMatrix();
    brightenUp.end();
    drawTextin.end();
    
    drawTextin.readToPixels(tempPix);
    tempPix.crop(0, 0, height, width);
    original.setFromPixels(tempPix);
    
    fadeLevel =1;
    levelToBlur(fadeLevel);
    isFadeIn = false;
    ////oflog()<< "is this life";
    
}


void fadeTextIn::peppersGhostThat(string text, int x, int y, ofTrueTypeFont ft){
    
    ofPushMatrix();
    //ofTranslate( x/2 + ft.getLineHeight()/2, y);
    ofTranslate(x, y);
    ofRotate(-90, 0, 0, 1);
    ofScale(-1, 1,1);
    //ofSetColor(255);
    ft.drawString(text,0, 0);
    ofPopMatrix();
}

// ranges from 1-255
void fadeTextIn::levelToBlur(int blurLevel){
    
    toBlur = original;

    int iterations = int(ofMap(blurLevel,1, 255,24,1 ));
    for (int i=0; i < iterations; i++){
        toBlur.blur();
    }
    toBlur.threshold(255-blurLevel);
    toBlur.blur();
  
    correctSize.scaleIntoMe(toBlur);
    //correctSize.invert();
    testImage = correctSize;
}

/*
 bool fadeTextIn::changeSize(int newSize){
 if(newSize != siz){
 
 ofTrueTypeFont fontSize;
 fontSize.loadFont(fntFilename, siz);
 ofRectangle boxSize = fontSize.getStringBoundingBox(txt, 0, 0);
 correctSize.resize(boxSize.height, boxSize.width);
 siz = newSize;
 return true;
 }
 else{
 return false;
 }
 }
 */

void fadeTextIn::triggerStart(){
    timeFadeStarted =ofGetElapsedTimeMillis();
    isFadeIn = true;
}

void fadeTextIn::draw(int x, int y){
    //int h = testImage.height/2;
    testImage.draw(x, y );
}

void fadeTextIn::update(){
    
    if(isFadeIn){
        int timeSincestart = ofGetElapsedTimeMillis() - timeFadeStarted;
        if (timeSincestart <= fadeTargetDur){
            int newFadeLev = int(ofMap(timeSincestart, 0, fadeTargetDur, 1, 255));
            if (newFadeLev != fadeLevel){
                levelToBlur(newFadeLev);
                fadeLevel = newFadeLev;
            }
        }
    }
}




void fadeTextIn::resetText(){
    isFadeIn = false;
    fadeLevel =1;
    levelToBlur(fadeLevel);
}