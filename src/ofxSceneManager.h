//
//  ofxSceneManager.h
//  ofxSceneManager
//
//  Created by Koki Nomura on 2014/03/12.
//  updated by Hiromitsu Iwanaka on 2019/03/20.
//
//

#pragma once

#include "ofxScene.h"

class ofxSceneManager {
public:

	ofxSceneManager();
	~ofxSceneManager();


	void setup();
	void setup(int screenWidth, int screenHeight);
	void setScreen(int screenWidth, int screenHeight);

    void update();
    void draw();
	void draw(int x, int y);

	void addScene(ofPtr<ofxScene> pScene);

    void changeScene(unsigned int sceneIndex);
	void changeScene(unsigned int sceneIndex, ofxSceneTransition t);

    void setExitByTime(bool b);
    void setSceneDuration(float drawInSec, float drawOutSec);
    void setSceneDuration(float drawInSec, float drawingSec, float drawOutSec);  
	void setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec);
	void setDelayDuration(float drawDelaySec);

	void setTransitionMode(ofxSceneTransition t);

	int getBeforeSceneIndex();
	int getCurrentSceneIndex();
	int getSceneIndex(ofPtr<ofxScene> scene);
	bool isTransition();
    
	
	vector<ofPtr<ofxScene> > scenes;


 private:


	int _beforeSceneIndex;
    int _sceneIndex;
    int _nextSceneIndex;
    
    ofPtr<ofxScene> _currentScene;
    ofPtr<ofxScene> _nextScene;

    ofFbo _fbo;
    ofFbo _nextFbo;
    
	ofxSceneTransition _transition;

	bool _isInTransition;
    
    void _onStartDrawIn(bool &b);
    void _onStartDrawing(bool &b);
    void _onFinishedDrawing(bool &b);    
    void _onStartDrawOut(bool &b);
    void _onFinishScene(bool &b);
    
    void _keyPressed(ofKeyEventArgs & args);
    void _keyReleased(ofKeyEventArgs & args);
    void _mouseMoved(ofMouseEventArgs & args);
    void _mouseDragged(ofMouseEventArgs & args);
    void _mousePressed(ofMouseEventArgs & args);
    void _mouseReleased(ofMouseEventArgs & args);
	void _mouseScrolled(ofMouseEventArgs & args);
	void _mouseEntered(ofMouseEventArgs & args);
	void _mouseExited(ofMouseEventArgs & args);
    void _windowResized(ofResizeEventArgs & args);
    void _dragEvent(ofDragInfo & dragInfo);
    void _gotMessage(ofMessage & msg);
};
