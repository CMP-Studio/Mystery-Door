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
    Boolean isClosed;
    float fadePercent; 
    
    void updateTarget();
    int uniqueID;
    
private:
    
    float noiseSeed;
    
    ofLight light;
    ofLight lightDouble;
    ofxLabel label;
    ofxPanel lightGui;
    ofxColorSlider colorAmb;
    ofxColorSlider colorSpec;
    ofxColorSlider colorDiff;
    
    ofxVec3Slider pos;
    ofxToggle drawLight;
    
    ofxFloatSlider minimumFlicker;
    ofxFloatSlider maximumFlicker;
    ofxFloatSlider speedOfFlicker;
    ofxFloatSlider speedOfFade;
    ofxToggle smoothFlicker;

    
    // this is the target it should reach so that there isn't a huge jump when it goes from fading to
    //flickering
    float targetBrightFadeToo;
    float brightFactor;
};
