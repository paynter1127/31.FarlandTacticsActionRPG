#include "stdafx.h"
#include "testScene.h"

HRESULT testScene::init()
{
	

	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void testScene::release()
{

}

void testScene::update()
{
	
}

void testScene::render()
{

	SetBkMode(getMemDC(), 2);

	//현재 씬
	TextOut(getMemDC(), WINSIZEX / 2, 0, "테스트", strlen("테스트"));
	fade(getMemDC(), _startTime, true);
}

