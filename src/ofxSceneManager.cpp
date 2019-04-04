
//
//  ofxSceneManager.cpp
//  ofxSceneManager
//
//  Created by Koki Nomura on 2014/03/12.
//  updated by Hiromitsu Iwanaka on 2019/03/20.
//
//

#include "ofxSceneManager.h"

//--------------------------------------------------------------
ofxSceneManager::ofxSceneManager() :
	_beforeSceneIndex(0),
	_sceneIndex(0),
	_nextSceneIndex(0),
	_isInTransition(false),
	_transition(ofxSceneTransition::SWITCH)
{
	//Add Events
	ofAddListener(ofEvents().keyPressed, this, &ofxSceneManager::_keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxSceneManager::_keyReleased);
	ofAddListener(ofEvents().mouseMoved, this, &ofxSceneManager::_mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxSceneManager::_mouseDragged);
	ofAddListener(ofEvents().mousePressed, this, &ofxSceneManager::_mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxSceneManager::_mouseReleased);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxSceneManager::_mouseScrolled);
	ofAddListener(ofEvents().mouseEntered, this, &ofxSceneManager::_mouseEntered);
	ofAddListener(ofEvents().mouseExited, this, &ofxSceneManager::_mouseExited);
	ofAddListener(ofEvents().windowResized, this, &ofxSceneManager::_windowResized);
	ofAddListener(ofEvents().fileDragEvent, this, &ofxSceneManager::_dragEvent);
	ofAddListener(ofEvents().messageEvent, this, &ofxSceneManager::_gotMessage);
}

//--------------------------------------------------------------
ofxSceneManager::~ofxSceneManager() {

	//Release ptr
	if (nullptr != _currentScene) _currentScene = nullptr;
	if (nullptr != _nextScene) _nextScene = nullptr;

	for (int i = 0; i < scenes.size(); i++) {
		if (nullptr != scenes[i]) scenes[i] = nullptr;
	}

	//Remove Events
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSceneManager::_keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxSceneManager::_keyReleased);
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxSceneManager::_mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxSceneManager::_mouseDragged);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxSceneManager::_mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxSceneManager::_mouseReleased);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxSceneManager::_mouseScrolled);
	ofRemoveListener(ofEvents().mouseEntered, this, &ofxSceneManager::_mouseEntered);
	ofRemoveListener(ofEvents().mouseExited, this, &ofxSceneManager::_mouseExited);
	ofRemoveListener(ofEvents().windowResized, this, &ofxSceneManager::_windowResized);
	ofRemoveListener(ofEvents().fileDragEvent, this, &ofxSceneManager::_dragEvent);
	ofRemoveListener(ofEvents().messageEvent, this, &ofxSceneManager::_gotMessage);
}

//--------------------------------------------------------------
void ofxSceneManager::setup() {

	ofPtr<ofxScene> previousScene;
	previousScene = _currentScene;
	_currentScene = scenes.at(_sceneIndex);

	_currentScene->startScene(previousScene, _transition);

}

//--------------------------------------------------------------
void ofxSceneManager::setup(int screenWidth, int screenHeight) {

	setScreen(screenWidth, screenHeight);
	setup();

}

//--------------------------------------------------------------
void ofxSceneManager::setScreen(int screenWidth, int screenHeight) {

	//fbo setup
	_fbo.allocate(screenWidth, screenHeight, GL_RGBA);
	_nextFbo.allocate(screenWidth, screenHeight, GL_RGBA);

	_fbo.begin();
	ofClear(255, 255, 255, 0);
	_fbo.end();

	_nextFbo.begin();
	ofClear(255, 255, 255, 0);
	_nextFbo.end();

}

