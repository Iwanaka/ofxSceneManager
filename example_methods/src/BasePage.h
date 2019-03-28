#pragma once
#include "ofxScene.h"

class BasePage : public ofxScene
{
public:
	BasePage() {
		ofAddListener(startDrawInEvent, this, &BasePage::startDrawIn);
		ofAddListener(startDrawingEvent, this, &BasePage::startDrawing);
		ofAddListener(finishedDrawingEvent, this, &BasePage::finishedDrawing);
		ofAddListener(startDrawOutEvent, this, &BasePage::startDrawOut);
		ofAddListener(finishSceneEvent, this, &BasePage::finishScene);
	}

	~BasePage() {}

#pragma basic methods
	virtual void start() override {};
	virtual void behindUpdate() override {};// always called on scenes running
	virtual void update() override {};
	virtual void draw() override {};


#pragma openframewoeks basic events
	virtual void keyPressed(int key) override {}
	virtual void keyReleased(int key) override {}
	virtual void mouseMoved(int x, int y) override {}
	virtual void mouseDragged(int x, int y, int button) override {}
	virtual void mousePressed(int x, int y, int button) override {}
	virtual void mouseReleased(int x, int y, int button) override {}
	virtual void mouseScrolled(int x, int y, float scrollX, float scrollY) override {}
	virtual void mouseEntered(int x, int y) override {}
	virtual void mouseExited(int x, int y) override {}
	virtual void windowResized(int w, int h) override {}
	virtual void dragEvent(ofDragInfo dragInfo) override {}
	virtual void gotMessage(ofMessage msg) override {}

private:

#pragma called on transition
	virtual void willDrawIn() override {};
	virtual void willDraw() override {};
	virtual void willDrawOut() override {};
	virtual void willExit() override {};

#pragma called on transition by ofEvent
	virtual void startDrawIn(bool &b) {};
	virtual void startDrawing(bool &b) {};
	virtual void finishedDrawing(bool &b) {};
	virtual void startDrawOut(bool &b) {};
	virtual void finishScene(bool &b) {};

};