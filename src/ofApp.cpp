#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    analytics.loadFile(ofToDataPath("analytics.csv"));
    //csv.loadFile(ofToDataPath(filename + ".csv"));
    

    
    
    isLoadedScreen = false;
    isLoadingModels = false;
    

    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    //ofEnableDepthTest();
    glEnable(GL_DEPTH_TEST);
    //light.setSpotlight();
    //light.setSpotlightCutOff(.2f);
    //light.setSpotConcentration(.8f);
    light = *new Lights();
    light.setup();
    
    ofEnableSeparateSpecularLight();

    ofxLoadCamera(cam, "ofEasyCamSettings");
    cam.enableMouseInput();
    
    //serial.setup();
    
    
    
    gui.setup();
        
    gui.add(vol.setup("vol door open",.5,0,1));
    gui.add(volClosed.setup("vol door closed",.5,0,1));
    gui.add(textureDraw.setup("texture models", true));
    gui.add(drift.setup("drift", false));
    gui.add(allowCameraInput.setup("camera input", false));
    
    
    
    hideGui=false;
    
    
    //arduino
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/cu.usbmodem1451", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)

    gui.loadFromFile("settings.xml");
    
    font.loadFont("PT.ttf", 20);
    
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
        animals.at(0).drawTex = textureDraw;
        animals.at(0).drift = drift;
        if(allowCameraInput){
            cam.enableMouseInput();
        }
        else{
            cam.disableMouseInput();
        }
    
    
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

    if(isLoadingModels){
    
    
    cam.begin();
    animals.at(animalPos).draw();
    cam.end();
    
    
    if(!hideGui){
        ofDisableDepthTest();
        ofDisableSeparateSpecularLight();
        gui.draw();
        light.drawGui(); 
        ofEnableSeparateSpecularLight();
        ofEnableDepthTest();
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
        ofTranslate( x, y);
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
