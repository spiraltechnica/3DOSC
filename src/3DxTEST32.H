/*----------------------------------------------------------------------
*  sbtest32.h  -- Basic Program to init 3D mouse, read its data and 
*                  print it out.
*
*  Written By Elio Querze
*  
*
*----------------------------------------------------------------------
*
* Copyright notice:
* Copyright (c) 1998-2015 3Dconnexion. All rights reserved. 
* 
* This file and source code are an integral part of the "3Dconnexion
* Software Developer Kit", including all accompanying documentation,
* and is protected by intellectual property laws. All use of the
* 3Dconnexion Software Developer Kit is subject to the License
* Agreement found in the "LicenseAgreementSDK.txt" file.
* All rights not expressly granted by 3Dconnexion are reserved.
*
*/

static char SbTestCvsId[]="(C) 1998-2015 3Dconnexion: $Id: 3DxTEST32.H 11091 2015-01-09 11:02:45Z jwick $";
/* Global Variables */

#ifndef SBTEST32_H
#define SBTEST32_H

HDC          hdc;         /* Handle to Device Context used to draw on screen */
HWND         hWndMain;    /* Handle to Main Window */

//#define TEST_MULTIPLECONNECTIONS
#ifdef  TEST_MULTIPLECONNECTIONS
// For testing reconnection on driver bounce for Bug 6812
HWND         hWndMain1,hWndMain2,hWndMain3;    /* Extra windows for testing */
SiHdl        devHdl1,devHdl2,devHdl3;      /* Extra Handle to 3D Mouse Device */
#endif

SiHdl        devHdl;      /* Handle to 3D Mouse Device */
TCHAR devicename[100] = _T("");

/* Functions --See C File for Additional Details */

LRESULT  WINAPI HandleNTEvent (HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);
int   DispatchLoopNT(); 
void  CreateSPWindow(int, int, int, int, TCHAR *);
int   SbInit();
void  SbMotionEvent(SiSpwEvent *pEvent);
void  SbZeroEvent();
void  SbButtonPressEvent(int buttonnumber);
void  SbButtonReleaseEvent(int buttonnumber);
void  HandleDeviceChangeEvent(SiSpwEvent *pEvent);
void  HandleV3DCMDEvent(SiSpwEvent *pEvent);
#endif
