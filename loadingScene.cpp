#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	SOUNDMANAGER->addSound("bgm02", "SOUND/bgm02(로드창).mid", true, true);
	//SOUNDMANAGER->play("bgm02");
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//이미지 및 사운드 로딩
	this->loadingImage();
	this->loadingSound();

	IMAGEMANAGER->addImage("menu_bg", "menu_bg.bmp", 640, 480);
	IMAGEMANAGER->addFrameImage("숫자2", "IMAGE/숫자2.bmp", 360 / 1.2, 42 / 1.2, 10, 1, true, RGB(255, 0, 255));
	i = 0;

	//로딩
	//SOUNDMANAGER->addSound("256113__nckn__done", "256113__nckn__done.wav", false, false);
	
	

	//isDone = false;

	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		count_frame++;

		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			SOUNDMANAGER->play("버튼");
			//SOUNDMANAGER->stop("bgm02");
			//SOUNDMANAGER->pause("bgm02");
			
			//loadPlayer();
			SCENEMANAGER->loadScene("타이틀");

		}
	}
}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();
	imageNum("숫자2", getMemDC(), 600, 424, (_loading->getCurrentGauge() * 100) / _loading->getLoadItem().size(), 20); //완료

	//파일 이름 표시
	if (IMAGEMANAGER->findImage(_loading->getLoadItem()[i]->getImageResource().keyName))
	{
		textOutFont(getMemDC(), 30, 434, IMAGEMANAGER->findImage(_loading->getLoadItem()[i]->getImageResource().keyName)->getFileName(), 16);
		if ((_loading->getCurrentGauge() * 100) / _loading->getLoadItem().size() == 99) 
		{
			//SOUNDMANAGER->play("256113__nckn__done");
			i++;
		}
		else if((_loading->getCurrentGauge() * 100) / _loading->getLoadItem().size() != 100) i++;
		else
		{
			//로딩
			if ((count_frame / 10) % 2 == 0)
			{
				textOutFont(getMemDC(), _ptMouse.x + 10, _ptMouse.y, "Click to", 40, RGB(250, 200, 0));
				textOutFont(getMemDC(), _ptMouse.x + 10, _ptMouse.y + 30, "Continue", 40, RGB(250, 200, 0));
			}
		}
	}

	

}

