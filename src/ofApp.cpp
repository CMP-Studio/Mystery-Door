#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
  
    
    string doop = "hello world";
    ofLog() << doop.size();
    ofLog() << doop[2];
    
    // set up for loading screen
    isLoadedScreen = false;
    isLoadingModels = false;
    //ofGetElapsedTimeMillis()
    // load in analystics
    analytics.loadFile(ofToDataPath("analytics.csv"));
    
    ofDisableArbTex();
    // So the 3D models work well in the lighting
    glEnable(GL_DEPTH_TEST);
    ofEnableSeparateSpecularLight();
   
    // set up lights
    bulb1.setup(1);
    bulb2.setup(2);
    bulb1.dmx.connect("tty.usbserial-EN175083");
    
    
    //load things
    ofxLoadCamera(cam, "ofEasyCamSettings");
    font.load("PC.ttf", 20);
    //cam.setTarget(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
    //gui setup
    ofSetFullscreen(true);
    
    //rotationOfModel.list
    gui.setup();
    gui.add(vol.setup("vol_door_open",.5,0,1));
    gui.add(volClosed.setup("vol_door_closed",.5,0,1));
    gui.add(textureDraw.setup("texture_models", true));
    gui.add(drift.setup("drift", false));
    gui.add(allowCameraInput.setup("camera_input", false));
    gui.add(realLightMin.setup("real_light_min",0,0,255));
    gui.add(realLightMax.setup("real_light_max",255,0,255));
    gui.add(posOfText.setup("position of the text",ofVec2f(20,20),ofVec2f(0,0),ofVec2f(ofGetHeight()*2,ofGetWidth()*2)));
    gui.add(widthHeightOfText.setup("width height of text",ofVec2f(20,20),ofVec2f(0,0),ofVec2f(ofGetHeight(),ofGetWidth())));
    gui.add(rotationOfModel.set("Rotation of Model", ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(360,360,360)));
    gui.add(scaleOfModel.set("Scale of Model", 1, 0, 10));
    gui.add(posOfModel.set("Pos of Model", ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ofVec2f(0,0), ofVec2f(ofGetWidth(),ofGetHeight()*1.5)));
    hideGui=true;
    gui.loadFromFile("settings.xml");
    rotationOfModel.addListener(this, &ofApp::rotationOfModelChanged);
    scaleOfModel.addListener(this, &ofApp::scaleOfModelChanged);
    posOfModel.addListener(this, &ofApp::posOfModelChanged);
    //arduino
    
	ard.connect("/dev/cu.usbmodem1461", 57600);
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino	= false;
    
    // materials for the text and the model. Changes their coloring and how it is effected by the lights
    materialDrawText.setEmissiveColor(ofFloatColor(1,1,1));
    
    
    
    materialModel.setShininess(.8f);
    materialModel.setSpecularColor(ofFloatColor(.2f,.2f,.3f));
    materialModel.setEmissiveColor(ofFloatColor(0,0,0));
    
    // light so that the gui is always in full brightness
    guiLight.setAmbientColor(ofFloatColor(1,1,1));
    
    waitToFadeText = 1500;
    startWaitToFadeText = 0;
    
    fadeLabelPercent =0;
    
    ofHideCursor();
    
    
}

void ofApp::populateVector(){
    int size = animals.size();
    for(int i =0; i< size; i++){
        randomIndices.push_back(i);
    }
}

void ofApp::rotationOfModelChanged(ofVec3f & rotation){
    ofLog() << rotation;
    animals.at(animalPos).rotateModel(rotation); 
    
}

void ofApp::scaleOfModelChanged(float & scale){
    animals.at(animalPos).scaleModel(scale);
}

void ofApp::posOfModelChanged(ofVec2f & pos){
    animals.at(animalPos).positionModel(pos);
}


