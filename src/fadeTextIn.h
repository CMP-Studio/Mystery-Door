//
//  fadeTextIn.h
//  textFadeIn
//
//  Created by Caroline Record on 12/28/15.
//
//

#include "ofMain.h"
#include "ofxOpenCv.h"


class fadeTextIn{
public:
    void setup(string text, int millis, int size, string fontFilename);
    void triggerStart();
    void update();
    void draw(int x, int y);
    void resetText();
    int textHeight;
    int textWidth;
    bool isFadeIn;
    
    
private:

    ofxCvColorImage original;
    ofxCvGrayscaleImage toBlur;
    ofxCvGrayscaleImage correctSize;
    ofxCvColorImage testImage;
    
    ofFbo drawTextin;
    ofPixels tempPix;
    float padding;
    
    int fadeTargetDur;
    int fadeLevel;
    int siz;
    int timeFadeStarted;
    
    void levelToBlur(int blurLevel);
    void peppersGhostThat(string text, int x, int y, ofTrueTypeFont font);
    
    string fntFilename;
    string txt;
    
    ofMaterial brightenUp; 
};


