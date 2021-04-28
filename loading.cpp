#include "stdafx.h"
#include "loading.h"

//=============================================================
//	## loadItem ## (로드아이템)
//=============================================================
HRESULT loadItem::init(string keyName, int width, int height)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_0;

	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.widht = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_1;

	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.widht = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_2;

	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.widht = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_0;

	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.widht = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;
	
	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_1;

	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.widht = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

//=============================================================
//	## loading ## (로딩클래스)
//=============================================================
HRESULT loading::init()
{

	count_prologue = 0;

	//로딩화면 백그라운드 이미지 초기화
	IMAGEMANAGER->addImage("ved01a", "IMAGE/00_prologue/ved01a.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01b", "IMAGE/00_prologue/ved01b.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01c", "IMAGE/00_prologue/ved01c.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01d", "IMAGE/00_prologue/ved01d.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01e", "IMAGE/00_prologue/ved01e.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01f", "IMAGE/00_prologue/ved01f.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01g", "IMAGE/00_prologue/ved01g.bmp", 440, 360);
	IMAGEMANAGER->addImage("ved01h", "IMAGE/00_prologue/ved01h.bmp", 640, 360);
	
	IMAGEMANAGER->addImage("vis03a", "IMAGE/00_prologue/vis03a.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis03b", "IMAGE/00_prologue/vis03b.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis03c", "IMAGE/00_prologue/vis03c.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis10a", "IMAGE/00_prologue/vis10a.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis15a", "IMAGE/00_prologue/vis15a.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis15b", "IMAGE/00_prologue/vis15b.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis15c", "IMAGE/00_prologue/vis15c.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("vis17a", "IMAGE/00_prologue/vis17a.bmp", WINSIZEX, WINSIZEY);
	
	IMAGEMANAGER->addImage("00_title", "IMAGE/00_prologue/title.bmp", WINSIZEX, WINSIZEY);
	IMAGEMANAGER->addImage("FX_whiteWall", "IMAGE/menu_bg.bmp", WINSIZEX, WINSIZEY);
	
	

	//로딩바 클래스 초기화
	_loadingBar = new progressBar;
	//_loadingBar->init("IMAGE/loadingBarFront", "IMAGE/loadingBarBack", 100, 430, 600, 20);
	_loadingBar->init("IMAGE/loadingBarFront", "IMAGE/loadingBarBack", 20, 430, 600, 20);
	_loadingBar->setGauge(0, 0);
	//현재 게이지
	_currentGauge = 0;

	return S_OK;
}

void loading::release()
{
	//로딩바 클래스 해제
	_loadingBar->release();
	SAFE_DELETE(_loadingBar);
}

void loading::update()
{
	//로딩바 클래스 업데이트
	_loadingBar->update();
	count_prologue++;
}

void loading::render()
{
	
	//백그라운드 렌더
	alphaPrologue("ved01a", 0, DIR3);
	alphaPrologue("ved01b", 300, DIR4);
	alphaPrologue("ved01c", 600, DIR9);
	alphaPrologue("ved01d", 900, DIR8);
	alphaPrologue("ved01e", 1200, DIR1);
	alphaPrologue("ved01f", 1500, DIR6);
	alphaPrologue("ved01g", 1800, DIR7);
	alphaPrologue("ved01h", 2100, DIR2);
	
	if (700 + 0 < count_prologue && count_prologue <= 700 + 4) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 4 < count_prologue && count_prologue <= 700 + 16) IMAGEMANAGER->findImage("vis03a")->render(getMemDC());
	if (700 + 16 < count_prologue && count_prologue <= 700 + 20) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 20 < count_prologue && count_prologue <= 700 + 36) IMAGEMANAGER->findImage("vis03b")->render(getMemDC());
	if (700 + 36 < count_prologue && count_prologue <= 700 + 40) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 40 < count_prologue && count_prologue <= 700 + 56) IMAGEMANAGER->findImage("vis03c")->render(getMemDC());
	if (700 + 56 < count_prologue && count_prologue <= 700 + 60) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 60 < count_prologue && count_prologue <= 700 + 76) IMAGEMANAGER->findImage("vis10a")->render(getMemDC());
	if (700 + 76 < count_prologue && count_prologue <= 700 + 80) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 80 < count_prologue && count_prologue <= 700 + 96) IMAGEMANAGER->findImage("vis15a")->render(getMemDC());
	if (700 + 96 < count_prologue && count_prologue <= 700 + 100) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 100 < count_prologue && count_prologue <= 700 + 116) IMAGEMANAGER->findImage("vis15b")->render(getMemDC());
	if (700 + 116 < count_prologue && count_prologue <= 700 + 120) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 120 < count_prologue && count_prologue <= 700 + 136) IMAGEMANAGER->findImage("vis15c")->render(getMemDC());
	if (700 + 136 < count_prologue && count_prologue <= 700 + 140) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	if (700 + 140 < count_prologue && count_prologue <= 700 + 156) IMAGEMANAGER->findImage("vis17a")->render(getMemDC());
	if (700 + 156 < count_prologue && count_prologue <= 700 + 160) IMAGEMANAGER->findImage("FX_whiteWall")->render(getMemDC());
	alphaPrologue("00_title", 3500, DIR5);
	if(count_prologue > 990) IMAGEMANAGER->findImage("00_title")->render(getMemDC());

	//textOutDbug(getMemDC(), 580, 50, count_prologue * 2);

	//로딩바 클래스 렌더
	_loadingBar->render();
}


