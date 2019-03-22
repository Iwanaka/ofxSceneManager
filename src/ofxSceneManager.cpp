
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
void ofxSceneManager::run() {

    _fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    _nextFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    _fbo.begin();
    ofClear(255, 255, 255, 0);
    _fbo.end();
    
    _nextFbo.begin();
    ofClear(255, 255, 255, 0);
    _nextFbo.end();
    
    ofPtr<ofxScene> previousScene;
    previousScene = _currentScene;
    _currentScene = scenes.at(_sceneIndex);

	if (transition == TRANSITION_SWITCH) _currentScene->startScene(previousScene, ofxScene::SWITCH);
	else if (transition == TRANSITION_FADE) _currentScene->startScene(previousScene, ofxScene::FADE);
	else if (transition == TRANSITION_SLIDE_LEFT_IN) _currentScene->startScene(previousScene, ofxScene::SLIDEIN_LEFT);
	else if (transition == TRANSITION_SLIDE_RIGHT_IN) _currentScene->startScene(previousScene, ofxScene::SLIDEIN_RIGHT);
	else if (transition == TRANSITION_SLIDE_TOP_IN) _currentScene->startScene(previousScene, ofxScene::SLIDEIN_TOP);
	else if (transition == TRANSITION_SLIDE_BOTTOM_IN) _currentScene->startScene(previousScene, ofxScene::SLIDEIN_BOTTOM);


    
    // Events
    ofAddListener(ofEvents().keyPressed, this, &ofxSceneManager::_keyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxSceneManager::_keyReleased);
    ofAddListener(ofEvents().mouseMoved, this, &ofxSceneManager::_mouseMoved);
    ofAddListener(ofEvents().mouseDragged, this, &ofxSceneManager::_mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &ofxSceneManager::_mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &ofxSceneManager::_mouseReleased);
    ofAddListener(ofEvents().windowResized, this, &ofxSceneManager::_windowResized);
    ofAddListener(ofEvents().fileDragEvent, this, &ofxSceneManager::_dragEvent);
    ofAddListener(ofEvents().messageEvent, this, &ofxSceneManager::_gotMessage);
}

//--------------------------------------------------------------
void ofxSceneManager::update() {

    if (_isInTransition) {
        _nextScene->updateScene();
    }
    _currentScene->updateScene();


	for (int i = 0; i < scenes.size(); i++) {
		scenes.at(i)->behindUpdateScene();
	}
}

//--------------------------------------------------------------
void ofxSceneManager::draw() {

    if (_isInTransition) {
        _nextFbo.begin();
		ofClear(0);
        _nextScene->drawScene();
        _nextFbo.end();
    }

    _fbo.begin();
	ofClear(0);
    _currentScene->drawScene();
    _fbo.end();


    ofPushStyle();
	ofSetColor(255, 255, 255, _currentScene->getSceneAlpha());
	_fbo.draw(_currentScene->getSceneX(), _currentScene->getSceneY());
	ofPopStyle();

    if (_isInTransition) {
        ofPushStyle();
		ofSetColor(255, 255, 255, _nextScene->getSceneAlpha());
		_nextFbo.draw(_nextScene->getSceneX(), _nextScene->getSceneY());
        ofPopStyle();
	}

}

//--------------------------------------------------------------
void ofxSceneManager::changeScene(unsigned int sceneIndex) {
    if (sceneIndex == _sceneIndex || scenes.size() - 1 <  sceneIndex) {
        return;
    }
    _nextSceneIndex = sceneIndex;
	if (transition == TRANSITION_SWITCH) _currentScene->exitScene(ofxScene::SWITCH);
	else if (transition == TRANSITION_FADE) _currentScene->exitScene(ofxScene::FADE);
	else if (transition == TRANSITION_SLIDE_LEFT_IN) _currentScene->exitScene(ofxScene::SLIDEIN_LEFT);
	else if (transition == TRANSITION_SLIDE_RIGHT_IN) _currentScene->exitScene(ofxScene::SLIDEIN_RIGHT);
	else if (transition == TRANSITION_SLIDE_TOP_IN) _currentScene->exitScene(ofxScene::SLIDEIN_TOP);
	else if (transition == TRANSITION_SLIDE_BOTTOM_IN) _currentScene->exitScene(ofxScene::SLIDEIN_BOTTOM);

}

//--------------------------------------------------------------
void ofxSceneManager::changeScene(unsigned int sceneIndex, ofxSmTransition t) {
	if (sceneIndex == _sceneIndex || scenes.size() - 1 < sceneIndex) {
		return;
	}
	transition = t;
	changeScene(sceneIndex);
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
void ofxSceneManager::setSceneDelayDuration(float drawDelaySec) {
	for (int i = 0; i < scenes.size(); i++) {
		scenes.at(i)->setSceneDelayDuration(drawDelaySec);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::setTransitionMode(ofxSmTransition t) {

	transition = t;

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
		if (scenes[i] == scene) {
			return i;
		}
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

	//cout << "FadeIn" << endl;

}

//--------------------------------------------------------------
void ofxSceneManager::_onStartDrawing(bool &b) {

	//cout << "Draw" << endl;

}

//--------------------------------------------------------------
void ofxSceneManager::_onFinishedDrawing(bool &b) {
    // called when scenes exit by time
    _nextSceneIndex = (_sceneIndex + 1) % scenes.size();
}

//--------------------------------------------------------------
void ofxSceneManager::_onStartDrawOut(bool &b) {

	//cout << "FadeOut" << endl;
	_isInTransition = true;
	_nextScene = scenes.at(_nextSceneIndex);

	if (transition == TRANSITION_FADE || transition == TRANSITION_SWITCH) {

	}
	else {

		if (transition == TRANSITION_SLIDE_LEFT_IN) {
			_nextScene->startScene(_currentScene, ofxScene::SLIDEIN_LEFT);
		}
		else if (transition == TRANSITION_SLIDE_RIGHT_IN) {
			_nextScene->startScene(_currentScene, ofxScene::SLIDEIN_RIGHT);
		}
		else if (transition == TRANSITION_SLIDE_TOP_IN) {
			_nextScene->startScene(_currentScene, ofxScene::SLIDEIN_TOP);
		}
		else if (transition == TRANSITION_SLIDE_BOTTOM_IN) {
			_nextScene->startScene(_currentScene, ofxScene::SLIDEIN_BOTTOM);
		}
	}

}

//--------------------------------------------------------------
void ofxSceneManager::_onFinishScene(bool &b) {

	//cout << "Finished" << endl;

	_beforeSceneIndex = _sceneIndex;
	_sceneIndex = _nextSceneIndex;

	if (transition == TRANSITION_FADE || transition == TRANSITION_SWITCH) {
		
		ofPtr<ofxScene> previousScene;
		previousScene = _currentScene;
		_currentScene = scenes.at(_sceneIndex);
		
		if (transition == TRANSITION_FADE) {
			_currentScene->startScene(previousScene, ofxScene::FADE);
		}
		else if (transition == TRANSITION_SWITCH) {
			_currentScene->startScene(previousScene, ofxScene::SWITCH);
		}

	}
	else {
		_currentScene = _nextScene;
	}
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