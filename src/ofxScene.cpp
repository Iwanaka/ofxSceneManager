#include "ofxScene.h"

template<typename T>
T isNaNCheck(T &val, T defaultVal) {
	if (isnan(val)) return defaultVal;
	return val;
}

//--------------------------------------------------------------
ofxScene::ofxScene() :
	_drawInSec(.5f),
	_drawingSec(.0f),
	_drawOutSec(.5f),
	_drawDelaySec(.0f),
	_statusEndTime(.0f),
	_exitDrawTime(.0f),
	_initTime(.0f),
	_currentAlpha(.0f),
	_currentPos(glm::vec2(.0f, .0f)),
	_bExitByTime(false),
	_isShown(false),
	_status(FINISHED),
	_transition(ofxSceneTransition::SWITCH)
{}

//--------------------------------------------------------------
ofxScene::~ofxScene() {

}

//--------------------------------------------------------------
void ofxScene::startScene(shared_ptr<ofxScene> prevScene, ofxSceneTransition t) {

	start();
	startTransitionIn(t);

}

//--------------------------------------------------------------
void ofxScene::behindUpdateScene() {
	
	behindUpdate();

}

//--------------------------------------------------------------
void ofxScene::updateScene() {

	auto now = ofGetElapsedTimef();
	if (!isFinished()) {

		_currentAlpha = 255.f;
		_currentPos = glm::vec2(.0f, .0f);

		auto diff = now - _initTime;
		auto _sTime = _initTime + _drawDelaySec;


		if (_isShown) {

			switch (_transition)
			{
			case FADE:

				_currentAlpha = ofxeasing::map_clamp(now, _initTime, _statusEndTime, 0.f, 255.f, &ofxeasing::quad::easeOut);
				break;

			case SWITCH:

				break;

			case SLIDEIN_LEFT:

				_currentPos.x = -ofGetWidth();
				if (_drawDelaySec < diff) _currentPos.x = ofxeasing::map_clamp(now, _sTime, _statusEndTime, -ofGetWidth(), 0, &ofxeasing::quad::easeOut);
				break;

			case SLIDEIN_RIGHT:

				_currentPos.x = ofGetWidth();
				if (_drawDelaySec < diff) _currentPos.x = ofxeasing::map_clamp(now, _sTime, _statusEndTime, ofGetWidth(), 0, &ofxeasing::quad::easeOut);
				break;

			case SLIDEIN_BOTTOM:

				_currentPos.y = ofGetHeight();
				if (_drawDelaySec < diff) _currentPos.y = ofxeasing::map_clamp(now, _sTime, _statusEndTime, ofGetHeight(), 0, &ofxeasing::quad::easeOut);
				break;

			case SLIDEIN_TOP:

				_currentPos.y = -ofGetHeight();
				if (_drawDelaySec < diff) _currentPos.y = ofxeasing::map_clamp(now, _sTime, _statusEndTime, -ofGetHeight(), 0, &ofxeasing::quad::easeOut);
				break;
			}

			_currentAlpha = isNaNCheck(_currentAlpha, 255.f);
			_currentPos.x = isNaNCheck(_currentPos.x, 0.f);
			_currentPos.y = isNaNCheck(_currentPos.y, 0.f);

		}
		else {

			if (_drawDelaySec < diff) {

				switch (_transition)
				{
				case FADE:

					_currentAlpha = ofxeasing::map_clamp(now, _sTime, _statusEndTime, 255.f, 0.f, &ofxeasing::quad::easeOut);
					break;

				case SWITCH:

					if (_drawDelaySec + _drawOutSec < diff) _currentAlpha = 0;
					break;

				case SLIDEIN_LEFT:

					_currentPos.x = ofxeasing::map_clamp(now, _sTime, _statusEndTime, 0, ofGetWidth(), &ofxeasing::quad::easeOut);
					break;

				case SLIDEIN_RIGHT:

					_currentPos.x = ofxeasing::map_clamp(now, _sTime, _statusEndTime, 0, -ofGetWidth(), &ofxeasing::quad::easeOut);
					break;

				case SLIDEIN_BOTTOM:

					_currentPos.y = ofxeasing::map_clamp(now, _sTime, _statusEndTime, 0, -ofGetHeight(), &ofxeasing::quad::easeOut);
					break;

				case SLIDEIN_TOP:

					_currentPos.y = ofxeasing::map_clamp(now, _sTime, _statusEndTime, 0, ofGetHeight(), &ofxeasing::quad::easeOut);
					break;

				}

			}

			_currentAlpha = isNaNCheck(_currentAlpha, 0.f);
			_currentPos.x = isNaNCheck(_currentPos.x, (float)-ofGetWidth());
			_currentPos.y = isNaNCheck(_currentPos.y, (float)-ofGetHeight());
		}


		if (finishedDrawIn()) startDrawing();
		else if (finishedDrawing()) startTransitionOut(_transition);
		else if (finishedDrawOut()) finishScene();
		else if (isFinished()) return;

		update();

	}

}

