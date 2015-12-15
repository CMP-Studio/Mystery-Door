#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    analytics.loadFile(ofToDataPath("analytics.csv"));
    //csv.loadFile(ofToDataPath(filename + ".csv"));
    
    // finds all files in the data folder with the ply extension.
    ofDirectory dir("");
    dir.allowExt("ply");
    dir.listDir();
    
    
    isLoaded = false;
    
    // populates a list of specimen based off the list of ply files
    for(int i=0; i<dir.size(); i++){
        Specimen temp = *new Specimen();
        temp.setup(split(dir.getName(i),'.').at(0));
        animals.push_back(temp);
    }
    
    animalPos =0;
    animals.at(animalPos).load();
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    //ofEnableDepthTest();
    glEnable(GL_DEPTH_TEST);
    light.setPointLight();
    light.setAmbientColor(ofFloatColor(0,0,1));
    light.enable();
    
    
    ofEnableSeparateSpecularLight();

    ofxLoadCamera(cam, "ofEasyCamSettings");
    cam.enableMouseInput();
    
    //serial.setup();

    gui.setup();
    gui.add(vol.setup("volume",.5,0,1));
    gui.add(color.setup("color ambient", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.setDefaultTextColor(ofColor(255));
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

}



//--------------------------------------------------------------
void ofApp::update(){
    
    updateArduino();
    
    ofColor lights = color;
    //ofLog()<< int(lights.r)/255;
    light.setDiffuseColor(ofFloatColor(float(lights.r)/255.f, float(lights.g)/255.f, float(lights.b)/255.f));
    ofSetColor(lights);
    animals.at(0).vol = vol;
    
    
    /*
    int myByte = serial.readByte();
    if ( myByte == OF_SERIAL_NO_DATA )
        printf("no data was read");
    else if ( myByte == OF_SERIAL_ERROR )
        printf("an error occurred");
    else
        printf("%d \n", myByte);
     */
    
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
        
    }
    // 1 is closed
    else if (ard.getDigital(pinNum) ==1){
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
    }
    
}

void ofApp::draw(){

    
    
    ofEnableDepthTest();
    cam.begin();
    animals.at(animalPos).draw();
    cam.end();
    ofDisableDepthTest();
    
    if(!hideGui){
        gui.draw();
    }
    
    
    ofEllipse(0,0,30,30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   
    if (key =='s'){
    
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
