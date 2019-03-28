#pragma once

#include "ofMain.h"
#include "ofxSceneManager.h"
#include "Page.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
		
		ofxSceneManager scenes;
};
