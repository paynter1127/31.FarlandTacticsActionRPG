#include "stdafx.h"
#include "titleScene.h"

HRESULT titleScene::init()
{
	SOUNDMANAGER->play("bgm01", 0.2f);

	for (int i = 0; i < 5; i++)
	{
		rc_btn[i] = RectMake(470, 260 + 40 * i, 152, 36);
	}
	IMAGEMANAGER->addImage("title_btn_white", "IMAGE/menu_bg.bmp", rc_btn[0].right - rc_btn[0].left, rc_btn[0].bottom - rc_btn[0].top);
	IMAGEMANAGER->addImage("title_btn_black", "IMAGE/shadow.bmp", rc_btn[0].right - rc_btn[0].left, rc_btn[0].bottom - rc_btn[0].top);
	
	rc_option = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 480, 360);
	isPop_option = false;
	rc_option_close = RectMakeCenter(480, 380, 120, 36);
	IMAGEMANAGER->addImage("rc_option_close", "IMAGE/menu_bg.bmp", rc_option_close.right - rc_option_close.left, rc_option_close.bottom - rc_option_close.top);
	
	rc_option_button[0] = RectMakeCenter(240, 196, 30, 30);
	rc_option_button[1] = RectMakeCenter(500, 196, 30, 30);
	rc_option_button[2] = RectMakeCenter(500, 252, 30, 30);
	rc_option_button[3] = RectMakeCenter(500, 312, 30, 30);

	//체력바 클래스 초기화
	hpMax = 400;
	hp = 300;
	hpBar = new progressBar;
	hpBar->init("IMAGE/ui/boss_hpBar_0.1496.128", "IMAGE/ui/castle_hpBar_back.1496.128.2", 260, 181, 220, 30);
	hpBar->setGauge(hp, hpMax);

	check[0] = true;
	check[1] = true;

	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void titleScene::release()
{
}

void titleScene::update()
{
	//플레이어 체력바 업데이트
	hpBar->update();
	hpBar->setGauge(hp, hpMax);

	if (!isPop_option)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rc_btn[0], _ptMouse))
			{
				SOUNDMANAGER->play("meter");
				//로드 팝업창 출력
				if (MessageBox(_hWnd, "새 게임을 시작 하시겠습니까?", "NEW GAME", MB_YESNO) == IDYES)
				{
					SOUNDMANAGER->play("find", 0.4f);
					SCENEMANAGER->loadScene("마을");
				}
			}
			else if (PtInRect(&rc_btn[1], _ptMouse))
			{
				SOUNDMANAGER->play("meter");
				//로드 팝업창 출력
				if (MessageBox(_hWnd, "이전 저장한 시점에서 계속 하시겠습니까?", "CONTINUE", MB_YESNO) == IDYES)
				{
					if (MessageBox(_hWnd, "데이터를 불러 왔습니다.", "LOAD", MB_OK) == IDOK)
					{
						//데이터 불러오기
						//마을로 씬 이동
						SOUNDMANAGER->play("find", 0.4f);
						SCENEMANAGER->loadScene("일반필드");
					}
				}
			}
			else if (PtInRect(&rc_btn[2], _ptMouse))
			{
				SOUNDMANAGER->play("meter");
				//옵션 팝업창 출력
				isPop_option = true;
			}
			else if (PtInRect(&rc_btn[3], _ptMouse))
			{
				SOUNDMANAGER->play("meter");
				//종료 팝업창 출력
				if (MessageBox(_hWnd, "맵 수정을 위해 이동 하시겠습니까?", "MAP TOOL", MB_YESNO) == IDYES)
				{
					SOUNDMANAGER->play("find", 0.4f);
					SCENEMANAGER->loadScene("맵툴");
				}
			}
			else if (PtInRect(&rc_btn[4], _ptMouse))
			{
				SOUNDMANAGER->play("meter");
				//종료 팝업창 출력
				if (MessageBox(_hWnd, "게임을 종료 하시겠습니까?", "EXIT", MB_YESNO) == IDYES)
				{
					SOUNDMANAGER->play("find", 0.4f);
					//SCENEMANAGER->loadScene("보스필드");
					PostMessage(_hWnd, WM_DESTROY, 0, 0);
				}
			}
		}
	}


	//옵션창 기능 처리
	if (isPop_option)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rc_option_close, _ptMouse)) //닫기
			{
				SOUNDMANAGER->play("meter");
				isPop_option = false;
			}
			if (PtInRect(&rc_option_button[2], _ptMouse))
			{
				if (!SOUNDMANAGER->isPlaySound("meter")) SOUNDMANAGER->play("meter");
				if (!check[0])
				{
					check[0] = true;
					if (SOUNDMANAGER->isPauseSound("bgm01")) SOUNDMANAGER->resume("bgm01");
				}
				else if (check[0])
				{
					check[0] = false;
					if (SOUNDMANAGER->isPlaySound("bgm01")) SOUNDMANAGER->pause("bgm01");
				}
			}
			if (PtInRect(&rc_option_button[3], _ptMouse))
			{
				if (!SOUNDMANAGER->isPlaySound("meter")) SOUNDMANAGER->play("meter");
				if (!check[1]) check[1] = true;
				else if (check[1]) check[1] = false;
			}
		}

		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rc_option_button[0], _ptMouse)) //다운
			{
				if(!SOUNDMANAGER->isPlaySound("meter")) SOUNDMANAGER->play("meter");
				hp--;
			}
			if (PtInRect(&rc_option_button[1], _ptMouse)) //업
			{
				if (!SOUNDMANAGER->isPlaySound("meter")) SOUNDMANAGER->play("meter");
				hp++;
			}
		}
	}


	//사운드 테스트
	if (KEYMANAGER->isOnceKeyDown(VK_UP)) SOUNDMANAGER->play("bgm02", 0.1f);
}