//--------------------------------------------------------------
void ofxSceneManager::update() {

    if (isTransition()) _nextScene->updateScene();
    _currentScene->updateScene();


	for (int i = 0; i < scenes.size(); i++) {
		scenes.at(i)->behindUpdateScene();
	}


	//fbo update
	if (isTransition()) {
		_nextFbo.begin();
		{
			ofClear(0);
			_nextScene->drawScene();
		}
		_nextFbo.end();
	}

	_fbo.begin();
	{
		ofClear(0);
		_currentScene->drawScene();
	}
	_fbo.end();
}

//--------------------------------------------------------------
void ofxSceneManager::draw() {

	ofPushStyle();
	{

		ofSetColor(255, 255, 255, _currentScene->getSceneAlpha());
		_fbo.draw(_currentScene->getScenePos());
		
		if (isTransition()) {
			ofSetColor(255, 255, 255, _nextScene->getSceneAlpha());
			_nextFbo.draw(_nextScene->getScenePos());
		}
	}
	ofPopStyle();

}

//--------------------------------------------------------------
void ofxSceneManager::draw(int x, int y) {
	ofPushMatrix();
	{
		ofTranslate(glm::vec2(x, y));
		draw();
	}
	ofPopMatrix();
}


//--------------------------------------------------------------
void ofxSceneManager::addScene(ofPtr<ofxScene> pScene) {

	ofAddListener(pScene->startDrawInEvent, this, &ofxSceneManager::_onStartDrawIn);
	ofAddListener(pScene->startDrawingEvent, this, &ofxSceneManager::_onStartDrawing);
	ofAddListener(pScene->finishedDrawingEvent, this, &ofxSceneManager::_onFinishedDrawing);
	ofAddListener(pScene->startDrawOutEvent, this, &ofxSceneManager::_onStartDrawOut);
	ofAddListener(pScene->finishSceneEvent, this, &ofxSceneManager::_onFinishScene);

	scenes.push_back(pScene);

}

//--------------------------------------------------------------
void ofxSceneManager::changeScene(unsigned int sceneIndex) {

    if (sceneIndex == _sceneIndex || scenes.size() - 1 <  sceneIndex) return;
    
    _nextSceneIndex = sceneIndex;
	_currentScene->exitScene(_transition);


}

//--------------------------------------------------------------
void ofxSceneManager::changeScene(unsigned int sceneIndex, ofxSceneTransition t) {
	_transition = t;
	changeScene(sceneIndex);
}

//--------------------------------------------------------------
void ofxSceneManager::setExitByTime(bool b) {
    for (int i=0; i<scenes.size(); i++) {
        scenes.at(i)->setExitByTime(b);
    }
}

//--------------------------------------------------------------
void ofxSceneManager::setSceneDuration(float drawInSec, float drawOutSec) {
    for (int i=0; i<scenes.size(); i++) {
        scenes.at(i)->setSceneDuration(drawInSec, drawOutSec);
    }
}

//--------------------------------------------------------------
void ofxSceneManager::setSceneDuration(float drawInSec, float drawingSec, float drawOutSec) {
    for (int i=0; i<scenes.size(); i++) {
        scenes.at(i)->setSceneDuration(drawInSec, drawingSec, drawOutSec);
    }
}