/* < 여기에 각 씬에 출력할 이미지 미리 로드 해두는 곳입니다. >*/
void loadingScene::loadingImage()
{
	//< 00_UI, NPC 이미지 로드 > ---------------------------------------------------------------------------------------------
	{
		_loading->loadImage("마우스테스트", "IMAGE/menu_bg.bmp", 3, 3);
		_loading->loadImage("빈이미지", "IMAGE/빈이미지.bmp", 3, 3);
		_loading->loadImage("fade", "IMAGE/shadow.bmp", WINSIZEX, WINSIZEY);

		_loading->loadFrameImage("숫자이미지1", "IMAGE/numSprite.bmp", 360, 42, 10, 1);
		_loading->loadFrameImage("숫자이미지2", "IMAGE/숫자2.bmp", 360, 42, 10, 1, true, RGB(255, 0, 255));

		_loading->loadFrameImage("숫자이미지2", "IMAGE/숫자2.bmp", 360, 42, 10, 1, true, RGB(255, 0, 255));

		_loading->loadImage("popUpFrame_01", "IMAGE/ui/popUpFrame_01.18.18.bmp", 18, 18);
		_loading->loadImage("popUpFrame_02", "IMAGE/ui/popUpFrame_02.18.18.bmp", 18, 18);
		_loading->loadImage("popUpFrame_03", "IMAGE/ui/popUpFrame_03.18.18.bmp", 18, 18);
		_loading->loadImage("popUpFrame_04", "IMAGE/ui/popUpFrame_04.18.18.bmp", 18, 18);
		_loading->loadImage("popUpFrame_11", "IMAGE/ui/popUpFrame_11.18.1.bmp", 18, 1);
		_loading->loadImage("popUpFrame_12", "IMAGE/ui/popUpFrame_12.1.18.bmp", 1, 18);
		_loading->loadImage("popUpFrame_13", "IMAGE/ui/popUpFrame_13.18.1.bmp", 18, 1);
		_loading->loadImage("popUpFrame_14", "IMAGE/ui/popUpFrame_14.1.18.bmp", 1, 18);

		_loading->loadFrameImage("포탈", "IMAGE/ui/mare5.bmp", 640 * 0.8, 208 * 0.8, 10, 1, true, RGB(0, 0, 255));
		
		_loading->loadFrameImage("talkBalloon", "IMAGE/ui/talkBalloon.86.20.bmp", 86, 20, 2, 1, true, RGB(255, 0, 0));
		_loading->loadImage("talkBG", "IMAGE/ui/talkBG.590.100.bmp", 590, 100, true, RGB(255, 0, 0));
		
		

		//npc
		_loading->loadFrameImage("npc조심해", "IMAGE/char_npc/npc_01.256.68.3.bmp", 256, 68, 3, 1);
		_loading->loadFrameImage("npc카운터아저씨", "IMAGE/char_npc/npc_02.bmp", 192, 72, 4, 1);
		_loading->loadFrameImage("npc카운터아가씨", "IMAGE/char_npc/npc_05.bmp", 160, 70, 4, 1);
		_loading->loadFrameImage("npc세이브", "IMAGE/char_npc/char_04c.bmp", 192, 82, 3, 1);
		

		//hud
		_loading->loadImage("uiBar", "IMAGE/ui/uiBar.400.120.bmp", 400, 120, true, RGB(255, 0, 255));
		_loading->loadImage("uiBar2", "IMAGE/ui/uiBar2.400.120.bmp", 400, 120, true, RGB(255, 0, 255));
		_loading->loadImage("hpBar", "IMAGE/ui/hpBar.322.8.bmp", 322, 8, true, RGB(255, 0, 255));
		_loading->loadImage("mpBar", "IMAGE/ui/mpBar.322.8.bmp", 322, 8, true, RGB(255, 0, 255));
		_loading->loadImage("face07a", "IMAGE/ui/face07a.bmp", 120, 96, true, RGB(255, 255, 255));
		_loading->loadImage("face07c", "IMAGE/ui/face07c.bmp", 120, 96, true, RGB(255, 255, 255));
		_loading->loadImage("face07d", "IMAGE/ui/face07d.bmp", 120, 96, true, RGB(255, 255, 255));
		_loading->loadImage("face07j", "IMAGE/ui/face07j.bmp", 120, 96, true, RGB(255, 255, 255));
		_loading->loadImage("skill_frame", "IMAGE/ui/skill_frame.26.26.bmp", 52, 52, true, RGB(255, 0, 255));
		_loading->loadImage("skill_bg", "IMAGE/shadow.bmp", 590, 100); //RectMakeCenter(WINSIZEX / 2, 400, 590, 100)
		_loading->loadImage("skill_icon1", "IMAGE/ui/skill_icon1.22.22.bmp", 44, 44, true, RGB(255, 0, 255));
		_loading->loadImage("skill_icon2", "IMAGE/ui/skill_icon2.22.22.bmp", 44, 44, true, RGB(255, 0, 255));

		//적 체력바
		_loading->loadImage("hpBar_back_enemy", "IMAGE/ui/castle_hpBar_back.1496.128.bmp", 32, 6, true, RGB(255, 0, 255));
		_loading->loadImage("hpBar_front_enemy", "IMAGE/ui/castle_hpBar_1.1496.128.bmp", 32, 6, true, RGB(255, 0, 255));
		_loading->loadImage("hpBar_back_boss", "IMAGE/ui/boss_hpBar_back.1496.128.bmp", 64, 8, true, RGB(255, 0, 255));
		_loading->loadImage("hpBar_front_boss_1", "IMAGE/ui/boss_hpBar_1.1496.128.bmp", 64, 8, true, RGB(255, 0, 255));
		_loading->loadImage("hpBar_front_boss_0", "IMAGE/ui/boss_hpBar_0.1496.128.bmp", 64, 8, true, RGB(255, 0, 255));

	}

	//< 01_타이틀 이미지 로드 > ---------------------------------------------------------------------------------------------
	{
		_loading->loadImage("01_title", "IMAGE/01_title/v09a.bmp", WINSIZEX, WINSIZEY);
		_loading->loadImage("01_title.500.108", "IMAGE/01_title/01_title.500.108.bmp", 500, 108);
		
	}

	//< 02_게임씬 이미지 로드 > ---------------------------------------------------------------------------------------------
	{

		//유저 캐릭터
		_loading->loadFrameImage("idle", "IMAGE/char_user/idle.195.68.bmp", 195, 68, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("run1", "IMAGE/char_user/run.660.134_01.bmp", 330, 67, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("run2", "IMAGE/char_user/run.660.134_02.bmp", 330, 67, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("run3", "IMAGE/char_user/run.660.134_03.bmp", 330, 67, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("run4", "IMAGE/char_user/run.660.134_04.bmp", 330, 67, 4, 1, true, RGB(255, 0, 255));


		//에너미 좀비
		_loading->loadFrameImage("zombie_idle", "IMAGE/char_monster/zombie_idle.128.96.bmp", 128, 96, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_beHit", "IMAGE/char_monster/zombie_beHit.64.96.bmp", 64, 96, 1, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_run1", "IMAGE/char_monster/zombie_run.256.96_01.bmp", 256, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_run2", "IMAGE/char_monster/zombie_run.256.96_02.bmp", 256, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_run3", "IMAGE/char_monster/zombie_run.256.96_03.bmp", 256, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_run4", "IMAGE/char_monster/zombie_run.256.96_04.bmp", 256, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_attack1", "IMAGE/char_monster/zombie_attack.576.96_01.bmp", 576, 96, 8, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_attack2", "IMAGE/char_monster/zombie_attack.576.96_02.bmp", 576, 96, 8, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_attack3", "IMAGE/char_monster/zombie_attack.576.96_03.bmp", 576, 96, 8, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("zombie_attack4", "IMAGE/char_monster/zombie_attack.576.96_04.bmp", 576, 96, 8, 1, true, RGB(255, 0, 255));


		//에너미 리치킹
		_loading->loadFrameImage("boss_idle", "IMAGE/char_monster/richKing_idle.256.96.bmp", 256, 96, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_run1", "IMAGE/char_monster/richKing_run.384.96_01.bmp", 384, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_run2", "IMAGE/char_monster/richKing_run.384.96_02.bmp", 384, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_run3", "IMAGE/char_monster/richKing_run.384.96_03.bmp", 384, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_run4", "IMAGE/char_monster/richKing_run.384.96_04.bmp", 384, 96, 4, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_attack1", "IMAGE/char_monster/richking_attack.384.128_01.bmp", 384, 128, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_attack2", "IMAGE/char_monster/richking_attack.384.128_02.bmp", 384, 128, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_attack3", "IMAGE/char_monster/richking_attack.384.128_03.bmp", 384, 128, 3, 1, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_attack4", "IMAGE/char_monster/richking_attack.384.128_04.bmp", 384, 128, 3, 1, true, RGB(255, 0, 255));
		
		//리치킹 스킬
		_loading->loadFrameImage("boss_thunder1", "IMAGE/skill/boss_thunder1.384.384.6.bmp", 384, 384, 6, 1, true, RGB(255, 0, 0));
		_loading->loadFrameImage("boss_thunder2", "IMAGE/skill/boss_thunder2.384.384.6.bmp", 384, 384, 6, 1, true, RGB(255, 0, 0));
		_loading->loadFrameImage("boss_magicCircle_big", "IMAGE/skill/boss_magicCircle_big.256.720.5.bmp", 256, 720, 1, 5, true, RGB(255, 0, 0));
		_loading->loadFrameImage("boss_magicCircle_small", "IMAGE/skill/boss_magicCircle_small.80.912.19.bmp", 80, 912, 1, 19, true, RGB(255, 0, 255));
		_loading->loadFrameImage("boss_fire", "IMAGE/skill/boss_fire.1260.224.14.bmp", 1260 / 2, 224 / 2, 14, 1, true, RGB(0, 255, 0));

		
	}

	//< 03_맵툴 이미지 로드 > ---------------------------------------------------------------------------------------------
	{
		//land1 타일 로딩
		{
			//타일
			_loading->loadImage("st51a_01", "IMAGE/mapTool/tile/st51a_01.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_02", "IMAGE/mapTool/tile/st51a_02.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_03", "IMAGE/mapTool/tile/st51a_03.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_04", "IMAGE/mapTool/tile/st51a_04.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_08", "IMAGE/mapTool/tile/st51a_08.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_09", "IMAGE/mapTool/tile/st51a_09.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_10", "IMAGE/mapTool/tile/st51a_10.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_11", "IMAGE/mapTool/tile/st51a_11.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_15", "IMAGE/mapTool/tile/st51a_15.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_16", "IMAGE/mapTool/tile/st51a_16.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_17", "IMAGE/mapTool/tile/st51a_17.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_18", "IMAGE/mapTool/tile/st51a_18.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_22", "IMAGE/mapTool/tile/st51a_22.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_23", "IMAGE/mapTool/tile/st51a_23.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_24", "IMAGE/mapTool/tile/st51a_24.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_25", "IMAGE/mapTool/tile/st51a_25.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_30", "IMAGE/mapTool/tile/st51a_30.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_31", "IMAGE/mapTool/tile/st51a_31.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_32", "IMAGE/mapTool/tile/st51a_32.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_37", "IMAGE/mapTool/tile/st51a_37.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_38", "IMAGE/mapTool/tile/st51a_38.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_39", "IMAGE/mapTool/tile/st51a_39.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_45", "IMAGE/mapTool/tile/st51a_45.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_46", "IMAGE/mapTool/tile/st51a_46.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_52", "IMAGE/mapTool/tile/st51a_52.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_53", "IMAGE/mapTool/tile/st51a_53.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_59", "IMAGE/mapTool/tile/st51a_59.bmp", 64, 32, true, RGB(0, 0, 255));
			_loading->loadImage("st51a_60", "IMAGE/mapTool/tile/st51a_60.bmp", 64, 32, true, RGB(0, 0, 255));

			//미니맵 타일
			_loading->loadImage("mini_st51a_01", "IMAGE/mapTool/tile/st51a_01.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_02", "IMAGE/mapTool/tile/st51a_02.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_03", "IMAGE/mapTool/tile/st51a_03.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_04", "IMAGE/mapTool/tile/st51a_04.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_08", "IMAGE/mapTool/tile/st51a_08.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_09", "IMAGE/mapTool/tile/st51a_09.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_10", "IMAGE/mapTool/tile/st51a_10.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_11", "IMAGE/mapTool/tile/st51a_11.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_15", "IMAGE/mapTool/tile/st51a_15.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_16", "IMAGE/mapTool/tile/st51a_16.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_17", "IMAGE/mapTool/tile/st51a_17.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_18", "IMAGE/mapTool/tile/st51a_18.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_22", "IMAGE/mapTool/tile/st51a_22.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_23", "IMAGE/mapTool/tile/st51a_23.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_24", "IMAGE/mapTool/tile/st51a_24.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_25", "IMAGE/mapTool/tile/st51a_25.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_30", "IMAGE/mapTool/tile/st51a_30.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_31", "IMAGE/mapTool/tile/st51a_31.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_32", "IMAGE/mapTool/tile/st51a_32.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_37", "IMAGE/mapTool/tile/st51a_37.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_38", "IMAGE/mapTool/tile/st51a_38.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_39", "IMAGE/mapTool/tile/st51a_39.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_45", "IMAGE/mapTool/tile/st51a_45.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_46", "IMAGE/mapTool/tile/st51a_46.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_52", "IMAGE/mapTool/tile/st51a_52.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_53", "IMAGE/mapTool/tile/st51a_53.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_59", "IMAGE/mapTool/tile/st51a_59.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_st51a_60", "IMAGE/mapTool/tile/st51a_60.bmp", 64 / 10, 32 / 10, true, RGB(0, 0, 255));
		}

		//이동 불가 타일 표시
		_loading->loadImage("tile_block", "IMAGE/mapTool/tile_block.bmp", 64, 32);

		//스킬 범위 타일 표시
		_loading->loadImage("skill_range", "IMAGE/skill/skill_range.bmp", 64, 32, true, RGB(255, 0, 0));
		_loading->loadImage("skill_hitRange", "IMAGE/skill/skill_hitRange.bmp", 64, 32, true, RGB(255, 0, 0));
		_loading->loadFrameImage("skill_meteo", "IMAGE/skill/skill_meteo.5952.192.31.bmp", 5952, 192, 31, 1, true, RGB(0, 0, 255));
		_loading->loadFrameImage("skill_thunder", "IMAGE/skill/skill_thunder.256.400.4.bmp", 256, 400, 4, 1, true, RGB(255, 0, 0));
		_loading->loadFrameImage("skill_thunder2", "IMAGE/skill/skill_thunder2.896.128.7.bmp", 896, 128, 7, 1, true, RGB(255, 0, 0));

		

		//오브젝트 타일 로드
		{
			_loading->loadImage("sign_2_04", "IMAGE/mapTool/obj/sign_2_04.bmp", 64, 95, true, RGB(0, 0, 255));
			_loading->loadImage("sign_2_05", "IMAGE/mapTool/obj/sign_2_05.bmp", 64, 95, true, RGB(0, 0, 255));
			_loading->loadImage("sign_2_06", "IMAGE/mapTool/obj/sign_2_06.bmp", 64, 95, true, RGB(0, 0, 255));
			_loading->loadImage("tree.64.138", "IMAGE/mapTool/obj/tree.64.138.bmp", 64, 138, true, RGB(0, 0, 255));
			_loading->loadImage("bush1.64.55", "IMAGE/mapTool/obj/bush1.64.55.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush2.64.45", "IMAGE/mapTool/obj/bush2.64.45.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush3.64.48", "IMAGE/mapTool/obj/bush3.64.48.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush4.64.37", "IMAGE/mapTool/obj/bush4.64.37.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush5.64.58", "IMAGE/mapTool/obj/bush5.64.58.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush6.64.48", "IMAGE/mapTool/obj/bush6.64.48.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush7.64.45", "IMAGE/mapTool/obj/bush7.64.45.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("bush8.64.37", "IMAGE/mapTool/obj/bush8.64.37.bmp", 64, 55, true, RGB(0, 0, 255));
			_loading->loadImage("barrel.64.58", "IMAGE/mapTool/obj/barrel.64.58.bmp", 64, 58, true, RGB(0, 0, 255));
			_loading->loadImage("palmtree1.440.252", "IMAGE/mapTool/obj/palmtree1.440.252.bmp", 440, 252, true, RGB(255, 0, 255));
			_loading->loadImage("palmtree2.344.318", "IMAGE/mapTool/obj/palmtree2.344.318.bmp", 344, 318, true, RGB(255, 0, 255));
			_loading->loadImage("st50a", "IMAGE/mapTool/obj/st50a.bmp", 384, 328, true, RGB(255, 0, 0));
			_loading->loadImage("st50b", "IMAGE/mapTool/obj/st50b.bmp", 384, 328, true, RGB(255, 0, 0));
			_loading->loadImage("st50c", "IMAGE/mapTool/obj/st50c.bmp", 384, 328, true, RGB(255, 0, 0));
			_loading->loadImage("st02a", "IMAGE/mapTool/obj/st02a.bmp", 1316, 876, true, RGB(0, 0, 255));
			
			
			//의자, 테이블
			_loading->loadImage("chair01_1", "IMAGE/mapTool/obj/chair01_1.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair01_2", "IMAGE/mapTool/obj/chair01_2.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair02_1", "IMAGE/mapTool/obj/chair02_1.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair02_2", "IMAGE/mapTool/obj/chair02_2.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair03_1", "IMAGE/mapTool/obj/chair03_1.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair03_2", "IMAGE/mapTool/obj/chair03_2.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair04_1", "IMAGE/mapTool/obj/chair04_1.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("chair04_2", "IMAGE/mapTool/obj/chair04_2.bmp", 94, 69, true, RGB(255, 0, 0));
			_loading->loadImage("table01_1", "IMAGE/mapTool/obj/table01_1.bmp", 94, 94, true, RGB(255, 0, 0));
			_loading->loadImage("table01_2", "IMAGE/mapTool/obj/table01_2.bmp", 142, 103, true, RGB(255, 0, 0));
			_loading->loadImage("table02_1", "IMAGE/mapTool/obj/table02_1.bmp", 94, 96, true, RGB(255, 0, 0));
			_loading->loadImage("table02_2", "IMAGE/mapTool/obj/table02_2.bmp", 142, 103, true, RGB(255, 0, 0));
			_loading->loadImage("table03_1", "IMAGE/mapTool/obj/table03_1.bmp", 122, 88, true, RGB(255, 0, 0));
			_loading->loadImage("table04_1", "IMAGE/mapTool/obj/table04_1.bmp", 122, 88, true, RGB(255, 0, 0));
			_loading->loadImage("table05_1", "IMAGE/mapTool/obj/table05_1.bmp", 122, 88, true, RGB(255, 0, 0));
			_loading->loadImage("chair05", "IMAGE/mapTool/obj/chair05.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair06", "IMAGE/mapTool/obj/chair06.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair07", "IMAGE/mapTool/obj/chair07.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair08", "IMAGE/mapTool/obj/chair08.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair09", "IMAGE/mapTool/obj/chair09.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair10", "IMAGE/mapTool/obj/chair10.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair11", "IMAGE/mapTool/obj/chair11.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("chair12", "IMAGE/mapTool/obj/chair12.bmp", 47, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter1", "IMAGE/mapTool/obj/counter1.bmp", 100, 84, true, RGB(255, 0, 0));
			_loading->loadImage("counter2", "IMAGE/mapTool/obj/counter2.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter3", "IMAGE/mapTool/obj/counter3.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter4", "IMAGE/mapTool/obj/counter4.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter5", "IMAGE/mapTool/obj/counter5.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter6", "IMAGE/mapTool/obj/counter6.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter7", "IMAGE/mapTool/obj/counter7.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter8", "IMAGE/mapTool/obj/counter8.bmp", 64, 108, true, RGB(255, 0, 0));
			_loading->loadImage("counter9", "IMAGE/mapTool/obj/counter9.bmp", 32, 60, true, RGB(255, 0, 0));
			_loading->loadImage("counter10", "IMAGE/mapTool/obj/counter10.bmp", 32, 60, true, RGB(255, 0, 0));

			//보스맵
			_loading->loadImage("st44a1", "IMAGE/mapTool/obj/st44a1.bmp", 1216, 816, true, RGB(255, 0, 0));

			//미니맵 타일
			_loading->loadImage("mini_sign_2_04", "IMAGE/mapTool/obj/sign_2_04.bmp", 64 / 10, 95 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_sign_2_05", "IMAGE/mapTool/obj/sign_2_05.bmp", 64 / 10, 95 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_sign_2_06", "IMAGE/mapTool/obj/sign_2_06.bmp", 64 / 10, 95 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_tree.64.138", "IMAGE/mapTool/obj/tree.64.138.bmp", 64 / 10, 138 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush1.64.55", "IMAGE/mapTool/obj/bush1.64.55.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush2.64.45", "IMAGE/mapTool/obj/bush2.64.45.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush3.64.48", "IMAGE/mapTool/obj/bush3.64.48.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush4.64.37", "IMAGE/mapTool/obj/bush4.64.37.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush5.64.58", "IMAGE/mapTool/obj/bush5.64.58.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush6.64.48", "IMAGE/mapTool/obj/bush6.64.48.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush7.64.45", "IMAGE/mapTool/obj/bush7.64.45.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_bush8.64.37", "IMAGE/mapTool/obj/bush8.64.37.bmp", 64 / 10, 55 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_barrel.64.58", "IMAGE/mapTool/obj/barrel.64.58.bmp", 64 / 10, 58 / 10, true, RGB(0, 0, 255));
			_loading->loadImage("mini_palmtree1.440.252", "IMAGE/mapTool/obj/palmtree1.440.252.bmp", 440 / 10, 252 / 10, true, RGB(255, 0, 255));
			_loading->loadImage("mini_palmtree2.344.318", "IMAGE/mapTool/obj/palmtree2.344.318.bmp", 344 / 10, 318 / 10, true, RGB(255, 0, 255));
			_loading->loadImage("mini_st50a", "IMAGE/mapTool/obj/st50a.bmp", 384 / 10, 328 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_st50b", "IMAGE/mapTool/obj/st50b.bmp", 384 / 10, 328 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_st50c", "IMAGE/mapTool/obj/st50c.bmp", 384 / 10, 328 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_st02a", "IMAGE/mapTool/obj/st02a.bmp", 1316 / 10, 876 / 10, true, RGB(0, 0, 255));
			
			//의자, 테이블
			_loading->loadImage("mini_chair01_1", "IMAGE/mapTool/obj/chair01_1.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair01_2", "IMAGE/mapTool/obj/chair01_2.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair02_1", "IMAGE/mapTool/obj/chair02_1.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair02_2", "IMAGE/mapTool/obj/chair02_2.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair03_1", "IMAGE/mapTool/obj/chair03_1.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair03_2", "IMAGE/mapTool/obj/chair03_2.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair04_1", "IMAGE/mapTool/obj/chair04_1.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair04_2", "IMAGE/mapTool/obj/chair04_2.bmp", 94 / 10, 69 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table01_1", "IMAGE/mapTool/obj/table01_1.bmp", 94 / 10, 94 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table01_2", "IMAGE/mapTool/obj/table01_2.bmp", 142 / 10, 103 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table02_1", "IMAGE/mapTool/obj/table02_1.bmp", 94 / 10, 96 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table02_2", "IMAGE/mapTool/obj/table02_2.bmp", 142 / 10, 103 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table02_2", "IMAGE/mapTool/obj/table02_2.bmp", 142 / 10, 103 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table03_1", "IMAGE/mapTool/obj/table03_1.bmp", 122 / 10, 88 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table04_1", "IMAGE/mapTool/obj/table04_1.bmp", 122 / 10, 88 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_table05_1", "IMAGE/mapTool/obj/table05_1.bmp", 122 / 10, 88 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair05", "IMAGE/mapTool/obj/chair05.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair06", "IMAGE/mapTool/obj/chair06.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair07", "IMAGE/mapTool/obj/chair07.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair08", "IMAGE/mapTool/obj/chair08.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair09", "IMAGE/mapTool/obj/chair09.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair10", "IMAGE/mapTool/obj/chair10.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair11", "IMAGE/mapTool/obj/chair11.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_chair12", "IMAGE/mapTool/obj/chair12.bmp", 47 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter1", "IMAGE/mapTool/obj/counter1.bmp", 100 / 10, 84 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter2", "IMAGE/mapTool/obj/counter2.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter3", "IMAGE/mapTool/obj/counter3.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter4", "IMAGE/mapTool/obj/counter4.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter5", "IMAGE/mapTool/obj/counter5.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter6", "IMAGE/mapTool/obj/counter6.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter7", "IMAGE/mapTool/obj/counter7.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter8", "IMAGE/mapTool/obj/counter8.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter9", "IMAGE/mapTool/obj/counter9.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));
			_loading->loadImage("mini_counter10", "IMAGE/mapTool/obj/counter10.bmp", 32 / 10, 60 / 10, true, RGB(255, 0, 0));

			//보스맵
			_loading->loadImage("mini_st44a1", "IMAGE/mapTool/obj/st44a1.bmp", 1216 / 10, 816 / 10, true, RGB(255, 0, 0));
		}

		//에너미 타일 로드
		_loading->loadImage("char_15e", "IMAGE/mapTool/enemy/char_15e.bmp", 44, 53, true, RGB(255, 0, 255));
		_loading->loadImage("necro1", "IMAGE/mapTool/enemy/necro1.bmp", 80, 96, true, RGB(255, 0, 255));
		_loading->loadImage("char_33a", "IMAGE/mapTool/enemy/char_33a.bmp", 49, 84, true, RGB(255, 0, 255));
		_loading->loadImage("char_68e", "IMAGE/mapTool/enemy/char_68e.bmp", 66, 92, true, RGB(255, 0, 255));

		//미니맵 타일
		_loading->loadImage("mini_char_15e", "IMAGE/mapTool/enemy/char_15e.bmp", 44 / 10, 53 / 10, true, RGB(255, 0, 255));
		_loading->loadImage("mini_necro1", "IMAGE/mapTool/enemy/necro1.bmp", 80 / 10, 96 / 10, true, RGB(255, 0, 255));
		_loading->loadImage("mini_char_33a", "IMAGE/mapTool/enemy/char_33a.bmp", 49 / 10, 84 / 10, true, RGB(255, 0, 255));
		_loading->loadImage("mini_char_68e", "IMAGE/mapTool/enemy/char_68e.bmp", 66 / 10, 92 / 10, true, RGB(255, 0, 255));
		
		//보스맵 (성화, 성화대)
		_loading->loadImage("frameMound", "IMAGE/mapTool/frameMound.bmp", 480, 321, true, RGB(255, 0, 0));
		_loading->loadFrameImage("fire", "IMAGE/mapTool/fire.640.384.60.bmp", 640 * 3, 384 * 3, 10, 6, true, RGB(255, 0, 255));

		
		//설명(키보드)
		_loading->loadImage("keyInfo.1280.350", "IMAGE/mapTool/keyInfo.1280.350.bmp", 1280 / 2.8, 350 / 2.8);
	}

}

/* < 여기에 각 씬에 출력할 사운드 미리 로드 해두는 곳입니다. >*/
void loadingScene::loadingSound()
{
	SOUNDMANAGER->addSound("떼껄룩", "SOUND/떼껄룩2.mp3", false, false);
	SOUNDMANAGER->addSound("버튼", "SOUND/버튼.mp3", false, false);
	SOUNDMANAGER->addSound("find", "SOUND/find.wav", false, false);
	SOUNDMANAGER->addSound("meter", "SOUND/meter.wav", false, false);
	SOUNDMANAGER->addSound("battle02", "SOUND/battle02.wav", false, false);
	SOUNDMANAGER->addSound("battle08", "SOUND/battle08.wav", false, false);
	SOUNDMANAGER->addSound("water", "SOUND/water.wav", false, false);
	
	//문
	SOUNDMANAGER->addSound("unlock", "SOUND/unlock2.wav", false, false);
	SOUNDMANAGER->addSound("close", "SOUND/close.wav", false, false);
	
	//공격
	SOUNDMANAGER->addSound("attack", "SOUND/attack.wav", false, false);

	SOUNDMANAGER->addSound("church", "SOUND/church.wav", false, false);
	SOUNDMANAGER->addSound("magic02", "SOUND/magic02.wav", false, false);
	SOUNDMANAGER->addSound("spectate", "SOUND/spectate.wav", false, false);
	SOUNDMANAGER->addSound("cat", "SOUND/cat.wav", false, false);
	
	SOUNDMANAGER->addSound("meteo1", "SOUND/meteo1.wav", false, false);
	SOUNDMANAGER->addSound("meteo2", "SOUND/meteo2.wav", false, false);
	SOUNDMANAGER->addSound("meteo3", "SOUND/meteo3.wav", false, false);
	SOUNDMANAGER->addSound("magic04", "SOUND/magic04.wav", false, false);
	SOUNDMANAGER->addSound("magic04_cut", "SOUND/magic04-[AudioTrimmer.com].wav", false, false);
	SOUNDMANAGER->addSound("die", "SOUND/die.wav", false, false);
	SOUNDMANAGER->addSound("summon", "SOUND/summon.wav", false, false);
	SOUNDMANAGER->addSound("geno4", "SOUND/geno4.wav", false, false);


	SOUNDMANAGER->addSound("bgm00", "SOUND/bgm00(타이틀).mid", true, true);
	SOUNDMANAGER->addSound("bgm01", "SOUND/bgm01(마을).mid", true, true);

}
