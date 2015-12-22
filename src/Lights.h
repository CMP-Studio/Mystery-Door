//
//  lights.h
//  mini_Door
//
//  Created by Caroline Record on 12/16/15.
//
//
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDmx.h"

//#include "ofxGui.h"
class Lights {
    
public:
    static ofxDmx dmx;
    static int dmxLightMin;
    static int dmxLightMax;
    
    void setup(int ID);
    void updateLights();
    void drawGui();
    void fadeUp();
    void fadeDown();
    void flicker();
    void enable();
    void disable();
    void saveGui();
    Boolean isFlicker;
    Boolean isFadeOn;
    float fadePercent; 
    
private:
    float noiseSeed;
    
    int uniqueID; 
    
    ofLight light;

    ofxLabel label; 
    ofxPanel lightGui;

    ofxColorSlider colorAmb;
    ofxColorSlider colorSpec;
    ofxColorSlider colorDiff;
    
    ofxIntSlider posX;
    ofxIntSlider posY;
    ofxIntSlider posZ;
    
    ofxToggle drawLight;
    
    ofxFloatSlider minimumFlicker;
    ofxFloatSlider maximumFlicker;
    ofxFloatSlider speedOfFlicker;
    ofxFloatSlider speedOfFade;
    ofxToggle smoothFlicker;
};