//--------------------------------------------------------------
void ofxSceneManager::setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec) {
	for (int i = 0; i < scenes.size(); i++) {
		scenes.at(i)->setSceneDuration(drawInSec, drawingSec, drawOutSec, drawDelaySec);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::setDelayDuration(float drawDelaySec) {
	for (int i = 0; i < scenes.size(); i++) {
		scenes.at(i)->setSceneDelayDuration(drawDelaySec);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::setTransitionMode(ofxSceneTransition t) {
	_transition = t;
}

//--------------------------------------------------------------
int ofxSceneManager::getBeforeSceneIndex() {
	return _beforeSceneIndex;
};

//--------------------------------------------------------------
int ofxSceneManager::getCurrentSceneIndex() {
	return _sceneIndex;
}

//--------------------------------------------------------------
int ofxSceneManager::getSceneIndex(ofPtr<ofxScene> scene) {

	for (int i = 0; i < scenes.size(); i++) {
		if (scenes[i] == scene) return i;
	}

	//not mutch
	return -1;
}

//--------------------------------------------------------------
bool ofxSceneManager::isTransition() {
	return _isInTransition;
}




#pragma mark - Private Methods
//--------------------------------------------------------------
void ofxSceneManager::_onStartDrawIn(bool &b) {
	//cout << "start draw in" << endl;
}

//--------------------------------------------------------------
void ofxSceneManager::_onStartDrawing(bool &b) {
	//cout << "drawing" << endl;
}

//--------------------------------------------------------------
void ofxSceneManager::_onFinishedDrawing(bool &b) {
	//cout << "finished drawing" << endl;
    // called when scenes exit by time
    _nextSceneIndex = (_sceneIndex + 1) % scenes.size();
}

//--------------------------------------------------------------
void ofxSceneManager::_onStartDrawOut(bool &b) {

	//cout << "start draw out" << endl;

	_isInTransition = true;
	_nextScene = scenes.at(_nextSceneIndex);

	if (_transition == ofxSceneTransition::FADE || _transition == ofxSceneTransition::SWITCH) {
		;
	}
	else {

		if (_transition == ofxSceneTransition::SLIDEIN_LEFT ||
			_transition == ofxSceneTransition::SLIDEIN_RIGHT ||
			_transition == ofxSceneTransition::SLIDEIN_TOP ||
			_transition == ofxSceneTransition::SLIDEIN_BOTTOM) {

			_nextScene->startScene(_currentScene, _transition);

		}
	}
}

//--------------------------------------------------------------
void ofxSceneManager::_onFinishScene(bool &b) {

	//cout << "Finished" << endl;

	_beforeSceneIndex = _sceneIndex;
	_sceneIndex = _nextSceneIndex;

	if (_transition == ofxSceneTransition::FADE || _transition == ofxSceneTransition::SWITCH) {

		ofPtr<ofxScene> previousScene;
		previousScene = _currentScene;
		_currentScene = scenes.at(_sceneIndex);
		_currentScene->startScene(previousScene, _transition);
		
	}
	else _currentScene = _nextScene;
	
	_isInTransition = false;

}


#pragma mark - Events
//--------------------------------------------------------------
void ofxSceneManager::_keyPressed(ofKeyEventArgs &args) {
    _currentScene->keyPressed(args.key);
}

//--------------------------------------------------------------
void ofxSceneManager::_keyReleased(ofKeyEventArgs &args) {
    _currentScene->keyReleased(args.key);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseMoved(ofMouseEventArgs &args) {
    _currentScene->mouseMoved(args.x, args.y);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseDragged(ofMouseEventArgs &args) {
    _currentScene->mouseDragged(args.x, args.y, args.button);
}

//--------------------------------------------------------------
void ofxSceneManager::_mousePressed(ofMouseEventArgs &args) {
    _currentScene->mousePressed(args.x, args.y, args.button);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseReleased(ofMouseEventArgs &args) {
    _currentScene->mouseReleased(args.x, args.y, args.button);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseScrolled(ofMouseEventArgs &args) {
	_currentScene->mouseScrolled(args.x, args.y, args.scrollX, args.scrollY);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseEntered(ofMouseEventArgs &args) {
	_currentScene->mouseEntered(args.x, args.y);
}

//--------------------------------------------------------------
void ofxSceneManager::_mouseExited(ofMouseEventArgs &args) {
	_currentScene->mouseExited(args.x, args.y);
}

//--------------------------------------------------------------
void ofxSceneManager::_windowResized(ofResizeEventArgs &args) {
    _currentScene->windowResized(args.width, args.height);
}

//--------------------------------------------------------------
void ofxSceneManager::_dragEvent(ofDragInfo &dragInfo) {
    _currentScene->dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofxSceneManager::_gotMessage(ofMessage &msg) {
    _currentScene->gotMessage(msg);
}