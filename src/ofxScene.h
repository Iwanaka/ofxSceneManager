//
//  ofxScene.h
//  ofxSceneManager
//
//  Created by Koki Nomura on 2014/01/22.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTween.h"

class ofxScene {
public:

#pragma mark - transition modes
	typedef enum {
		SWITCH,
		FADE,
		SLIDEIN_LEFT,
		SLIDEIN_RIGHT,
		SLIDEIN_TOP,
		SLIDEIN_BOTTOM
	} _TRANSITION;



#pragma mark - Basics
	//--------------------------------------------------------------
    void startScene(ofPtr<ofxScene> previousScene, _TRANSITION mode) {

        start();
        if (!_isSetupOverridden) {
            // If a subclass overrides setup(),
            // ofxScene won't call setup(previousScene).
            start(previousScene);
        }

		if (mode == _TRANSITION::FADE) {
			startFadingIn();
		}
		else if (mode == _TRANSITION::SWITCH) {
			startSwitchIn();
		}
		else startSlideIn(mode);

    }
    
	//--------------------------------------------------------------
	void behindUpdateScene() { behindUpdate(); }

	//--------------------------------------------------------------
    void updateScene() {
				
		if (!isFinished()) _alphaTween.update();
		if (!isFinished()) _xTween.update();
		if (!isFinished()) _yTween.update();

        if (finishedDrawIn()) {

            startDrawing();
        
		} else if (finishedDrawing()) {
            
			startFadingOut();
        
		} else if (finishedDrawOut()) {
            
			finishScene();

        } else if (isFinished()) {
            return;
        }

        update();
    }
    
	//--------------------------------------------------------------
    void drawScene() { draw(); }
    
	//0 : left out ,1: right out,2 : top out, 3: bottom out, other fadeOut
	//--------------------------------------------------------------
    void exitScene(_TRANSITION mode) {

        // Call this function to exit scene.
		if (mode == _TRANSITION::FADE) {
			startFadingOut();
		}
		else if (mode == _TRANSITION::SWITCH) {
			startSwitchOut();
		}
		else startSlideOut(mode);

    }
    

#pragma mark - Status
	//--------------------------------------------------------------
    bool isDrawIn() {
        return _status == DRAWIN;
    }
	//--------------------------------------------------------------
    bool isDrawing() {
        return _status == DRAWING;
    }
	//--------------------------------------------------------------
    bool isDrawOut() {
        return _status == DRAWOUT;
    }
	//--------------------------------------------------------------
    bool isFinished() {
        return _status == FINISHED;
    }
    


#pragma mark - Duration
	//--------------------------------------------------------------
    void setSceneDuration(float drawInSec, float drawOutSec) {
        _drawInSec = drawInSec;
        _drawOutSec = drawOutSec;
    }

	//--------------------------------------------------------------
    void setSceneDuration(float drawInSec, float drawingSec, float drawOutSec) {
        _drawInSec = drawInSec;
        _drawingSec = drawingSec;
        _drawOutSec = drawOutSec;
    }

	//--------------------------------------------------------------
	void setSceneDuration(float drawInSec, float drawOutSec, float drawingSec, float drawDelaySec) {
		_drawInSec = drawInSec;
		_drawingSec = drawingSec;
		_drawOutSec = drawOutSec;
		_drawDelaySec = drawDelaySec;
	}
    
	//--------------------------------------------------------------
	void setSceneDelayDuration(float drawDelaySec) {
		
		_drawDelaySec = drawDelaySec;

	}

	//--------------------------------------------------------------
    float getDrawInDuration() {
        return _drawInSec;
    }
    
	//--------------------------------------------------------------
    float getDrawingDuration() {
        return _drawingSec;
    }
    
	//--------------------------------------------------------------
    float getDrawOutDuration() {
        return _drawOutSec;
    }

	//--------------------------------------------------------------
	float getDrawDelayDuration() {
		return _drawDelaySec;
	}
    


#pragma mark - Etc
	//--------------------------------------------------------------
    void setExitByTime(bool b) {
        _bExitByTime = b;
    }
    
	//--------------------------------------------------------------
    float getSceneAlpha() {
        return _alphaTween.getTarget(0);
    }

	//--------------------------------------------------------------
	float getSceneX() {
		return _xTween.getTarget(0);
	}