//이미지 이동 함수
void loading::alphaPrologue(string keyName, int countWhen, DIR_PROLOGUE DIR) //440, 360
{
	if (countWhen < count_prologue * 4 && count_prologue * 4 < countWhen + 500)
	{
		switch (DIR)
		{
		case DIR3:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), (count_prologue - countWhen / 4), (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), (count_prologue - countWhen / 4), 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR1:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, (count_prologue - countWhen / 4), (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, (count_prologue - countWhen / 4), 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR9:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), -(count_prologue - countWhen / 4) + WINSIZEY - 360, (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), -(count_prologue - countWhen / 4) + WINSIZEY - 360, 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR7:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, -(count_prologue - countWhen / 4) + WINSIZEY - 360, (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, -(count_prologue - countWhen / 4) + WINSIZEY - 360, 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR6:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), 40, (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), (count_prologue - countWhen / 4), 40, 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR4:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, 40, (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), -(count_prologue - countWhen / 4) + WINSIZEX - 440, 40, 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR8:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), 120, (count_prologue - countWhen / 4), (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), 120, (count_prologue - countWhen / 4), 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR2:
			if (count_prologue * 4 <= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), 0, -(count_prologue - countWhen / 4) + WINSIZEY - 360, (count_prologue - countWhen / 4) * 4);
			else if (count_prologue * 4 >= countWhen + 254)IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), 0, -(count_prologue - countWhen / 4) + WINSIZEY - 360, 254 - ((count_prologue - countWhen / 4) * 4) % 254);
			break;
		case DIR5:
			IMAGEMANAGER->findImage(keyName)->alphaRender(getMemDC(), 0, 0, (count_prologue - countWhen / 4) * 2);
			break;
		}
	}
}

void loading::loadImage(string keyName, int width, int height)
{
	loadItem* item = new loadItem;
	item->init(keyName, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, x, y, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

bool loading::loadingDone()
{
	//로딩완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return true;
	}
	
	loadItem* item = _vLoadItem[_currentGauge];

	switch (item->getLoadingKind())
	{
	case LOAD_KIND_IMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.widht, img.height);
		}
		break;

	case LOAD_KIND_IMAGE_1:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.widht, img.height, img.trans, img.transColor);
		}
		break;
	case LOAD_KIND_IMAGE_2:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.x, img.y, img.widht, img.height, img.trans, img.transColor);
		}
		break;
	case LOAD_KIND_FRAMEIMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.widht, img.height, img.frameX, img.frameY, img.trans, img.transColor);
		}
		break;
	case LOAD_KIND_FRAMEIMAGE_1:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.x, img.y, img.widht, img.height, img.frameX, img.frameY, img.trans, img.transColor);
		}
		break;
	
	//곧 사운드 배우고 난후 알아서...
	case LOAD_KIND_SOUND:
		break;
	}

	//현재 게이지 증가
	_currentGauge++;

	//로딩바 이미지 변경
	_loadingBar->setGauge(_currentGauge, _vLoadItem.size());

	return false;
}


