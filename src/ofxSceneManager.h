//
//  ofxSceneManager.h
//  ofxSceneManager
//
//  Created by Koki Nomura on 2014/03/12.
//  updated by Hiromitsu Iwanaka on 2019/03/20.
//
//

#ifndef __KokiNomura__ofxSceneManager__
#define __KokiNomura__ofxSceneManager__

#include "ofxScene.h"

enum ofxSmTransition{
	TRANSITION_SWITCH,
    TRANSITION_FADE,
	TRANSITION_SLIDE_LEFT_IN,
	TRANSITION_SLIDE_RIGHT_IN,
	TRANSITION_SLIDE_TOP_IN,
	TRANSITION_SLIDE_BOTTOM_IN
};

class ofxSceneManager {
public:

    void run();
    
    void update();
    void draw();

    //void changeScene();
    void changeScene(unsigned int sceneIndex);
	void changeScene(unsigned int sceneIndex, ofxSmTransition t);

    void addScene(ofPtr<ofxScene> pScene);

    void setExitByTime(bool b);
    void setSceneDuration(float drawInSec, float drawOutSec);
    void setSceneDuration(float drawInSec, float drawingSec, float drawOutSec);  
	void setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec);
	void setSceneDelayDuration(float drawDelaySec);

	void setTransitionMode(ofxSmTransition t);

	int getBeforeSceneIndex();
	int getCurrentSceneIndex();
	int getSceneIndex(ofPtr<ofxScene> scene);
	bool isTransition();
    vector<ofPtr<ofxScene> > scenes;

 private:
	int _beforeSceneIndex = 0;
    int _sceneIndex = 0;
    int _nextSceneIndex;
    
    ofPtr<ofxScene> _currentScene;
    ofPtr<ofxScene> _nextScene;

    ofFbo _fbo;
    ofFbo _nextFbo;
    
    ofxSmTransition transition = TRANSITION_FADE;
    bool _isInTransition = false;
    
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
    void _windowResized(ofResizeEventArgs & args);
    void _dragEvent(ofDragInfo & dragInfo);
    void _gotMessage(ofMessage & msg);
};

#endif /* defined(__KokiNomura__ofxSceneManager__) */