	//--------------------------------------------------------------
	float getSceneY() {
		return _yTween.getTarget(0);
	}
    


#pragma mark - Events
	//--------------------------------------------------------------
    virtual void keyPressed(int key) {}
    virtual void keyReleased(int key) {}
    virtual void mouseMoved(int x, int y) {}
    virtual void mouseDragged(int x, int y, int button) {}
    virtual void mousePressed(int x, int y, int button) {}
    virtual void mouseReleased(int x, int y, int button) {}
    virtual void windowResized(int w, int h) {}
    virtual void dragEvent(ofDragInfo dragInfo) {}
    virtual void gotMessage(ofMessage msg) {}
    
#pragma mark - Custom Events
	//--------------------------------------------------------------
    ofEvent<bool> startDrawInEvent;  // The arg is currently not used.
    ofEvent<bool> startDrawingEvent;
    ofEvent<bool> finishedDrawingEvent;    
    ofEvent<bool> startDrawOutEvent;
    ofEvent<bool> finishSceneEvent;


private:
    bool _bExitByTime = false;
	
	enum {
		DRAWIN,
		DRAWING,
		DRAWOUT,
		FINISHED
	} _status = FINISHED;
    
    float _drawInSec = 0.3;
    float _drawingSec = 1.0;
    float _drawOutSec = 0.3;
	float _drawDelaySec = 0.0;
    float _statusEndTime = 0;
    
	ofxTween _alphaTween, _xTween, _yTween;
    //ofxEasingLinear _easing;
	ofxEasingQuint _easing;

    bool _isSetupOverridden = true;  // a bit hacky...
    


#pragma mark - To be overridden
	//--------------------------------------------------------------
	virtual void setup() {};
    virtual void start() {_isSetupOverridden = false;}  // a bit hacky...
    virtual void start(ofPtr<ofxScene> previousScene) {}
	virtual void behindUpdate() {};
    virtual void update() {}
    virtual void draw() {}
    
    virtual void willDrawIn() {}
    virtual void willDraw() {}
    virtual void willDrawOut() {}
    virtual void willExit() {}
    


#pragma mark - Start/finish

	//--------------------------------------------------------------
    void startDrawing() {
        if (_status == DRAWING) {
            return;
        }
        _status = DRAWING;
        if (_bExitByTime) {
            _statusEndTime = _drawingSec + ofGetElapsedTimef();
        }

        willDraw();

		bool b = true;
        ofNotifyEvent(startDrawingEvent, b, this);
    }