//--------------------------------------------------------------
void ofxScene::drawScene() {
	
	draw();

}

//--------------------------------------------------------------
void ofxScene::exitScene(ofxSceneTransition t) {

	startTransitionOut(t);

}

//--------------------------------------------------------------
bool ofxScene::isDrawIn() {

	return _status == DRAWIN;

}

//--------------------------------------------------------------
bool ofxScene::isDrawing() {

	return _status == DRAWING;

}

//--------------------------------------------------------------
bool ofxScene::isDrawOut() {

	return _status == DRAWOUT;

}

//--------------------------------------------------------------
bool ofxScene::isFinished() {
	
	return _status == FINISHED;

}

//--------------------------------------------------------------
void ofxScene::setSceneDuration(float drawInSec, float drawOutSec) {

	_drawInSec = drawInSec;
	_drawOutSec = drawOutSec;

}

//--------------------------------------------------------------
void ofxScene::setSceneDuration(float drawInSec, float drawOutSec, float drawingSec) {

	_drawInSec = drawInSec;
	_drawOutSec = drawOutSec;
	_drawingSec = drawingSec;

}

//--------------------------------------------------------------
void ofxScene::setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec) {

	_drawInSec = drawInSec;
	_drawOutSec = drawOutSec;
	_drawingSec = drawingSec;
	_drawDelaySec = drawDelaySec;

}

//--------------------------------------------------------------
void ofxScene::setSceneDelayDuration(float drawDelaySec) {
	
	_drawDelaySec = drawDelaySec;

}

//--------------------------------------------------------------
float ofxScene::getDrawInDuration() {

	return _drawInSec;

}

//--------------------------------------------------------------
float ofxScene::getDrawingDuration() {

	return _drawingSec;

}

//--------------------------------------------------------------
float ofxScene::getDrawOutDuration() {

	return _drawOutSec;

}

//--------------------------------------------------------------
float ofxScene::getDrawDelayDuration() {

	return _drawDelaySec;

}

//--------------------------------------------------------------
void ofxScene::setExitByTime(bool b) {

	_bExitByTime = b;

}

//--------------------------------------------------------------
float ofxScene::getSceneAlpha() {

	return _currentAlpha;

}

//--------------------------------------------------------------
float ofxScene::getScenePosX() {

	return _currentPos.x;

}

//--------------------------------------------------------------
float ofxScene::getScenePosY() {

	return _currentPos.y;

}

//--------------------------------------------------------------
glm::vec2 ofxScene::getScenePos() {

	return _currentPos;

}



//--------------------------------------------------------------
void ofxScene::startDrawing() {

	if (_status == DRAWING) return;
	
	_status = DRAWING;

	//auto hide
	if (_bExitByTime) _exitDrawTime = _drawingSec + ofGetElapsedTimef();
	

	willDraw();
	bool b = true;
	ofNotifyEvent(startDrawingEvent, b, this);

}

//--------------------------------------------------------------
void ofxScene::startTransitionIn(ofxSceneTransition t) {

	if (_status == DRAWIN) return;

	_status = DRAWIN;

	_initTime = ofGetElapsedTimef();
	_statusEndTime = _initTime + _drawInSec;
	
	if (t == ofxSceneTransition::SLIDEIN_BOTTOM ||
		t == ofxSceneTransition::SLIDEIN_TOP ||
		t == ofxSceneTransition::SLIDEIN_LEFT ||
		t == ofxSceneTransition::SLIDEIN_RIGHT) {
		_statusEndTime += _drawDelaySec;
	}

	_isShown = true;
	_transition = t;

	willDrawIn();

	bool b = true;
	ofNotifyEvent(startDrawInEvent, b, this);

}

//--------------------------------------------------------------
void ofxScene::startTransitionOut(ofxSceneTransition t) {

	if (_status == DRAWOUT) return;

	_status = DRAWOUT;

	_initTime = ofGetElapsedTimef();
	_statusEndTime = _initTime + _drawOutSec + _drawDelaySec;
	_isShown = false;
	_transition = t;

	willDrawOut();

	bool b = true;
	ofNotifyEvent(startDrawOutEvent, b, this);

}

//--------------------------------------------------------------
void ofxScene::finishScene() {

	if (_status == FINISHED) return;
	
	_status = FINISHED;
	
	willExit();

	bool b = true;
	ofNotifyEvent(finishSceneEvent, b, this);
}

//--------------------------------------------------------------
bool ofxScene::finishedDrawIn() {

	return _status == DRAWIN && _statusEndTime < ofGetElapsedTimef();

}

//--------------------------------------------------------------
bool ofxScene::finishedDrawing() {

	if (_bExitByTime) {
		bool b = _status == DRAWING && _exitDrawTime < ofGetElapsedTimef();
		if (b) ofNotifyEvent(finishedDrawingEvent, b, this);
		return b;
	}
	
	return false;
	
}

//--------------------------------------------------------------
bool ofxScene::finishedDrawOut() {

	return _status == DRAWOUT && _statusEndTime < ofGetElapsedTimef();

}