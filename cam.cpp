#include "stdafx.h"
#include "cam.h"

HRESULT cam::init()
{
	//백그라운드 이미지 초기화
	_img = IMAGEMANAGER->addImage("신세경", "신세경.bmp", WINSIZEX, WINSIZEY);

	//카메라 렉트 위치 초기화
	_rcCam = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, CAMSIZEX, CAMSIZEY);

	return S_OK;
}

void cam::release()
{
}

void cam::update()
{
	//스페이스 토글 이후 이미지 이동
	if (KEYMANAGER->isToggleKey(VK_SPACE))
	{
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			_img->setX(_img->getX() + 5.0f);
		}
		if (KEYMANAGER->isStayKeyDown('D'))
		{
			_img->setX(_img->getX() - 5.0f);
		}
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			_img->setY(_img->getY() + 5.0f);
		}
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			_img->setY(_img->getY() - 5.0f);
		}
	}
	else //카메라 이동
	{
		//카메라 이동 및 화면밖으로 나가지 않도록 처리
		if (KEYMANAGER->isStayKeyDown('A') && _rcCam.left > 0)
		{
			_rcCam.left -= 5;
			_rcCam.right -= 5;
		}
		if (KEYMANAGER->isStayKeyDown('D') && _rcCam.right < WINSIZEX)
		{
			_rcCam.left += 5;
			_rcCam.right += 5;
		}
		if (KEYMANAGER->isStayKeyDown('W') && _rcCam.top > 0)
		{
			_rcCam.top -= 5;
			_rcCam.bottom -= 5;
		}
		if (KEYMANAGER->isStayKeyDown('S') && _rcCam.bottom < WINSIZEY)
		{
			_rcCam.top += 5;
			_rcCam.bottom += 5;
		}

	}
}

void cam::render()
{
	//백그라운드
	_img->render(getMemDC(), _rcCam.left, _rcCam.top, _img->getX() + _rcCam.left, _img->getY() + _rcCam.top, CAMSIZEX, CAMSIZEY);

	//RECT rc = RectMake(100, 100, 100, 100);
	//FrameRect(getMemDC(), rc, RGB(255, 255, 0));
	//비어 있는 렉트 보여주기
	FrameRect(getMemDC(), _rcCam, RGB(255, 255, 0));

}
