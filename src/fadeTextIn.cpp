//
//  fadeTextIn.cpp
//  textFadeIn
//
//  Created by Caroline Record on 12/28/15.
//
//

#include "fadeTextIn.h"

void fadeTextIn::setup(string text, int millis, int size, string fontFilename){
    
    fadeTargetDur = millis;
    ofTrueTypeFont fontSize;
    fontSize.loadFont(fontFilename, size);
    ofRectangle boxSize = fontSize.getStringBoundingBox(text, 0, 0);
    textWidth = boxSize.width;
    textHeight = boxSize.height;
    correctSize.allocate(textHeight,textWidth);
    
    font.loadFont(fontFilename, 100);
    ofRectangle box = font.getStringBoundingBox(text, 0, 0);
    padding = 20;
    int width = box.width+padding;
    int height = box.height+padding;
    drawTextin.allocate(2000, 2000, GL_RGB);
    tempPix.allocate(2000, 2000,OF_PIXELS_RGB);
    original.allocate( height, width);
    toBlur.allocate( height , width);
    drawTextin.begin();
    ofClear(0,0,0);
    ofBackground(0,0,0);
    peppersGhostThat(text, padding*2,0, font);
    drawTextin.end();

    drawTextin.readToPixels(tempPix);
    tempPix.crop(0, 0, height, width);
    original.setFromPixels(tempPix);

    fadeLevel =1;
    levelToBlur(fadeLevel);
    isFadeIn = false;
}

// ranges from 1-255
void fadeTextIn::levelToBlur(int blurLevel){
    
    toBlur = original;
    int iterations = int(ofMap(blurLevel,1, 255,10,1 ));
    for (int i=0; i < iterations; i++){
        toBlur.blur();
    }
    toBlur.threshold(255-blurLevel);
    toBlur.blur();
    correctSize.scaleIntoMe(toBlur);
}

void fadeTextIn::triggerStart(){
    timeFadeStarted =ofGetElapsedTimeMillis();
    isFadeIn = true;
}
    
void fadeTextIn::draw(int x, int y){
    
    correctSize.draw(x, y);
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

void fadeTextIn::peppersGhostThat(string text, int x, int y, ofTrueTypeFont font){
        ofPushMatrix();
        ofTranslate( x + font.getLineHeight()/2, y);
        ofRotate(-90, 0, 0, 1);
        ofScale(-1, 1,1);
        font.drawString(text,0, 0);
        ofPopMatrix();
}


void fadeTextIn::resetText(){
    isFadeIn = false;
    fadeLevel =1;
    levelToBlur(fadeLevel);
}