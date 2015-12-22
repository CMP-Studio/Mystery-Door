#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Specimen.h"
#include "ofxCameraSaveLoad.h"
#include "Lights.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void loadModels(); 
    void exit();
    void drawText(string text, int x, int y, bool isCentered);
    
    void doorOpened();
    void doorClosed();
    
    void textureChanged(); 
    
    vector<string> split(const std::string &s, char delim);
    
    vector<Specimen> animals;
    int animalPos; 
    ofEasyCam cam;
    //ofLight light;
    ofxCsv analytics;
   
    ofSerial serial;
    
    ofxPanel gui;
    
    ofxFloatSlider vol;
    ofxFloatSlider volClosed;
    ofxToggle textureDraw;
    ofxToggle drift;
    ofxToggle allowCameraInput;
    
    bool hideGui;
    
    ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    
    bool isLoadedScreen;
    bool isLoadingModels;
    
    ofTrueTypeFont font;
    
    Lights bulb1;
    Lights bulb2;
    
    ofLight guiLight;
    ofMaterial materialDrawText;
    ofMaterial materialDrawGui;
    ofMaterial materialModel;
    ofxIntSlider realLightMin;
    ofxIntSlider realLightMax;
    
    
    
private:
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void updateArduino();

};
