#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT gameNode::init()
{
	//HDC ���
	_hdc = GetDC(_hWnd);

	KEYMANAGER->init();					//Ű�Ŵ��� �ʱ�ȭ
	RND->init();						//������� �ʱ�ȭ
	IMAGEMANAGER->init();				//�̹����Ŵ��� �ʱ�ȭ
	PLAYERDATA->init();					//�÷��̾���� �ʱ�ȭ
	TXTDATA->init();					//TXT������ �ʱ�ȭ
	INIDATA->init();					//INI������ �ʱ�ȭ
	SCENEMANAGER->init();				//���Ŵ��� �ʱ�ȭ
	SOUNDMANAGER->init();				//����Ŵ��� �ʱ�ȭ
	TIMEMANAGER->init();				//Ÿ�ӸŴ��� �ʱ�ȭ
	GDIPLUS->init();					//GDI+ �ʱ�ȭ


	return S_OK;
}


//=============================================================
//	## ���� ## release()
//=============================================================
void gameNode::release()
{
	//Ű�Ŵ��� �̱��� ����
	KEYMANAGER->release();
	KEYMANAGER->releaseSingleton();
	//������� �̱��� ����
	RND->release();
	RND->releaseSingleton();
	//�̹����Ŵ��� �̱��� ����
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//�÷��̾���� �̱��� ����
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
	//TXT������ �̱��� ����
	TXTDATA->release();
	TXTDATA->releaseSingleton();
	//INI������ �̱��� ����
	INIDATA->release();
	INIDATA->releaseSingleton();
	//���Ŵ��� �̱��� ����
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//����Ŵ��� �̱��� ����
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//Ÿ�ӸŴ��� �̱��� ����
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	//GDI �̹��� �Ŵ��� ���� �� �̱��� ����
	GDIPLUS->release();
	GDIPLUS->releaseSingleton();

	//HDC ����
	ReleaseDC(_hWnd, _hdc);
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void gameNode::update()
{
}

//=============================================================
//	## ���� ## render(HDC hdc)
//=============================================================
void gameNode::render()
{
}

//=============================================================
//	## �������ν��� ## MainProc()
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
