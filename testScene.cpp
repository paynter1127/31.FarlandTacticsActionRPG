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

	//���� ��
	TextOut(getMemDC(), WINSIZEX / 2, 0, "�׽�Ʈ", strlen("�׽�Ʈ"));
	fade(getMemDC(), _startTime, true);
}

