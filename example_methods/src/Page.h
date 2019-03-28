#pragma once
#include "BasePage.h"

class Page : public BasePage
{
public:

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

	void start() override { 
		cout << "called " << index << " start" << endl; 
	};
	void behindUpdate() override { 
		//cout << "called " << index << " behindupdate" << endl; 
	};
	void update() override {
		//cout << "called " << index << " update" << endl;
	};

	void draw() override {
		//cout << "called " << index << " draw" << endl;
		ofPushStyle();
		ofSetColor(c);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		ofSetColor(0);
		ofDrawBitmapString(index, 50, 50);
		ofPopStyle();
	};

	void keyPressed(int key) override { cout << index << " : KeyPressed " << endl; }
	void keyReleased(int key) override { cout << index << " : KeyReleased" << endl; }
	void mouseMoved(int x, int y) override { cout << index << " : mouseMoved" << endl; }
	void mouseDragged(int x, int y, int button) override { cout << index << " : mouseDragged" << endl; }
	void mousePressed(int x, int y, int button) override { cout << index << " : mousePressed" << endl; }
	void mouseReleased(int x, int y, int button) override { cout << index << " : mouseReleased" << endl; }
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override { cout << index << " : mouseScrolled" << endl; }
	void mouseEntered(int x, int y) override { cout << index << " : mouseEntered" << endl; }
	void mouseExited(int x, int y) override { cout << index << " : mouseExited" << endl; }
	void windowResized(int w, int h) override { cout << index << " : windowResized" << endl; }
	void dragEvent(ofDragInfo dragInfo) override { cout << index << " : dragEvent" << endl; }
	void gotMessage(ofMessage msg) override { cout << index << " : gotMessage" << endl; }

	void willDrawIn() override { cout << index << " : willDrawIn" << endl; };
	void willDraw() override { cout << index << " : willDraw" << endl; };
	void willDrawOut() override { cout << index << " : willDrawOut" << endl; };
	void willExit() override { cout << index << " : willExit" << endl; };

	void startDrawIn(bool &b) override { cout << index << " : startDrawIn" << endl; };
	void startDrawing(bool &b) override { cout << index << " : startDrawing" << endl; };
	void finishedDrawing(bool &b) override { cout << index << " : finishedDrawing" << endl; };
	void startDrawOut(bool &b) override { cout << index << " : startDrawOut" << endl; };
	void finishScene(bool &b) override { cout << index << " : finishScene" << endl; };

private:

	string index;
	ofColor c;

};