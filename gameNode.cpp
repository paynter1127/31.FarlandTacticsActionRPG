#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT gameNode::init()
{
	//HDC 얻기
	_hdc = GetDC(_hWnd);

	KEYMANAGER->init();					//키매니져 초기화
	RND->init();						//랜덤펑션 초기화
	IMAGEMANAGER->init();				//이미지매니져 초기화
	PLAYERDATA->init();					//플레이어데이터 초기화
	TXTDATA->init();					//TXT데이터 초기화
	INIDATA->init();					//INI데이터 초기화
	SCENEMANAGER->init();				//씬매니져 초기화
	SOUNDMANAGER->init();				//사운드매니져 초기화
	TIMEMANAGER->init();				//타임매니져 초기화
	GDIPLUS->init();					//GDI+ 초기화


	return S_OK;
}


//=============================================================
//	## 해제 ## release()
//=============================================================
void gameNode::release()
{
	//키매니져 싱글톤 해제
	KEYMANAGER->release();
	KEYMANAGER->releaseSingleton();
	//랜덤펑션 싱글톤 해제
	RND->release();
	RND->releaseSingleton();
	//이미지매니져 싱글톤 해제
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//플레이어데이터 싱글톤 해제
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
	//TXT데이터 싱글톤 해제
	TXTDATA->release();
	TXTDATA->releaseSingleton();
	//INI데이터 싱글톤 해제
	INIDATA->release();
	INIDATA->releaseSingleton();
	//씬매니져 싱글톤 해제
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//사운드매니져 싱글톤 해제
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//타임매니져 싱글톤 해제
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	//GDI 이미지 매니저 해제 및 싱글톤 해제
	GDIPLUS->release();
	GDIPLUS->releaseSingleton();

	//HDC 해제
	ReleaseDC(_hWnd, _hdc);
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void gameNode::update()
{
}

//=============================================================
//	## 렌더 ## render(HDC hdc)
//=============================================================
void gameNode::render()
{
}

//=============================================================
//	## 메인프로시져 ## MainProc()
//=============================================================
LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;		 
	PAINTSTRUCT ps; 

	switch (iMessage)
	{
	case WM_MOUSEMOVE:		
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:		
		switch (wParam)
		{
		case VK_ESCAPE:	
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
