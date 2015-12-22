#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    // set up for loading screen
    isLoadedScreen = false;
    isLoadingModels = false;
    
    // load in analystics
    analytics.loadFile(ofToDataPath("analytics.csv"));
 
    // So the 3D models work well in the lighting
    glEnable(GL_DEPTH_TEST);
    ofEnableSeparateSpecularLight();
    
    // set up lights
    bulb1.setup(1);
    //bulb2.setup(2);
    bulb1.dmx.connect("tty.usbserial-EN175083");
    
    
    //load things
    ofxLoadCamera(cam, "ofEasyCamSettings");
    font.loadFont("PT.ttf", 20);
    
    //gui setup
    gui.setup();
    gui.add(vol.setup("vol door open",.5,0,1));
    gui.add(volClosed.setup("vol door closed",.5,0,1));
    gui.add(textureDraw.setup("texture models", true));
    gui.add(drift.setup("drift", false));
    gui.add(allowCameraInput.setup("camera input", false));
    gui.add(realLightMin.setup("real light min: ",0,0,255));
    gui.add(realLightMax.setup("real light max: ",255,0,255));
    hideGui=true;
    gui.loadFromFile("settings.xml");
    
    //arduino
	ard.connect("/dev/cu.usbmodem1451", 57600);
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino	= false;
    
    // materials for the text and the model. Changes their coloring and how it is effected by the lights
    materialDrawText.setEmissiveColor(ofFloatColor(1,1,1));
    materialModel.setShininess(.8f);
    materialModel.setSpecularColor(ofFloatColor(.2f,.2f,.3f));
    materialModel.setEmissiveColor(ofFloatColor(0,0,0));
    
    // light so that the gui is always in full brightness
    guiLight.setAmbientColor(ofFloatColor(1,1,1));
}


void ofApp::loadModels(){
    // finds all files in the data folder with the ply extension.
    ofDirectory dir("");
    dir.allowExt("ply");
    dir.listDir();
    // populates a list of specimen based off the list of ply files
    for(int i=0; i<dir.size(); i++){
        Specimen temp = *new Specimen();
        temp.setup(split(dir.getName(i),'.').at(0));
        animals.push_back(temp);
    }
    
    animalPos =0;
    animals.at(animalPos).load();
    animals.at(0).vol = volClosed;
    animals.at(0).drawTex = textureDraw;
    isLoadingModels = true;
}


//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    if(isLoadingModels){
        updateArduino();
        if(!hideGui){
        animals.at(0).drawTex = textureDraw;
        animals.at(0).drift = drift;
        if(allowCameraInput){
            cam.enableMouseInput();
        }
        else{
            cam.disableMouseInput();
        }
            bulb1.dmxLightMin = realLightMin;
            bulb1.dmxLightMax = realLightMax;
        }
        bulb1.updateLights();
        bulb1.dmx.update(); 
        //bulb2.updateLights();
    }
    else{
    }
    
    
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    

    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
}

//--------------------------------------------------------------


void ofApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
}


void ofApp::digitalPinChanged(const int & pinNum) {
    // do something with the digital input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    ofLog() << "digital pin: " + ofToString(pinNum) + " = " + ofToString(ard.getDigital(pinNum));
    // 0 is opened
    if(ard.getDigital(pinNum) ==0){
        doorOpened();
    }
    // 1 is closed
    else if (ard.getDigital(pinNum) ==1){
        doorClosed();
        
    }
    
}


void ofApp::doorOpened(){
    bulb1.fadePercent =0;
    bulb1.isFadeOn = true;
    //bulb2.fadePercent =0;
    //bulb2.isFadeOn = true;
    animals.at(0).vol = vol;
}


void ofApp::doorClosed(){
    animals.at(animalPos).unload();
    int row = analytics.numRows;
    analytics.setString(row, 0, "closed");
    analytics.setString(row, 1, ofGetTimestampString());
    analytics.setString(row, 2, animals.at(animalPos).commonName);
    //analytics.saveFile(ofToDataPath("analytics.csv"));
    
    if(animalPos < animals.size()-1){
        animalPos++;
    }
    else{
        animalPos =0;
    }
    animals.at(animalPos).load();
    int row2 = analytics.numRows;
    analytics.setString(row2, 0, "opened");
    analytics.setString(row2, 1, ofGetTimestampString());
    analytics.setString(row2, 2, animals.at(animalPos).commonName);
    analytics.saveFile(ofToDataPath("analytics.csv"));
    animals.at(0).vol = volClosed;
    
}

void ofApp::draw(){
    
    ofBackground(0);
    if(isLoadingModels){
    
        
        bulb1.enable();
        //bulb2.enable();
    cam.begin();
    materialModel.begin();
        animals.at(animalPos).draw(); 
    materialModel.end();
    cam.end();
    
    materialDrawText.begin();
    drawText(animals.at(animalPos).commonName +"\n" +animals.at(animalPos).region +"\n" + ofToString(animals.at(animalPos).date)  , 30,  30, false);
    materialDrawText.end();
        bulb1.disable();
        //bulb2.disable();
        
    if(!hideGui){
        guiLight.enable();
        ofDisableDepthTest();
        ofDisableSeparateSpecularLight();
        gui.draw();
        bulb1.drawGui();
        //bulb2.drawGui();
        ofEnableSeparateSpecularLight();
        ofEnableDepthTest();
        guiLight.disable();
    }
    
    
    ofEllipse(0,0,30,30);
    }
    else if (!isLoadedScreen) {
        drawText("loading models... Please wait", ofGetWidth()/2,  ofGetHeight()/2, true);
        isLoadedScreen = true;
    }
    else{
        drawText("loading models... Please wait", ofGetWidth()/2,  ofGetHeight()/2, false);
        loadModels();
    }
    
  
}

void ofApp::drawText(string text, int x, int y, bool isCentered){
    if(isCentered){
        ofPushMatrix();
        ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);
        ofTranslate( x , y- bounds.width/2);
        ofRotate(-90, 0, 0, 1);
        ofScale(-1, 1,1);
        font.drawString(text,0, 0);
        ofPopMatrix();
    }
    else{
        ofPushMatrix();
        ofTranslate( x + font.getLineHeight()/2, y);
        ofRotate(-90, 0, 0, 1);
        ofScale(-1, 1,1);
        font.drawString(text,0, 0);
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   
    if (key =='s'){
        doorClosed();
        doorOpened();
    }
    if (key =='h'){
        hideGui = !hideGui;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}



//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit(){
    ofxSaveCamera(cam, "ofEasyCamSettings");
    gui.saveToFile("settings.xml");
    bulb1.saveGui();
    //bulb2.saveGui();
}

vector<string> ofApp::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
