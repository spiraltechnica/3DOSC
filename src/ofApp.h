#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

/* Standard Win32 Includes */
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <crtdbg.h>


/* SpaceWare Specific Includes */

#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */

extern "C" {
#include "si.h"
#include "siapp.h"
#include "virtualkeys.hpp"
}

//#include "si.h"
//#include "siapp.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofxOscSender sender;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		HDC          hdc;         /* Handle to Device Context used to draw on screen */
		HWND         hWndMain;    /* Handle to Main Window */


		SiHdl        devHdl;      /* Handle to 3D Mouse Device */
		//TCHAR devicename[100] = _T("");

		string devicename;

		/* Functions --See C File for Additional Details */

		//LRESULT  WINAPI HandleNTEvent(HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);
		int   DispatchLoopNT();
		//void  CreateSPWindow(int, int, int, int, TCHAR *);
		int   SbInit();
		void  SbMotionEvent(SiSpwEvent *pEvent);
		void  SbZeroEvent();
		void  SbButtonPressEvent(int buttonnumber);
		void  SbButtonReleaseEvent(int buttonnumber);
		void  HandleDeviceChangeEvent(SiSpwEvent *pEvent);
		void  HandleV3DCMDEvent(SiSpwEvent *pEvent);

		float tx, ty, tz, rx, ry, rz;
		ofEasyCam cam; //

		int res;
		
};
