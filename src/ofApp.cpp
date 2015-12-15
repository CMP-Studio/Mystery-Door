#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    analytics.loadFile(ofToDataPath("analytics.csv"));
    //csv.loadFile(ofToDataPath(filename + ".csv"));
    
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
    
    //glShadeModel(GL_SMOOTH);
    //glEnable(GL_NORMALIZE);
    ofEnableDepthTest();
    //glEnable(GL_DEPTH_TEST);
    //light.setAmbientColor(ofFloatColor(0,0,1));
    light.enable();
    //ofEnableSeparateSpecularLight();

    ofxLoadCamera(cam, "ofEasyCamSettings");
    cam.enableMouseInput();
    
    serial.setup();
    
    gui.setup();
    gui.add(vol.setup("volume",.5,0,1));
    gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.setDefaultTextColor(ofColor(255));
    hideGui=false;
}

//--------------------------------------------------------------
void ofApp::update(){
    int myByte = serial.readByte();
    if ( myByte == OF_SERIAL_NO_DATA )
        printf("no data was read");
    else if ( myByte == OF_SERIAL_ERROR )
        printf("an error occurred");
    else
        printf("%d \n", myByte);
}

//--------------------------------------------------------------
void ofApp::draw(){
    /*
    light.enable();
    ofEnableDepthTest();
    cam.end();
    light.disable();
    ofDisableDepthTest();
    */
    
    
    ofEnableDepthTest();
    cam.begin();
    animals.at(animalPos).draw();
    cam.end();
    ofDisableDepthTest();
    
    if(!hideGui){
        gui.draw();
    }
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
