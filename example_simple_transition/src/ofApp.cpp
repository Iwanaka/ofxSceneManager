#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);

	//create scenes
	shared_ptr<Page> a = make_shared<Page>("A", ofColor::red);
	shared_ptr<Page> b = make_shared<Page>("B", ofColor::green);
	shared_ptr<Page> c = make_shared<Page>("C", ofColor::yellow);

	//add scenes to management class
	scenes.addScene(a);
	scenes.addScene(b);
	scenes.addScene(c);

	//select transition mode
	scenes.setTransitionMode(ofxSceneTransition::FADE);

	//set transition time, 0:drawin sec, 1:drawout sec
	scenes.setSceneDuration(1.0f, 1.0f);

	//set screen size
	scenes.setup(ofGetWidth(), ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::update(){

	scenes.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

	scenes.draw();

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){


	//call scene
	if (key == 'a') scenes.changeScene(0);
	if (key == 'b') scenes.changeScene(1);
	if (key == 'c') scenes.changeScene(2);

}