#include "stdafx.h"
#include "mainGame.h"


//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//�̰����� �ʱ�ȭ�� �Ѵ�

	
	/*���߰�*/
	SCENEMANAGER->addScene("�ε�", new loadingScene);
	SCENEMANAGER->addScene("Ÿ��Ʋ", new titleScene);
	SCENEMANAGER->addScene("����", new townScene);
	SCENEMANAGER->addScene("�Ϲ��ʵ�", new fieldScene);
	SCENEMANAGER->addScene("�����ʵ�", new bossScene);
	
	SCENEMANAGER->addScene("����", new mapToolScene);



	//����� ����
	SCENEMANAGER->loadScene("�ε�");

	//��Ʈ �߰�
	AddFontResource("Maplestory Bold.ttf");



	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
	//�̹��� Ŭ������ ���������� ����� �� ����
	//�����Ҵ� new�� ����ߴٸ� �̰����� SAFE_DELETE() ����Ѵ�
	RemoveFontResource("Maplestory Bold.ttf");
}


//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	//�̰����� ����, Ű����, ���콺��� ������Ʈ�� �Ѵ�
	//�����ϰ� �̰����� �ڵ��Ѵٰ� �����ϸ� �ȴ�
	
	SCENEMANAGER->update();
	SOUNDMANAGER->update();
	

	

	if (KEYMANAGER->isOnceKeyDown(VK_HOME)) SCENEMANAGER->loadScene("Ÿ��Ʋ");
	if (KEYMANAGER->isOnceKeyDown(VK_END)) SCENEMANAGER->loadScene("�����ʵ�");

}

//=============================================================
//	## ���� ## render(HDC hdc)
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ�� (�̰͵� ������ �׳� �α�)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	SCENEMANAGER->render();

	//Ÿ�ӸŴ��� ����
	TIMEMANAGER->render(getMemDC());


	//textOutMouse(getMemDC());

//=============================================================
	//������� ������ HDC�� �׸��� (�̰͵� ������ �׳� �α�)
	this->getBackBuffer()->render(getHDC());
}
