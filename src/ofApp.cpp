#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);


	res = SbInit();	

	tx = 0;
	ty = 0;
	tz = 0;
	rx = 0;
	ry = 0;
	rz = 0;

	cout << res << endl;

	ofSetBackgroundAuto(false);
	ofBackground(0);


	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	//ofEnableDepthTest();

	// this sets the camera's distance from the object
	//cam.setDistance(100);
}

int ofApp::SbInit() {
	int res;          
	hWndMain = WindowFromDC(wglGetCurrentDC());
	/* result of SiOpen, to be returned  */
	SiOpenData oData;                    /* OS Independent data to open ball  */

										 /*init the SpaceWare input library */
	if (SiInitialize() == SPW_DLL_LOAD_ERROR)
	{
		MessageBox(hWndMain, _T("Error: Could not load SiAppDll dll files"),
			NULL, MB_ICONEXCLAMATION);
	}

	SiOpenWinInit(&oData, hWndMain);    /* init Win. platform specific data  */
	SiSetUiMode(devHdl, SI_UI_ALL_CONTROLS); /* Config SoftButton Win Display */

											 /* open data, which will check for device type and return the device handle
											 to be used by this function */
	if ((devHdl = SiOpen("3dosc", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
	{
#if 0
		SPWchar err[100];
		extern enum SpwRetVal SpwErrorVal;
		_stprintf(err, _T("SiOpen error: %d\n"), SpwErrorVal);
		MessageBox(NULL, err, _T("3DxTest: SiOpen Error"), MB_ICONERROR);
#endif

		SiTerminate();  /* called to shut down the SpaceWare input library */
		res = 0;        /* could not open device */
		return res;
	}
	else
	{
		SiGrabDevice(devHdl, SPW_TRUE); /* called if you want to receive input when application loses focus */
		SiDeviceName devName;
		SiGetDeviceName(devHdl, &devName);
		devicename = ofToString(devName.name);;
		//_stprintf(devicename, _T("%S"), devName.name);
		res = 1;        /* opened device succesfully */






		return res;
	}
}

/*--------------------------------------------------------------------------
* Function: DispatchLoopNT()
*
* Description:
*    This function contains the main message loop which constantly checks for
*    3D mouse Events and handles them apropriately.
*
* Args: None
*
*
* Return Value:
*    int  msg.wparam                  // event passed to window
*
*--------------------------------------------------------------------------*/
int ofApp::DispatchLoopNT()
{
	MSG            msg;      /* incoming message to be evaluated */
	BOOL           handled;  /* is message handled yet */
	SiSpwEvent     Event;    /* SpaceWare Event */
	SiGetEventData EData;    /* SpaceWare Event Data */

	handled = SPW_FALSE;     /* init handled */

							 /* start message loop */
	if (GetMessage(&msg, NULL, 0, 0)) //while <- it used to be this
	{
		handled = SPW_FALSE;

		/* init Window platform specific data for a call to SiGetEvent */
		SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

		/* check whether msg was a 3D mouse event and process it */
		if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
		{
			if (Event.type == SI_MOTION_EVENT)
			{
				SbMotionEvent(&Event);        /* process 3D mouse motion event */
			}
			else if (Event.type == SI_ZERO_EVENT)
			{
				SbZeroEvent();                /* process 3D mouse zero event */
			}
			else if (Event.type == SI_BUTTON_PRESS_EVENT)
			{
				SbButtonPressEvent(Event.u.hwButtonEvent.buttonNumber);  /* process button press event */
			}
			else if (Event.type == SI_BUTTON_RELEASE_EVENT)
			{
				SbButtonReleaseEvent(Event.u.hwButtonEvent.buttonNumber); /* process button release event */
			}
			else if (Event.type == SI_DEVICE_CHANGE_EVENT)
			{
				HandleDeviceChangeEvent(&Event); /* process 3D mouse device change event */
			}
			else if (Event.type == SI_CMD_EVENT)
			{
				HandleV3DCMDEvent(&Event); /* V3DCMD_* events */
			}

			handled = SPW_TRUE;              /* 3D mouse event handled */
		}

		/* not a 3D mouse event, let windows handle it */
		if (handled == SPW_FALSE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return((int)msg.wParam);
}


void ofApp::SbMotionEvent(SiSpwEvent *pEvent)
{
	TCHAR buff0[30];                            /* text buffer for TX */
	TCHAR buff1[30];                            /* text buffer for TY */
	TCHAR buff2[30];                            /* text buffer for TZ */
	TCHAR buff3[30];                            /* text buffer for RX */
	TCHAR buff4[30];                            /* text buffer for RY */
	TCHAR buff5[30];                            /* text buffer for RZ */
	TCHAR buff6[30];                            /* text buffer for Period */

	int len0, len1, len2, len3, len4, len5, len6;	   /* length of each buffer */

											   /* put the actual ball data into the buffers */
	
	tx = pEvent->u.spwData.mData[SI_TX]; 
	ty = pEvent->u.spwData.mData[SI_TY];
	tz = pEvent->u.spwData.mData[SI_TZ];
	rx = pEvent->u.spwData.mData[SI_RX];
	ry = pEvent->u.spwData.mData[SI_RY];
	rz = pEvent->u.spwData.mData[SI_RZ];


	//OSC begin
	ofxOscMessage m;
	m.setAddress("/tx");
	m.addFloatArg(tx);

	ofxOscMessage m2;
	m2.setAddress("/ty");
	m2.addFloatArg(ty);

	ofxOscMessage m3;
	m3.setAddress("/tz");
	m3.addFloatArg(tz);

	ofxOscMessage m4;
	m4.setAddress("/rx");
	m4.addFloatArg(rx);

	ofxOscMessage m5;
	m5.setAddress("/ry");
	m5.addFloatArg(ry);

	ofxOscMessage m6;
	m6.setAddress("/rz");
	m6.addFloatArg(rz);
	

	sender.sendMessage(m, false);
	sender.sendMessage(m2, false);
	sender.sendMessage(m3, false);
	sender.sendMessage(m4, false);
	sender.sendMessage(m5, false);
	sender.sendMessage(m6, false);
	
	
	//OSC end
	
	len0 = _stprintf(buff0, _T("TX: %d         "), pEvent->u.spwData.mData[SI_TX]);
	len1 = _stprintf(buff1, _T("TY: %d         "), pEvent->u.spwData.mData[SI_TY]);
	len2 = _stprintf(buff2, _T("TZ: %d         "), pEvent->u.spwData.mData[SI_TZ]);
	len3 = _stprintf(buff3, _T("RX: %d         "), pEvent->u.spwData.mData[SI_RX]);
	len4 = _stprintf(buff4, _T("RY: %d         "), pEvent->u.spwData.mData[SI_RY]);
	len5 = _stprintf(buff5, _T("RZ: %d         "), pEvent->u.spwData.mData[SI_RZ]);
	len6 = _stprintf(buff6, _T(" P: %d         "), pEvent->u.spwData.period);


	//working for test cout << pEvent->u.spwData.mData[SI_TX] << endl;

	/* get handle of our window to draw on */
	//hdc = GetDC(hWndMain);

	/* print buffers */
	//TCHAR *buf = _T("Motion Event              ");
	/*TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
	TextOut(hdc, 0, 20, devicename, (int)_tcslen(devicename));
	TextOut(hdc, 15, 100, buff0, len0);
	TextOut(hdc, 15, 120, buff1, len1);
	TextOut(hdc, 15, 140, buff2, len2);
	TextOut(hdc, 15, 160, buff3, len3);
	TextOut(hdc, 15, 180, buff4, len4);
	TextOut(hdc, 15, 200, buff5, len5);
	TextOut(hdc, 15, 220, buff6, len6);*/

	/* Dump to debugger output buffer to get a running log */
	//_RPT3(_CRT_WARN, "%S %S %S", buff0, buff1, buff2);
	//_RPT3(_CRT_WARN, " %S %S %S", buff3, buff4, buff5);
	//_RPT1(_CRT_WARN, " %S\n", buff6);

	// Also dump to stdout for a searchable log
	printf("%d %d %d %d %d %d %d\n",
		pEvent->u.spwData.mData[SI_TX],
		pEvent->u.spwData.mData[SI_TY],
		pEvent->u.spwData.mData[SI_TZ],
		pEvent->u.spwData.mData[SI_RX],
		pEvent->u.spwData.mData[SI_RY],
		pEvent->u.spwData.mData[SI_RZ],
		pEvent->u.spwData.period); 

	/*release our window handle */
	//ReleaseDC(hWndMain, hdc);
}

/*----------------------------------------------------------------------
* Function: SbZeroEvent()
*
* Description :
	*This function clears the previous data, no motion data was received
	*
	*
	*
	* Args :
	*NONE
	*
	* Return Value :
*NONE
*
*----------------------------------------------------------------------*/
void ofApp::SbZeroEvent()
{
	/* get handle of our window to draw on */
	//hdc = GetDC(hWndMain);

	/* print null data */
	/*TextOut(hdc, 0, 0, _T("Zero Event                  "), 28);
	TextOut(hdc, 0, 20, devicename, (int)_tcslen(devicename));
	TextOut(hdc, 15, 100, _T("TX: 0          "), 15);
	TextOut(hdc, 15, 120, _T("TY: 0          "), 15);
	TextOut(hdc, 15, 140, _T("TZ: 0          "), 15);
	TextOut(hdc, 15, 160, _T("RX: 0          "), 15);
	TextOut(hdc, 15, 180, _T("RY: 0          "), 15);
	TextOut(hdc, 15, 200, _T("RZ: 0          "), 15);
	TextOut(hdc, 15, 220, _T(" P: 0          "), 15);*/

	/*release our window handle */
	//ReleaseDC(hWndMain, hdc);
}

/*----------------------------------------------------------------------
* Function: SbButtonPressEvent()
*
* Description:
*    This function receives 3D mouse button information and prints out the
*    info on screen.
*
*
* Args:
*    int     buttonnumber   //Containts number of button pressed
*
* Return Value:
*    NONE
*
*----------------------------------------------------------------------*/
void ofApp::SbButtonPressEvent(int buttonnumber)
{
	TCHAR bn[100];

	/* get handle of our window to draw on */
	hdc = GetDC(hWndMain);

	SiButtonName name;
	SiGetButtonName(devHdl, buttonnumber, &name);
	
	/* print button pressed (does not include rezero button) */
	switch (buttonnumber)
	{
	case s3dm::V3DK_MENU: _tcscpy(bn, _T("V3DK_MENU")); break;
	case s3dm::V3DK_FIT: _tcscpy(bn, _T("V3DK_FIT")); break;
	case s3dm::V3DK_TOP: _tcscpy(bn, _T("V3DK_TOP")); break;
	case s3dm::V3DK_LEFT: _tcscpy(bn, _T("V3DK_LEFT")); break;
	case s3dm::V3DK_RIGHT: _tcscpy(bn, _T("V3DK_RIGHT")); break;
	case s3dm::V3DK_FRONT: _tcscpy(bn, _T("V3DK_FRONT")); break;
	case s3dm::V3DK_BOTTOM: _tcscpy(bn, _T("V3DK_BOTTOM")); break;
	case s3dm::V3DK_BACK: _tcscpy(bn, _T("V3DK_BACK")); break;
	case s3dm::V3DK_ROLL_CW: _tcscpy(bn, _T("V3DK_ROLL_CW")); break;
	case s3dm::V3DK_ROLL_CCW: _tcscpy(bn, _T("V3DK_ROLL_CCW")); break;
	case s3dm::V3DK_ISO1: _tcscpy(bn, _T("V3DK_ISO1")); break;
	case s3dm::V3DK_ISO2: _tcscpy(bn, _T("V3DK_ISO2")); break;
	case s3dm::V3DK_1: _tcscpy(bn, _T("V3DK_1")); break;
	case s3dm::V3DK_2: _tcscpy(bn, _T("V3DK_2")); break;
	case s3dm::V3DK_3: _tcscpy(bn, _T("V3DK_3")); break;
	case s3dm::V3DK_4: _tcscpy(bn, _T("V3DK_4")); break;
	case s3dm::V3DK_5: _tcscpy(bn, _T("V3DK_5")); break;
	case s3dm::V3DK_6: _tcscpy(bn, _T("V3DK_6")); break;
	case s3dm::V3DK_7: _tcscpy(bn, _T("V3DK_7")); break;
	case s3dm::V3DK_8: _tcscpy(bn, _T("V3DK_8")); break;
	case s3dm::V3DK_9: _tcscpy(bn, _T("V3DK_9")); break;
	case s3dm::V3DK_10: _tcscpy(bn, _T("V3DK_10")); break;
	case s3dm::V3DK_ESC: _tcscpy(bn, _T("V3DK_ESC")); break;
	case s3dm::V3DK_ALT: _tcscpy(bn, _T("V3DK_ALT")); break;
	case s3dm::V3DK_SHIFT: _tcscpy(bn, _T("V3DK_SHIFT")); break;
	case s3dm::V3DK_CTRL: _tcscpy(bn, _T("V3DK_CTRL")); break;
	case s3dm::V3DK_ROTATE: _tcscpy(bn, _T("V3DK_ROTATE")); break;
	case s3dm::V3DK_PANZOOM: _tcscpy(bn, _T("V3DK_PANZOOM")); break;
	case s3dm::V3DK_DOMINANT: _tcscpy(bn, _T("V3DK_DOMINANT")); break;
	case s3dm::V3DK_PLUS: _tcscpy(bn, _T("V3DK_PLUS")); break;
	case s3dm::V3DK_MINUS: _tcscpy(bn, _T("V3DK_MINUS")); break;
	case s3dm::V3DK_SPIN_CW: _tcscpy(bn, _T("V3DK_SPIN_CW")); break;
	case s3dm::V3DK_SPIN_CCW: _tcscpy(bn, _T("V3DK_SPIN_CCW")); break;
	case s3dm::V3DK_TILT_CW: _tcscpy(bn, _T("V3DK_TILT_CW")); break;
	case s3dm::V3DK_TILT_CCW: _tcscpy(bn, _T("V3DK_TILT_CCW")); break;
	case s3dm::V3DK_BUTTON_UI: _tcscpy(bn, _T("V3DK_BUTTON_UI")); break;

	default:
		// Put unrecognized buttons in parens
		_stprintf(bn, _T("(button %d)"), buttonnumber);
		break;
	}

	wchar_t fullStr[100];
	//wcscpy(fullStr, _T("-------------------------------------------------------"));
	//TextOut(hdc, 0, 0, fullStr, (int)_tcslen(fullStr));

	//_stprintf(fullStr, _T("%s Pressed"), bn);
	//TextOut(hdc, 0, 0, fullStr, (int)_tcslen(fullStr));
//	ofDrawBitmapString(ofToString(bn), 10, 400, 0);
	// Output to stdout for tracing
	printf("%S\n", fullStr);

	/*release our window handle */
	ReleaseDC(hWndMain, hdc);
}

/*----------------------------------------------------------------------
* Function: SbButtonReleaseEvent()
*
* Description:
*    This function receives 3D mouse button information and prints out the
*    info on screen.
*
*
* Args:
*    int     buttonnumber   //Containts number of button pressed
*
* Return Value:
*    NONE
*
*----------------------------------------------------------------------*/
void ofApp::SbButtonReleaseEvent(int buttonnumber)
{
	TCHAR bn[100];

	/* get handle of our window to draw on */
	hdc = GetDC(hWndMain);

	/* print button pressed (does not include rezero button) */
	switch (buttonnumber)
	{
	case s3dm::V3DK_MENU: _tcscpy(bn, _T("V3DK_MENU")); break;
	case s3dm::V3DK_FIT: _tcscpy(bn, _T("V3DK_FIT")); break;
	case s3dm::V3DK_TOP: _tcscpy(bn, _T("V3DK_TOP")); break;
	case s3dm::V3DK_LEFT: _tcscpy(bn, _T("V3DK_LEFT")); break;
	case s3dm::V3DK_RIGHT: _tcscpy(bn, _T("V3DK_RIGHT")); break;
	case s3dm::V3DK_FRONT: _tcscpy(bn, _T("V3DK_FRONT")); break;
	case s3dm::V3DK_BOTTOM: _tcscpy(bn, _T("V3DK_BOTTOM")); break;
	case s3dm::V3DK_BACK: _tcscpy(bn, _T("V3DK_BACK")); break;
	case s3dm::V3DK_ROLL_CW: _tcscpy(bn, _T("V3DK_ROLL_CW")); break;
	case s3dm::V3DK_ROLL_CCW: _tcscpy(bn, _T("V3DK_ROLL_CCW")); break;
	case s3dm::V3DK_ISO1: _tcscpy(bn, _T("V3DK_ISO1")); break;
	case s3dm::V3DK_ISO2: _tcscpy(bn, _T("V3DK_ISO2")); break;
	case s3dm::V3DK_1: _tcscpy(bn, _T("V3DK_1")); break;
	case s3dm::V3DK_2: _tcscpy(bn, _T("V3DK_2")); break;
	case s3dm::V3DK_3: _tcscpy(bn, _T("V3DK_3")); break;
	case s3dm::V3DK_4: _tcscpy(bn, _T("V3DK_4")); break;
	case s3dm::V3DK_5: _tcscpy(bn, _T("V3DK_5")); break;
	case s3dm::V3DK_6: _tcscpy(bn, _T("V3DK_6")); break;
	case s3dm::V3DK_7: _tcscpy(bn, _T("V3DK_7")); break;
	case s3dm::V3DK_8: _tcscpy(bn, _T("V3DK_8")); break;
	case s3dm::V3DK_9: _tcscpy(bn, _T("V3DK_9")); break;
	case s3dm::V3DK_10: _tcscpy(bn, _T("V3DK_10")); break;
	case s3dm::V3DK_ESC: _tcscpy(bn, _T("V3DK_ESC")); break;
	case s3dm::V3DK_ALT: _tcscpy(bn, _T("V3DK_ALT")); break;
	case s3dm::V3DK_SHIFT: _tcscpy(bn, _T("V3DK_SHIFT")); break;
	case s3dm::V3DK_CTRL: _tcscpy(bn, _T("V3DK_CTRL")); break;
	case s3dm::V3DK_ROTATE: _tcscpy(bn, _T("V3DK_ROTATE")); break;
	case s3dm::V3DK_PANZOOM: _tcscpy(bn, _T("V3DK_PANZOOM")); break;
	case s3dm::V3DK_DOMINANT: _tcscpy(bn, _T("V3DK_DOMINANT")); break;
	case s3dm::V3DK_PLUS: _tcscpy(bn, _T("V3DK_PLUS")); break;
	case s3dm::V3DK_MINUS: _tcscpy(bn, _T("V3DK_MINUS")); break;
	case s3dm::V3DK_SPIN_CW: _tcscpy(bn, _T("V3DK_SPIN_CW")); break;
	case s3dm::V3DK_SPIN_CCW: _tcscpy(bn, _T("V3DK_SPIN_CCW")); break;
	case s3dm::V3DK_TILT_CW: _tcscpy(bn, _T("V3DK_TILT_CW")); break;
	case s3dm::V3DK_TILT_CCW: _tcscpy(bn, _T("V3DK_TILT_CCW")); break;
	case s3dm::V3DK_BUTTON_UI: _tcscpy(bn, _T("V3DK_BUTTON_UI")); break;

	default:
		// Put unrecognized buttons in parens
		_stprintf(bn, _T("(button %d)"), buttonnumber);
		break;
	}

	wchar_t fullStr[100];
	wcscpy(fullStr, _T("--------------------------------------------------------"));
	TextOut(hdc, 0, 0, fullStr, (int)_tcslen(fullStr));

	_stprintf(fullStr, _T("%s Released"), bn);
	TextOut(hdc, 0, 0, fullStr, (int)_tcslen(fullStr));

	// Output to stdout for tracing
	printf("%S\n", fullStr);

	/*release our window handle */
	ReleaseDC(hWndMain, hdc);
}

void ofApp::HandleDeviceChangeEvent(SiSpwEvent *pEvent)
{
	TCHAR buf[100];
	hdc = GetDC(hWndMain);

	switch (pEvent->u.deviceChangeEventData.type)
	{
	case SI_DEVICE_CHANGE_CONNECT:
		_stprintf(buf, _T("Device ID %d connected"), pEvent->u.deviceChangeEventData.devID);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	case SI_DEVICE_CHANGE_DISCONNECT:
		_stprintf(buf, _T("Device ID %d disconnected"), pEvent->u.deviceChangeEventData.devID);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	default:
		_stprintf(buf, _T("Unknown deviceChangeEvent type: %d"), pEvent->u.deviceChangeEventData.type);
		TextOut(hdc, 0, 20, buf, (int)_tcslen(buf));
		break;
	}
	ReleaseDC(hWndMain, hdc);
}

void ofApp::HandleV3DCMDEvent(SiSpwEvent *pEvent)
{
	TCHAR buf[100];
	hdc = GetDC(hWndMain);
	switch (pEvent->u.cmdEventData.functionNumber)
	{
	case V3DCMD_VIEW_QZ_IN:
		_stprintf(buf, _T("V3DCMD_VIEW_QZ_IN(%s)"), pEvent->u.cmdEventData.pressed ? L"Pressed" : L"Released");
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_VIEW_QZ_OUT:
		_stprintf(buf, _T("V3DCMD_VIEW_QZ_OUT(%s)"), pEvent->u.cmdEventData.pressed ? L"Pressed" : L"Released");
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOM:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOM(ZoomFrom=%d, ZoomTo=%d, Scale=%f)"), pEvent->u.cmdEventData.iArgs[0], pEvent->u.cmdEventData.iArgs[1], pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CENTERTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CENTERTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCURSOR:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMIN_CURSORTOCURSOR(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CENTERTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CENTERTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCENTER:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCENTER(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	case V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCURSOR:
		_stprintf(buf, _T("V3DCMD_MOTIONMACRO_ZOOMOUT_CURSORTOCURSOR(Scale=%f)"), pEvent->u.cmdEventData.fArgs[0]);
		TextOut(hdc, 0, 0, buf, (int)_tcslen(buf));
		break;
	}
	ReleaseDC(hWndMain, hdc);
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0);

	

	/*ofDrawBitmapString("TX: " + ofToString(tx), 50, 50, 0);
	ofDrawBitmapString("TY: " + ofToString(ty), 50, 100, 0);
	ofDrawBitmapString("TZ: " + ofToString(tz), 50, 150, 0);
	ofDrawBitmapString("RX: " + ofToString(rx), 50, 200, 0);
	ofDrawBitmapString("RY: " + ofToString(ry), 50, 250, 0);
	ofDrawBitmapString("RZ: " + ofToString(rz), 50, 300, 0);
	ofDrawBitmapString("Device Name: " + ofToString(devicename), 50, 350, 0);*/
	
	
	DispatchLoopNT();
	//cam.begin();
	
	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);
	ofRotate(rz);
	//ofRotate(rx, ry, rz);
	

	ofSetColor(128);
	
	ofBox(tx, -ty, -tz, 51);
	
	//ofSetColor(233);
	ofSetColor(ofColor::fromHsb(ofDist(tx/2,ty/2,tz/2,0,0,0),255,255));
	ofBox( tx,  - ty, -tz, 50);
	ofPopMatrix();
	
	//cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
