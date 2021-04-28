#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ϸ� ���ΰ��ӿ� �߰��ϱ�*/
#include "loadingScene.h"
#include "titleScene.h"
#include "townScene.h"
#include "fieldScene.h"
#include "bossScene.h"
#include "mapToolScene.h"
#include "testScene.h"


class mainGame : public gameNode
{
	
public:
	HRESULT init();
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

