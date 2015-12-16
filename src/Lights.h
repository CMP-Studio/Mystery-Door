//
//  lights.h
//  mini_Door
//
//  Created by Caroline Record on 12/16/15.
//
//
#include "ofMain.h"
#include "ofxGui.h"

//#include "ofxGui.h"
class Lights {
    
public:
    void setup();
    void updateLights();
    void drawGui();
    void fadeUp();
    void fadeDown();
    void flicker();
    void enable();
    void disable();
    
private:
    ofLight light;
    ofxPanel lightGui;
    
    
    ofxColorSlider colorAmb;
    ofxColorSlider colorSpec;
    ofxColorSlider colorDiff;
    
    ofxIntSlider posX;
    ofxIntSlider posY;
    ofxIntSlider posZ;
    
};
