/* < 참고 >
11.14
	- 선생님 프레임 워크에 내 프레임 워크 옮기기
	- A* : 길찾기
	- Z오더(Ysort 와 동일 개념인듯 합니다.)
	- 클리핑
	- Flood fill 알고리즘(Seed fill) : 같은 상태의 인접한 오브젝트 찾기(맵툴 업그레이드)
	- 광원(전장의 안개)

10.31
	- 타임 매니저 추가
	- GDI+ 매니저 추가
	- 알파 렌더 자르는 부분 코드 누락 부 추가

10.20
	쌤이 쓰셨던 버그 수정 적용 버전
	- fmod 사운드 매니저 내 pause 기능쪽 함수 틀린 것 수정
	- 이미지 매니저 폴더 image 로 변경
	- 사운드 매니저 폴더 SOUND 로 변경
	- 이미지 숫자 함수 제작
	- 디버그용 함수 제작(마우스, 숫자)
	- 이미지 클래스 마젠타 디폴트 매개변수로 변경
	- 이미지 클래스 루프 알파 렌더 쌤 오류난 것 수정 버전
	- 통합한 코드 안정성 검사
*/

// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include <SDKDDKVer.h>

//디버깅용 (주석치면 콘솔창이 사라진다)
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <Windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
// C++ 런타임 헤더 파일입니다.
#include <iostream>
using namespace std;
// 자주사용하는 STL
#include <string>
#include <vector>
#include <map>
#include <list>


//=============================================================
//	## 내가 만든 헤더파일을 이곳에 추가한다 ##
//=============================================================
#include "commonMacroFunction.h"
#include "keyManager.h"
#include "randomFunction.h"
#include "imageManager.h"
#include "playerData.h"
#include "txtData.h"
#include "iniData.h"
#include "sceneManager.h"
#include "soundManager.h"
#include "timeManager.h"
// 유틸
#include "util.h"
using namespace MY_UTIL;
// GDI+
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus")
using namespace Gdiplus;
#include "gdipManager.h"


//=============================================================
//	## 싱글톤을 추가한다 ##
//=============================================================
#define KEYMANAGER keyManager::getSingleton()
#define RND randomFunction::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define PLAYERDATA playerData::getSingleton()
#define TXTDATA	txtData::getSingleton()
#define INIDATA iniData::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define GDIPLUS	gdipManager::getSingleton()

//=============================================================
//	## 디파인문 ## (윈도우창 초기화)
//=============================================================
#define WINNAME (LPTSTR)(TEXT(" programing by Paynter "))
#define WINSTARTX	640
#define WINSTARTY	260
#define WINSIZEX		640
#define WINSIZEY		480
#define WINSTYLE		WS_CAPTION | WS_SYSMENU

//=============================================================
//	## 매크로함수 ## (클래스에서 동적할당된 부분 해제)
//=============================================================
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}

//=============================================================
//	## 전역변수 ##
//=============================================================
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;