void titleScene::render()
{
	IMAGEMANAGER->findImage("01_title")->render(getMemDC());
	IMAGEMANAGER->findImage("01_title.500.108")->render(getMemDC(), 70, 64);

	//버튼 출력
	{
		for (int i = 0; i < 5; i++)
		{
			IMAGEMANAGER->findImage("title_btn_white")->alphaRender(getMemDC(), rc_btn[i].left, rc_btn[i].top, 128);
			FrameRect(getMemDC(), rc_btn[i], RGB(200, 200, 200));
		}

		textOutFont(getMemDC(), rc_btn[0].left + 10, rc_btn[0].top + 4, "NEWGAME", 30);
		textOutFont(getMemDC(), rc_btn[1].left + 44, rc_btn[1].top + 4, "LOAD", 30);
		textOutFont(getMemDC(), rc_btn[2].left + 22, rc_btn[2].top + 4, "OPTION", 30);
		textOutFont(getMemDC(), rc_btn[3].left + 10, rc_btn[3].top + 4, "MAPTOOL", 30);
		textOutFont(getMemDC(), rc_btn[4].left + 46, rc_btn[4].top + 4, "EXIT", 30);

		if (!isPop_option)
		{
			for (int i = 0; i < 5; i++)
			{
				if (PtInRect(&rc_btn[i], _ptMouse))
				{
					IMAGEMANAGER->findImage("title_btn_black")->alphaRender(getMemDC(), rc_btn[i].left, rc_btn[i].top, 220);
				}
				FrameRect(getMemDC(), rc_btn[i], RGB(200, 200, 200));
			}

			if (PtInRect(&rc_btn[0], _ptMouse))
			{
				textOutFont(getMemDC(), rc_btn[0].left + 6, rc_btn[0].top + 4, "NEWGAME", 32, RGB(180, 180, 180));
			}
			else if (PtInRect(&rc_btn[1], _ptMouse))
			{
				textOutFont(getMemDC(), rc_btn[1].left + 42, rc_btn[1].top + 4, "LOAD", 32, RGB(180, 180, 180));
			}
			else if (PtInRect(&rc_btn[2], _ptMouse))
			{
				textOutFont(getMemDC(), rc_btn[2].left + 20, rc_btn[2].top + 4, "OPTION", 32, RGB(180, 180, 180));
			}
			else if (PtInRect(&rc_btn[3], _ptMouse))
			{
				textOutFont(getMemDC(), rc_btn[3].left + 6, rc_btn[3].top + 4, "MAPTOOL", 32, RGB(180, 180, 180));
			}
			else if (PtInRect(&rc_btn[4], _ptMouse))
			{
				textOutFont(getMemDC(), rc_btn[4].left + 46, rc_btn[4].top + 4, "EXIT", 32, RGB(180, 180, 180));
			}
		}

		//옵션 창 렌더
		if (isPop_option)
		{
			drawPopUp(getMemDC(), rc_option, RGB(20, 20, 20), true);
			IMAGEMANAGER->findImage("rc_option_close")->alphaRender(getMemDC(), rc_option_close.left, rc_option_close.top, 128);
			FrameRect(getMemDC(), rc_option_close, RGB(200, 200, 200));
			textOutFont(getMemDC(), rc_option_close.left + 16, rc_option_close.top + 4, "CLOSE", 30);

			hpBar->render();
			Rectangle(getMemDC(), rc_option_button[0]);
			Rectangle(getMemDC(), rc_option_button[1]);
			Rectangle(getMemDC(), rc_option_button[2]);
			Rectangle(getMemDC(), rc_option_button[3]);
			textOutFont(getMemDC(), rc_option_button[0].left + 4, rc_option_button[0].top, "◀", 30, RGB(0, 0, 0));
			textOutFont(getMemDC(), rc_option_button[1].left + 2, rc_option_button[1].top, "▶", 30, RGB(0, 0, 0));
			
			if (check[0]) textOutFont(getMemDC(), rc_option_button[2].left - 5, rc_option_button[2].top - 20, "Ⅴ", 50, RGB(200, 200, 55));
			else if (!check[0])textOutFont(getMemDC(), rc_option_button[2].left - 5, rc_option_button[2].top - 20, " ", 50, RGB(200, 200, 55));

			if (check[1]) textOutFont(getMemDC(), rc_option_button[3].left - 5, rc_option_button[3].top - 20, "Ⅴ", 50, RGB(200, 200, 55));
			else if (!check[1]) textOutFont(getMemDC(), rc_option_button[3].left - 5, rc_option_button[3].top - 20, " ", 50, RGB(200, 200, 55));

			textOutFont(getMemDC(), 100, 80, "『OPTION』", 30, RGB(255, 255, 255), "돋움");
			textOutFont(getMemDC(), 100, 180, "Volum", 30, RGB(255, 255, 255), "돋움");
			textOutFont(getMemDC(), 100, 240, "BGM On / OFF", 30, RGB(255, 255, 255), "돋움");
			textOutFont(getMemDC(), 100, 300, "Sound effect On / OFF", 30, RGB(255, 255, 255), "돋움");
		}
	}

	
	fade(getMemDC(), _startTime, true);
}
