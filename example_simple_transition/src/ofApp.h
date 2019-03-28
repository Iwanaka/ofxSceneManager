#pragma once

#include "ofMain.h"
#include "ofxSceneManager.h"

class Page : public ofxScene
{
public:
	Page() {}
	Page(string index, ofColor c) {
		
		this->index = index;
		this->c = c;

	}
	~Page() {

		cout << "called " << index << " destract" << endl;

	};

	void set(string index, ofColor c) {
		
		this->index = index;
		this->c = c;

	}

	void update() override {

		cout << "called " << index << " update" << endl;

	}

	void draw() override {

		ofPushStyle();
		ofSetColor(c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(0);
		ofDrawBitmapString(index, 50, 50);
		ofPopStyle();

	}

private:

	string index;
	ofColor c;

};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
		
		ofxSceneManager scenes;

};
