//
//  ofxScene.h
//  ofxSceneManager
//
//  Created by Koki Nomura on 2014/01/22.
//
//

#pragma once
#include "ofMain.h"
#include "ofxEasing.h"


#pragma mark - transition modes
typedef enum {
	SWITCH,
	FADE,
	SLIDEIN_LEFT,
	SLIDEIN_RIGHT,
	SLIDEIN_TOP,
	SLIDEIN_BOTTOM
} ofxSceneTransition;



class ofxScene {
public:

	ofxScene();
	~ofxScene();

#pragma mark - Basics
	void startScene(shared_ptr<ofxScene> prevScene, ofxSceneTransition t);
	void behindUpdateScene();
	void updateScene();
	void drawScene();
	void exitScene(ofxSceneTransition t);


#pragma mark - Status
	bool isDrawIn();
	bool isDrawing();
	bool isDrawOut();
	bool isFinished();


#pragma mark - Duration
	void setSceneDuration(float drawInSec, float drawOutSec);
	void setSceneDuration(float drawInSec, float drawingSec, float drawOutSec);
	void setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec);
	void setSceneDelayDuration(float drawDelaySec);
	float getDrawInDuration();
	float getDrawingDuration();
	float getDrawOutDuration();
	float getDrawDelayDuration();


#pragma mark - Etc
	void setExitByTime(bool b);
	float getSceneAlpha();
	float getScenePosX();
	float getScenePosY();
	glm::vec2 getScenePos();


#pragma mark - Events
	virtual void keyPressed(int key) {}
	virtual void keyReleased(int key) {}
	virtual void mouseMoved(int x, int y) {}
	virtual void mouseDragged(int x, int y, int button) {}
	virtual void mousePressed(int x, int y, int button) {}
	virtual void mouseReleased(int x, int y, int button) {}
	virtual void mouseScrolled(int x, int y, float scrollX, float scrollY) {}
	virtual void mouseEntered(int x, int y) {}
	virtual void mouseExited(int x, int y) {}
	virtual void windowResized(int w, int h) {}
	virtual void dragEvent(ofDragInfo dragInfo) {}
	virtual void gotMessage(ofMessage msg) {}


#pragma mark - Custom Events
	ofEvent<bool> startDrawInEvent;
	ofEvent<bool> startDrawingEvent;
	ofEvent<bool> finishedDrawingEvent;
	ofEvent<bool> startDrawOutEvent;
	ofEvent<bool> finishSceneEvent;



private:
#pragma mark - private method

	enum {
		DRAWIN,
		DRAWING,
		DRAWOUT,
		FINISHED
	} _status;

	float _drawInSec, _drawingSec, _drawOutSec;
	float _drawDelaySec;
	float _statusEndTime;
	float _exitDrawTime;

	float _initTime;
	float _currentAlpha;
	glm::vec2 _currentPos;

	ofxSceneTransition _transition;
	bool _bExitByTime;
	bool _isShown;


#pragma mark - To be overridden
	virtual void start() {};
	virtual void behindUpdate() {};
	virtual void update() {}
	virtual void draw() {}

	virtual void willDrawIn() {}
	virtual void willDraw() {}
	virtual void willDrawOut() {}
	virtual void willExit() {}



#pragma mark - Start/finish

	void startDrawing();

	void startTransitionIn(ofxSceneTransition t);
	void startTransitionOut(ofxSceneTransition t);

	void finishScene();


#pragma mark - Status
	bool finishedDrawIn();
	bool finishedDrawing();
	bool finishedDrawOut();

};