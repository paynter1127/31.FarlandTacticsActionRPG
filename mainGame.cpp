#include "stdafx.h"
#include "mainGame.h"


//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//이곳에서 초기화를 한다

	
	/*씬추가*/
	SCENEMANAGER->addScene("로딩", new loadingScene);
	SCENEMANAGER->addScene("타이틀", new titleScene);
	SCENEMANAGER->addScene("마을", new townScene);
	SCENEMANAGER->addScene("일반필드", new fieldScene);
	SCENEMANAGER->addScene("보스필드", new bossScene);
	
	SCENEMANAGER->addScene("맵툴", new mapToolScene);



	//현재씬 설정
	SCENEMANAGER->loadScene("로딩");

	//폰트 추가
	AddFontResource("Maplestory Bold.ttf");



	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
	//이미지 클래스를 나갈때까진 사용할 일 없다
	//동적할당 new를 사용했다면 이곳에서 SAFE_DELETE() 사용한다
	RemoveFontResource("Maplestory Bold.ttf");
}


//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	//이곳에서 계산식, 키보드, 마우스등등 업데이트를 한다
	//간단하게 이곳에서 코딩한다고 생각하면 된다
	
	SCENEMANAGER->update();
	SOUNDMANAGER->update();
	

	

	if (KEYMANAGER->isOnceKeyDown(VK_HOME)) SCENEMANAGER->loadScene("타이틀");
	if (KEYMANAGER->isOnceKeyDown(VK_END)) SCENEMANAGER->loadScene("보스필드");

}

//=============================================================
//	## 렌더 ## render(HDC hdc)
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵 (이것도 렌더에 그냥 두기)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	SCENEMANAGER->render();

	//타임매니져 렌더
	TIMEMANAGER->render(getMemDC());


	//textOutMouse(getMemDC());

//=============================================================
	//백버퍼의 내용을 HDC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}