void ofApp::loadModels(){
    // finds all files in the data folder with the ply extension.
    ofDirectory dir("");
    dir.allowExt("dae");
    dir.listDir();
    ////oflog() << ofToString(dir.size());
    //oflog() << "rightHere";

    // populates a list of specimen based off the list of ply files
    for(int i=0; i<dir.size(); i++){
        Specimen temp = *new Specimen();
        temp.setup(split(dir.getName(i),'.').at(0));
        animals.push_back(temp);
    }
    
    if(animals.size()>0){
    animalPos =0;
    animals.at(animalPos).load();
    animals.at(0).vol = volClosed;
    animals.at(0).drawTex = textureDraw;
    animals.at(0).drift = drift;
        if(allowCameraInput){
            cam.enableMouseInput();
        }
        else{
            cam.disableMouseInput();
        }
    }
    isLoadingModels = true;
    populateVector();
    
    float currentScale = animals.at(animalPos).getScale();
    scaleOfModel.set(currentScale);
    ofVec3f rot = animals.at(animalPos).currRot;
    rotationOfModel.set(rot);
    doorOpened();
    bulb1.dmxLightMin = realLightMin;
    bulb1.dmxLightMax = realLightMax;
    
    
    
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    //toFade.update();
    if(ofGetElapsedTimeMillis()%(100*60*8) <= 30){
        ofHttpResponse resp = ofLoadURL("http://studiostatus.carnegiemuseums.org/ping/mystery-door");
    }
   
    
    if(isLoadingModels){
        updateArduino();
        if(!hideGui){
            ofShowCursor();
        //animals.at(0).drawTex = textureDraw;
        animals.at(animalPos).drift = drift;
        if(allowCameraInput){
            cam.enableMouseInput();
        }
        else{
            cam.disableMouseInput();
        }
            bulb1.dmxLightMin = realLightMin;
            bulb1.dmxLightMax = realLightMax;
        }
        else{
            ofHideCursor();
        }

        bulb1.updateLights();
        bulb2.updateLights();
        bulb1.dmx.update();
        fadeLabelUp();
        
       
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
    //oflogNotice() << ard.getFirmwareName();
    //oflogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    // set pins D2 and A5 to digital input
    ard.sendDigitalPinMode(2, ARD_INPUT);
	
    // Listen for changes on the digital and analog pins
    ofAddListener(ard.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofLog()<<"setup arduino!";
    
}

//--------------------------------------------------------------


void ofApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
}


void ofApp::digitalPinChanged(const int & pinNum) {

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
    
    startWaitToFadeText = ofGetElapsedTimeMillis();
    
    animals.at(animalPos).vol = vol;
    animals.at(animalPos).triggerStartText();
    bulb1.isClosed = false;
}


void ofApp::doorClosed(){
    fadeLabelPercent =0;
    animals.at(animalPos).unload();
    int row = analytics.numRows;
    analytics.setString(row, 0, "closed");
    analytics.setString(row, 1, ofGetTimestampString());
    analytics.setString(row, 2, animals.at(animalPos).commonName);
    //analytics.saveFile(ofToDataPath("analytics.csv"));
    
   // animalPos = int(round(ofRandom(-.5,animals.size()-.5)));
    
    /*
    if(animalPos < animals.size()-1){
        animalPos++;
    }
    else{
        animalPos =0;
    }
     */
    
    
    if(randomIndices.size() > 0){
        int num = ofRandom(randomIndices.size()-1);
        animalPos = randomIndices.at(num);
        randomIndices.erase(randomIndices.begin() + num);
    }
    else{
        populateVector();
        int num = ofRandom(randomIndices.size()-1);
        animalPos = randomIndices.at(num);
        randomIndices.erase(randomIndices.begin() + num);
    }
    
    animals.at(animalPos).load();
    
    // change the gui to the correct position..
    float currentScale = animals.at(animalPos).getScale();
    scaleOfModel.set(currentScale);
    ofVec3f rot = animals.at(animalPos).currRot;
    rotationOfModel.set(rot);
    ofVec2f pos = animals.at(animalPos).currPosition;
    posOfModel = pos; 
    
    bulb1.fadePercent =0;
    bulb1.isClosed = true;
    bulb1.updateTarget(); 
    
    //bulb2.fadePercent =0;
    //bulb2.isClosed = true;
    
    int row2 = analytics.numRows;
    analytics.setString(row2, 0, "opened");
    analytics.setString(row2, 1, ofGetTimestampString());
    analytics.setString(row2, 2, animals.at(animalPos).commonName);
    analytics.saveFile(ofToDataPath("analytics.csv"));
    animals.at(animalPos).vol = volClosed;
}

void ofApp::draw(){
    
    ofBackground(0);
    //materialDrawText.begin();
    //toFade.draw(0,0);
    //materialDrawText.end();

    if(isLoadingModels){
    
     
        
     bulb1.enable();
    cam.begin();
        
        //light.enable();
        ofPushMatrix();
        animals.at(animalPos).draw();
        ofPopMatrix(); 
        //light.disable();
        
    cam.end();
        bulb1.disable();
        
       
        
        

    if(!hideGui){
        
        ofDisableLighting();
        ofDisableDepthTest();
        ofDisableSeparateSpecularLight();
        gui.draw();
        bulb1.drawGui();
        bulb2.drawGui();
        ofEnableSeparateSpecularLight();
        ofEnableDepthTest();
        //guiLight.disable();
        ofEnableLighting();
        
    }
        
        bulb2.enable();
        //materialDrawText.begin();
        ofDisableDepthTest();
        //museum label
        //ofSetColor(0);
        //ofSetColor(fadeLabelPercent*255, fadeLabelPercent*255, fadeLabelPercent*255);
        //ofSetColor(255,0,0);
        
        //ofDrawRectangle(posOfText->x-20, posOfText->y-20,widthHeightOfText->x, widthHeightOfText->y);
        //ofSetColor(255);
        animals.at(animalPos).drawText(false, posOfText->x, posOfText->y);
        
        //materialDrawText.end();
        ofEnableDepthTest();
        
        bulb2.disable();
    
    //ofDrawEllipse(0,0,30,30);
        
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
   
    //toFade.triggerStart();
    if (key =='s'){
        doorClosed();
    }
    if (key =='d'){
        doorOpened();
    }
    if (key =='h'){
        hideGui = !hideGui;
    }
    
}

void ofApp::fadeLabelUp(){
    
    if ((fadeLabelPercent < 1)& !bulb1.isClosed ){
        ////oflog()<< "hey " <<fadeLabelPercent;
        fadeLabelPercent += .01;
    }
    //materialDrawText.setEmissiveColor(ofFloatColor(fadeLabelPercent,fadeLabelPercent,fadeLabelPercent));
    //materialDrawText.setEmissiveColor(ofFloatColor(0,0,0));
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 't'){
        animals.at(animalPos).saveAnimalSettings(); 
        
    }
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
    bulb2.saveGui();
    bulb1.dmx.setLevel(bulb1.uniqueID, 0);
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