	//--------------------------------------------------------------
	// FADE
	//--------------------------------------------------------------
	void startFadingIn() {
		if (_status == DRAWIN) {
			return;
		}
		_status = DRAWIN;
		_statusEndTime = _drawInSec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 0, 255, _drawInSec * 1000, 0);
		_xTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);
		_yTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);


		willDrawIn();

		bool b = true;
		ofNotifyEvent(startDrawInEvent, b, this);
	}

	//--------------------------------------------------------------
    void startFadingOut() {
		if (_status == DRAWOUT) {
			return;
		}
		_status = DRAWOUT;
		_statusEndTime = _drawOutSec + _drawDelaySec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 255, 0, _drawOutSec * 1000, _drawDelaySec * 1000);
		_xTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);
		_yTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);
		_alphaTween.start();

		willDrawOut();

		bool b = true;
		ofNotifyEvent(startDrawOutEvent, b, this);
    }

	//--------------------------------------------------------------
	// slide
	//--------------------------------------------------------------
	//0 : left in, 1:, right in, 2: top in, 3 bottom in
	void startSlideIn(_TRANSITION mode) {

		if (_status == DRAWIN) return;

		_status = DRAWIN;
		_statusEndTime = _drawInSec + _drawDelaySec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 255, 255, 0, 0);

		if (mode == _TRANSITION::SLIDEIN_LEFT) {
			_xTween.setParameters(_easing, ofxTween::easeOut, -ofGetWidth(), 0, _drawInSec * 1000, _drawDelaySec * 1000);
			_yTween.setParameters(_easing, ofxTween::easeOut, 0, 0, 0, 0);
		}
		else if (mode == _TRANSITION::SLIDEIN_RIGHT) {
			_xTween.setParameters(_easing, ofxTween::easeOut, ofGetWidth(), 0, _drawInSec * 1000, _drawDelaySec * 1000);
			_yTween.setParameters(_easing, ofxTween::easeOut, 0, 0, 0, 0);
		}
		else if (mode == _TRANSITION::SLIDEIN_TOP) {
			_xTween.setParameters(_easing, ofxTween::easeOut, 0, 0, 0, 0);
			_yTween.setParameters(_easing, ofxTween::easeOut, -ofGetHeight(), 0, _drawInSec * 1000, _drawDelaySec * 1000);
		}
		else if (mode == _TRANSITION::SLIDEIN_BOTTOM) {
			_xTween.setParameters(_easing, ofxTween::easeOut, 0, 0, 0, 0);
			_yTween.setParameters(_easing, ofxTween::easeOut, ofGetHeight(), 0, _drawInSec * 1000, _drawDelaySec * 1000);
		}

		willDrawIn();

		bool b = true;
		ofNotifyEvent(startDrawInEvent, b, this);
	}

	//--------------------------------------------------------------
	//0 : left out, 1:, right out, 2: top out, 3 bottom out
	void startSlideOut(_TRANSITION mode) {

		if (_status == DRAWOUT) return;

		_status = DRAWOUT;
		_statusEndTime = _drawOutSec + _drawDelaySec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 255, 255, 0, 0);

		if (mode == _TRANSITION::SLIDEIN_LEFT) _xTween.setParameters(_easing, ofxTween::easeOut, 0, ofGetWidth(), _drawOutSec * 1000, _drawDelaySec * 1000);
		else if (mode == _TRANSITION::SLIDEIN_RIGHT) _xTween.setParameters(_easing, ofxTween::easeOut, 0, -ofGetWidth(), _drawOutSec * 1000, _drawDelaySec * 1000);
		else if (mode == _TRANSITION::SLIDEIN_TOP) _yTween.setParameters(_easing, ofxTween::easeOut, 0, ofGetHeight(), _drawOutSec * 1000, _drawDelaySec * 1000);
		else if (mode == _TRANSITION::SLIDEIN_BOTTOM) _yTween.setParameters(_easing, ofxTween::easeOut, 0, -ofGetHeight(), _drawOutSec * 1000, _drawDelaySec * 1000);
		
		willDrawOut();

		bool b = true;
		ofNotifyEvent(startDrawOutEvent, b, this);
	}


	//--------------------------------------------------------------
	// switch
	//--------------------------------------------------------------
	void startSwitchIn() {

		if (_status == DRAWIN) return;
		_status = DRAWIN;
		_statusEndTime = _drawInSec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 0, 255, 0, _drawInSec * 1000);
		_xTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);
		_yTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);

		willDrawIn();

		bool b = true;
		ofNotifyEvent(startDrawInEvent, b, this);
	}

	//--------------------------------------------------------------
	void startSwitchOut() {

		if (_status == DRAWOUT) return;

		_status = DRAWOUT;
		_statusEndTime = _drawOutSec + _drawDelaySec + ofGetElapsedTimef();

		_alphaTween.setParameters(_easing, ofxTween::easeOut, 255, 0, 0, _drawOutSec * 1000 + _drawDelaySec * 1000);
		_xTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);
		_yTween.setParameters(_easing, ofxTween::easeInOut, 0, 0, 0, 0);

		willDrawOut();

		bool b = true;
		ofNotifyEvent(startDrawOutEvent, b, this);
	}
    
	//--------------------------------------------------------------
    void finishScene() {
        if (_status == FINISHED) {
            return;
        }
        _status = FINISHED;
        willExit();
           
		bool b = true;
        ofNotifyEvent(finishSceneEvent, b, this);
    }
    




#pragma mark - Status
	//--------------------------------------------------------------
    bool finishedDrawIn() {
        return _status == DRAWIN && _statusEndTime < ofGetElapsedTimef();
    }

	//--------------------------------------------------------------
    bool finishedDrawing() {
        if (_bExitByTime) {
            bool b = _status == DRAWING && _statusEndTime < ofGetElapsedTimef();
            if (b) {
                ofNotifyEvent(finishedDrawingEvent, b, this);
            }
            return b;
        } else {
            return false;
        }
    }
    

	//--------------------------------------------------------------
    bool finishedDrawOut() {
        return _status == DRAWOUT && _statusEndTime < ofGetElapsedTimef();
    }

};