#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	SOUNDMANAGER->stop("bgm01");
	//�� Ÿ�� ���
	img_bg = IMAGEMANAGER->findImage("st02a");

	//�þ� �ʱ�ȭ
	GDIPLUS->addImage("viewSight", "PNGIMAGE/viewSight.png");

	init_pos = { 17, 1 };	//ĳ���� �ʱ� Ÿ�� �ε���
	tileSize = MAXTILESIZE_TOWN;	//���� �� Ÿ�� ������

	//Ÿ�� ��Ʈ ���� ��ǥ
	pt_tileMark = { (WINSIZEX / 2) + (((init_pos.x - init_pos.y) / 2) * TILEWIDTH), WINSIZEY / 2 - init_pos.y * TILEHEIGHT - (((init_pos.x - init_pos.y) / 2) * TILEHEIGHT) };

	//Ŭ���� ��Ʈ �ʱ�ȭ
	rc_clipping = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 540, 400);

	//�� Ÿ�� �ʱ�ȭ
	for (int i = 0; i < MAXTILESIZE_TOWN; i++)
	{
		for (int j = 0; j < MAXTILESIZE_TOWN; j++)
		{
			tile[i][j].x = pt_tileMark.x - (i * 64 / 2) + (j * 64 / 2);
			tile[i][j].y = pt_tileMark.y + (i * 32 / 2) + (j * 32 / 2);
			tile[i][j].rc = RectMakeCenter(tile[i][j].x, tile[i][j].y, 64, 32);
			tile[i][j].rc_mini = RectMake(tile[i][j].x_mini, tile[i][j].y_mini, 64 / 10, 32 / 10);
			sprintf(tile_strKey[i][j].str_img, "%s", "st51a_22");
			tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
			tile_strKey[i][j].isImg = true;

			tile_strKey[i][j].state_obj = NONE;
			tile_strKey[i][j].floor = 0;
			sprintf(tile_strKey[i][j].str_img_floor[0], "%s", "st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor[1], "%s", "st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor[2], "%s", "st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor[3], "%s", "st51a_37");
			tile_img[i][j].img_floor[0] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[0]);
			tile_img[i][j].img_floor[1] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[1]);
			tile_img[i][j].img_floor[2] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[2]);
			tile_img[i][j].img_floor[3] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[3]);

			sprintf(tile_strKey[i][j].str_img_obj, "%s", "���̹���");
			tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //�ʱ� Ÿ��(�� Ÿ��)
			sprintf(tile_strKey[i][j].str_img_enemy, "%s", "���̹���");
			tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //�ʱ� Ÿ��(�� Ÿ��)

			tile_strKey[i][j].isImg_obj = false;
			tile_strKey[i][j].isImg_enemy = false;

			tile[i][j].z_order = false;
			tile[i][j].isFloodFill = false;
		}
	}

	count = 0;	//������ ī��Ʈ


	//npc ���� �ʱ�ȭ
	{
		npc[0].x = 2;
		npc[0].y = 12;
		npc[0].img = IMAGEMANAGER->findImage("npcī���;�����");

		npc[1].x = 2;
		npc[1].y = 6;
		npc[1].img = IMAGEMANAGER->findImage("npcī���;ư���");

		npc[2].x = 10;
		npc[2].y = 1;
		npc[2].img = IMAGEMANAGER->findImage("npc���̺�");

		npc[3].x = 12;
		npc[3].y = 2;
		npc[3].img = IMAGEMANAGER->findImage("char_15e");

		for (int i = 0; i < NPCSIZE; i++) npc[i].isTalk = false;


		//����
		sign.x = 20;
		sign.y = 1;
		sign.isTalk = false;
	}


	//ĳ���� ���� �ʱ�ȭ
	index.x = init_pos.x;
	index.y = init_pos.y;
	x = tile[index.x][index.y].x;
	y = tile[index.x][index.y].y;
	rc = RectMake(x - IMAGEMANAGER->findImage("run1")->getWidth() / 8, y - IMAGEMANAGER->findImage("run1")->getHeight(), 330 / 4, 67);
	state = IDLE;
	direc_LR = LEFT;
	direc_DU = DOWN;
	runSpeed = 1;
	hpMax = 500;
	hp = hpMax;
	//ü�¹� Ŭ���� �ʱ�ȭ
	hpBar = new progressBar;
	hpBar->init("IMAGE/ui/hpBar.322.8", "IMAGE/shadow", 64, 436, 322, 8);
	hpBar->setGauge(hp, hpMax);

	//A* ���� �ʱ�ȭ
	{
		//��ü��� �ʱ�ȭ
		for (int i = 0; i < MAXTILESIZE_TOWN; i++)
		{
			for (int j = 0; j < MAXTILESIZE_TOWN; j++)
			{
				//���ο� ���� ��Ʈ��ġ ����
				_totalNode[i][j] = new node(i, j);
				//_totalNode[i][j]->rc = tile[i][j].rc;
			}
		}
		isNodePick = false;	//A* ��� ���� ����
		nodeNum = 0;		//A* ��� ��

		//ùŬ���� ¦���� �ǰ� �ϱ����ؼ� -1�� �ʱ�ȭ
		_count = -1;
		//��ã�ҳ�?
		_isFind = false;
	}

	//ķ ���� ���콺
	_ptMouseC = { _ptMouse.x + x, _ptMouse.y + y };

	//�� ���� �ε�
	{
		HANDLE file;
		DWORD read;

		file = CreateFile("map_town.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE_TOWN * MAXTILESIZE_TOWN, &read, NULL);

		//�� Ÿ�� �ε�
		for (int i = 0; i < MAXTILESIZE_TOWN; i++)
		{
			for (int j = 0; j < MAXTILESIZE_TOWN; j++)
			{
				tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
				tile_img[i][j].img_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_mini);
				tile_img[i][j].img_floor[0] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[0]);
				tile_img[i][j].img_floor[1] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[1]);
				tile_img[i][j].img_floor[2] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[2]);
				tile_img[i][j].img_floor[3] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[3]);
				tile_img[i][j].img_floor_mini[0] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[0]);
				tile_img[i][j].img_floor_mini[1] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[1]);
				tile_img[i][j].img_floor_mini[2] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[2]);
				tile_img[i][j].img_floor_mini[3] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[3]);
				tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //�ʱ� Ÿ��(�� Ÿ��)
				tile_img[i][j].img_obj_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj_mini); //�ʱ� Ÿ��(�� Ÿ��)
				tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //�ʱ� Ÿ��(�� Ÿ��)
				tile_img[i][j].img_enemy_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy_mini); //�ʱ� Ÿ��(�� Ÿ��)

				//A* �� ó��
				if (tile_strKey[i][j].state_obj == BLOCK)
				{
					_totalNode[i][j]->nodeState = NODE_WALL;
				}
			}
		}
		CloseHandle(file);
	}

	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void townScene::release()
{
}

void townScene::update()
{
	count++;

	//�÷��̾� ü�¹� ������Ʈ
	hpBar->update();
	hpBar->setGauge(hp, hpMax);
	//ü�� �ٿ�
	if (KEYMANAGER->isStayKeyDown(VK_DOWN)) hp -= 10;

	//for�� üũ ����
	{
		//ȭ�� ���� ���� - ���� 10, ���� 14
		x_for_start = index.x - 11;
		if (x_for_start < 0) x_for_start = 0;
		x_for_end = index.x + 11;
		if (x_for_end > tileSize) x_for_end = tileSize;
		y_for_start = index.y - 11;
		if (y_for_start < 0) y_for_start = 0;
		y_for_end = index.y + 11;
		if (y_for_end > tileSize) y_for_end = tileSize;

		//z-order üũ ���� - ���� 4, ���� 4
		x_for_start_z = index.x - 5;
		if (x_for_start_z < 0) x_for_start_z = 0;
		x_for_end_z = index.x + 5;
		if (x_for_end_z > tileSize) x_for_end_z = tileSize;
		y_for_start_z = index.y - 5;
		if (y_for_start_z < 0) y_for_start_z = 0;
		y_for_end_z = index.y + 5;
		if (y_for_end_z > tileSize) y_for_end_z = tileSize;
	}


	//A*��� ó�� 
	{
		//��Ŭ���ϸ� ��� ã��
		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON) && state == IDLE)
		{
			//���â �ݱ�
			for (int i = 0; i < NPCSIZE; i++) npc[i].isTalk = false;
			sign.isTalk = false; //����

			//��ȭ ���� npc ��ȭâ on/off ó��
			{
				//npc
				for (int i = 0; i < NPCSIZE; i++)
				{
					if ((-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						!npc[i].isTalk) npc[i].isTalk = true;
					else if ((-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						npc[i].isTalk)
					{
						npc[i].isTalk = false;
						if (SOUNDMANAGER->isPlaySound("spectate")) SOUNDMANAGER->stop("spectate");
						if (SOUNDMANAGER->isPlaySound("magic02")) SOUNDMANAGER->stop("magic02");
						if (SOUNDMANAGER->isPlaySound("church")) SOUNDMANAGER->stop("church");
					}
				}
				//����
				if ((-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					!sign.isTalk)
				{
					if (!SOUNDMANAGER->isPlaySound("battle08")) SOUNDMANAGER->play("battle08");
					sign.isTalk = true;
				}
				else if ((-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					sign.isTalk) sign.isTalk = false;
			}



			bool tttest = false;
			_isFind = false;
			//��� Ŭ����
			_openList.clear();
			_closeList.clear();
			_finalList.clear();
			nodeNum = 0;


			//��� ã��
			for (int i = x_for_start; i < x_for_end; i++)
			{
				for (int j = y_for_start; j < y_for_end; j++)
				{
					if ((-0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						(0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(-0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 && 
						_totalNode[i][j]->nodeState != NODE_WALL)//���� ��� ����
					{
						tttest = true;
						_totalNode[index.x][index.y]->nodeState = NODE_START;
						_startNode = _totalNode[index.x][index.y];
						_totalNode[i][j]->nodeState = NODE_END;
						_endNode = _totalNode[i][j];
						temp_endNode = _endNode;
						state = RUN;
					}
				}
			}

			if (tttest) this->pathFinding();
			if (_finalList.size() > 0) _finalList.push_back(temp_endNode);
		}

		//��� ���� �̵��ϱ�
		if (_finalList.size() > 0)
		{
			if (_finalList.size() > nodeNum && _finalList.size() != nodeNum + 1)
			{
				//���� ����
				if (_finalList[nodeNum]->index.x + 1 == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y == _finalList[nodeNum + 1]->index.y)//����
				{
					direc_LR = LEFT;
					direc_DU = DOWN;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y + 1 == _finalList[nodeNum + 1]->index.y)//����
				{
					direc_LR = RIGHT;
					direc_DU = DOWN;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y - 1 == _finalList[nodeNum + 1]->index.y)//�»�
				{
					direc_LR = LEFT;
					direc_DU = UP;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x - 1 == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y == _finalList[nodeNum + 1]->index.y)//���
				{
					direc_LR = RIGHT;
					direc_DU = UP;
					state = RUN;
				}

			}
			else if (_finalList.size() == nodeNum + 1)
			{
				state = IDLE;
				nodeNum = 0;
				_isFind = false;
				//��� Ŭ����
				_openList.clear();
				_closeList.clear();
				_finalList.clear();
			}


			if (state == RUN)
			{
				//�̵� ó��
				switch (direc_DU)
				{
				case DOWN:
					y += runSpeed;
					switch (direc_LR)
					{
					case LEFT:
						x -= runSpeed * 2;
						break;
					case RIGHT:
						x += runSpeed * 2;
						break;
					}
					break;
				case UP:
					y -= runSpeed;
					switch (direc_LR)
					{
					case LEFT:
						x -= runSpeed * 2;
						break;
					case RIGHT:
						x += runSpeed * 2;
						break;
					}
					break;
				}

				//�ε��� ����
				for (int i = x_for_start; i < x_for_end; i++)
				{
					for (int j = y_for_start; j < y_for_end; j++)
					{
						if (x == tile[i][j].x && y == tile[i][j].y)
						{
							//state = IDLE;
							nodeNum++;
							index.x = i;
							index.y = j;
						}
					}
				}
			}
		}
	}

	//ĳ���� �ε��� �ֺ� z-order üŷ
	for (int i = x_for_start_z; i < x_for_end_z; i++)
	{
		for (int j = y_for_start_z; j < y_for_end_z; j++)
		{
			if (y < tile[i][j].y) tile[i][j].z_order = true;
			else tile[i][j].z_order = false;
		}
	}

	this->userControl();


	//�Ϲ� �ʵ�� �̵�
	//if (index.x == 17 && index.y == 0) SCENEMANAGER->loadScene("�Ϲ��ʵ�");
	if (index.x == 17 && index.y == 0)
	{
		_isFind = false;
		//��� Ŭ����
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
		nodeNum = 0;
		SOUNDMANAGER->play("close", 0.2f);
		SCENEMANAGER->loadScene("�Ϲ��ʵ�");
	}

	//ĳ������ RC
	rc = RectMake(x - IMAGEMANAGER->findImage("run1")->getWidth() / 8, y - IMAGEMANAGER->findImage("run1")->getHeight(), 330 / 4, 67);
	//rc_clipping = RectMakeCenter(x, y, 540, 400);
	rc_clipping = RectMakeCenter(x, y, WINSIZEX, WINSIZEY);
	//ķ ���� ���콺 ��ǥ ������Ʈ
	_ptMouseC = { _ptMouse.x - (tile[init_pos.x][init_pos.y].x - x), _ptMouse.y - (tile[init_pos.x][init_pos.y].y - y) };
}

void townScene::render()
{
	//ī�޶� ���� �� ����
	{
		//Ÿ�� ����
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
				{
					//Ÿ�� ����
					if (tile_strKey[i][j].isImg)
						tile_img[i][j].img->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x), tile[i][j].rc.top + (tile[init_pos.x][init_pos.y].y - y)); //Ÿ�� �̹��� ����
				}
			}
		}

		//��� ����
		img_bg->render(getMemDC(), tile[20][0].rc.left + 16 + (tile[init_pos.x][init_pos.y].x - x), tile[0][0].rc.top - 204 + (tile[init_pos.x][init_pos.y].y - y));

		//���ʹ� + ������Ʈ ����
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				//���ʹ� ����
				if (tile_strKey[i][j].isImg_enemy)
					tile_img[i][j].img_enemy->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_enemy->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_enemy->getHeight());

				//������Ʈ ���� + ���� ó��
				if (!tile[i][j].z_order)
				{
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_obj->getHeight());
				}
			}
		}
	}

	//ĳ���� ����
	if (state == IDLE)
	{
		if (count % 16 == 0)
		{
			if (IMAGEMANAGER->findImage("idle")->getFrameX() == 2)
			{
				IMAGEMANAGER->findImage("idle")->setFrameX(-1);
			}
			IMAGEMANAGER->findImage("idle")->setFrameX(IMAGEMANAGER->findImage("idle")->getFrameX() + 1);
		}
		//imageNum("����2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("idle")->getFrameX(), 20);
		IMAGEMANAGER->findImage("idle")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
	}
	if (state == RUN)
	{
		switch (direc_DU)
		{
		case DOWN:
			switch (direc_LR)
			{
			case LEFT:
				if (count % 8 == 0)
				{
					if (IMAGEMANAGER->findImage("run1")->getFrameX() >= 3)
					{
						IMAGEMANAGER->findImage("run1")->setFrameX(-1);
					}
					IMAGEMANAGER->findImage("run1")->setFrameX(IMAGEMANAGER->findImage("run1")->getFrameX() + 1);
				}
				//imageNum("����2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run1")->getFrameX(), 20);
				IMAGEMANAGER->findImage("run1")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
				break;
			case RIGHT:
				if (count % 8 == 0)
				{
					if (IMAGEMANAGER->findImage("run2")->getFrameX() >= 3)
					{
						IMAGEMANAGER->findImage("run2")->setFrameX(-1);
					}
					IMAGEMANAGER->findImage("run2")->setFrameX(IMAGEMANAGER->findImage("run2")->getFrameX() + 1);
				}
				//imageNum("����2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run2")->getFrameX(), 20);
				IMAGEMANAGER->findImage("run2")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
				break;
			}
			break;
		case UP:
			switch (direc_LR)
			{
			case LEFT:
				if (count % 8 == 0)
				{
					if (IMAGEMANAGER->findImage("run3")->getFrameX() >= 3)
					{
						IMAGEMANAGER->findImage("run3")->setFrameX(-1);
					}
					IMAGEMANAGER->findImage("run3")->setFrameX(IMAGEMANAGER->findImage("run3")->getFrameX() + 1);
				}
				//imageNum("����2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run3")->getFrameX(), 20);
				IMAGEMANAGER->findImage("run3")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
				break;
			case RIGHT:
				if (count % 8 == 0)
				{
					if (IMAGEMANAGER->findImage("run4")->getFrameX() >= 3)
					{
						IMAGEMANAGER->findImage("run4")->setFrameX(-1);
					}
					IMAGEMANAGER->findImage("run4")->setFrameX(IMAGEMANAGER->findImage("run4")->getFrameX() + 1);
				}
				//imageNum("����2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run4")->getFrameX(), 20);
				IMAGEMANAGER->findImage("run4")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
				break;
			}
			break;
		}
	}


	//npc����
	{
		if (count % 15 == 0) //ī���;�����
		{
			if (npc[0].img->getFrameX() == 3)
			{
				npc[0].img->setFrameX(-1);
			}
			npc[0].img->setFrameX(npc[0].img->getFrameX() + 1);
		}
		npc[0].img->frameRender(getMemDC(),
			tile[npc[0].x][npc[0].y].rc.left + 8 + (tile[init_pos.x][init_pos.y].x - x),
			tile[npc[0].x][npc[0].y].rc.bottom - 14 - npc[0].img->getHeight() + (tile[init_pos.x][init_pos.y].y - y));

		if (count % 15 == 0) //ī���;ư���
		{
			if (npc[1].img->getFrameX() == 3)
			{
				npc[1].img->setFrameX(-1);
			}
			npc[1].img->setFrameX(npc[1].img->getFrameX() + 1);
		}
		npc[1].img->frameRender(getMemDC(),
			tile[npc[1].x][npc[1].y].rc.left + 12 + (tile[init_pos.x][init_pos.y].x - x),
			tile[npc[1].x][npc[1].y].rc.bottom - 16 - npc[1].img->getHeight() + (tile[init_pos.x][init_pos.y].y - y));

		if (count % 15 == 0) //���̺�
		{
			if (npc[2].img->getFrameX() == 2)
			{
				npc[2].img->setFrameX(-1);
			}
			npc[2].img->setFrameX(npc[2].img->getFrameX() + 1);
		}
		npc[2].img->frameRender(getMemDC(),
			tile[npc[2].x][npc[2].y].rc.left + 12 + (tile[init_pos.x][init_pos.y].x - x),
			tile[npc[2].x][npc[2].y].rc.bottom - 16 - npc[2].img->getHeight() + (tile[init_pos.x][init_pos.y].y - y));

		//��Ż
		if (count % 4 == 0)
		{
			if (IMAGEMANAGER->findImage("��Ż")->getFrameX() == 5)
			{
				IMAGEMANAGER->findImage("��Ż")->setFrameX(-1);
			}
			IMAGEMANAGER->findImage("��Ż")->setFrameX(IMAGEMANAGER->findImage("��Ż")->getFrameX() + 1);
		}
		IMAGEMANAGER->findImage("��Ż")->frameRender(getMemDC(),
			tile[17][0].rc.left + (tile[init_pos.x][init_pos.y].x - x),
			tile[17][0].rc.bottom - 180 + (tile[init_pos.x][init_pos.y].y - y));

		//��ǳ�� ����
		{
			for (int i = 0; i < NPCSIZE; i++)
			{
				if ((-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.left) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(-0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(0.5) * (_ptMouseC.x - tile[npc[i].x][npc[i].y].rc.right) + tile[npc[i].x][npc[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0)
				{
					if ((count / 20) % 2 == 1) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(0);
					else if ((count / 20) % 2 == 0) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(1);
					IMAGEMANAGER->findImage("talkBalloon")->frameRender(getMemDC(), _ptMouse.x - IMAGEMANAGER->findImage("talkBalloon")->getFrameWidth() / 2, _ptMouse.y - npc[i].img->getFrameHeight());
				}
			}
			if ((-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
				(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.left) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(-0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(0.5) * (_ptMouseC.x - tile[sign.x][sign.y].rc.right) + tile[sign.x][sign.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0)
			{
				if ((count / 20) % 2 == 1) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(0);
				else if ((count / 20) % 2 == 0) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(1);
				IMAGEMANAGER->findImage("talkBalloon")->frameRender(getMemDC(), _ptMouse.x, _ptMouse.y - 80);
			}
		}
	}



	//(Z����)Ÿ�� ���� + Ŭ���� ó�� + ������Ʈ ����
	for (int i = x_for_start; i < x_for_end; i++)
	{
		for (int j = y_for_start; j < y_for_end; j++)
		{
			//������Ʈ ���� + ���� ó��
			if (tile[i][j].z_order)
			{
				if (tile_strKey[i][j].isImg_obj)
					tile_img[i][j].img_obj->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_obj->getHeight());
			}
		}
	}

	// < ġƮ > ��� Ű1
	{
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
				{
					if (KEYMANAGER->isToggleKey(VK_F5)) drawTileIndex(tile[i][j], i, j, tile[init_pos.x][init_pos.y]);//Ÿ�� �ε��� ����
					//if (KEYMANAGER->isToggleKey(VK_F6)) FrameRect(getMemDC(), tile[i][j].rc_picking, RGB(255, 0, 255));//��ŷ ���� ����
					if (KEYMANAGER->isToggleKey(VK_F7))
					{
						drawDiamond(tile[i][j], PS_DOT, RGB(255, 255, 0));//���̼� Ÿ�� ����
						//Ŭ���� ���� ���<- ����� ī�޶��?
						FrameRect(getMemDC(), rc_clipping, RGB(255, 0, 255));
						//����� �����ֱ�
						if (_totalNode[i][j]->nodeState == NODE_WALL)
						{
							setNodeColor(tile[i][j], RGB(200, 150, 100));
						}
					}
					if (KEYMANAGER->isToggleKey(VK_F8)) //�̵� ���� ���� üũ ����
					{
						if (tile_strKey[i][j].state_obj == BLOCK)
						{
							IMAGEMANAGER->findImage("tile_block")->alphaRender(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x), tile[i][j].rc.top + (tile[init_pos.x][init_pos.y].y - y), 80);
						}
					}
				}
			}
		}
		//���Ű ���� ���
		if (KEYMANAGER->isOnceKeyUp(VK_F5) || KEYMANAGER->isOnceKeyUp(VK_F6) || KEYMANAGER->isOnceKeyUp(VK_F7) || KEYMANAGER->isOnceKeyUp(VK_F8) || KEYMANAGER->isOnceKeyUp(VK_F9))	SOUNDMANAGER->play("��ư");
	}

	//��ŷ ���콺 ������
	for (int i = x_for_start; i < x_for_end; i++)
	{
		for (int j = y_for_start; j < y_for_end; j++)
		{
			if (KEYMANAGER->isToggleKey(VK_F7))
			{
				if ((-0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
					(0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(-0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
					(0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0)
					drawDiamond(tile[i][j], PS_SOLID, RGB(255, 0, 255));
			}
			if ((-0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
				(0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(-0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
				tile_strKey[i][j].state_obj != BLOCK)
				drawDiamond(tile[i][j], PS_SOLID, RGB(0, 255, 0), tile[init_pos.x][init_pos.y]);
			else if ((-0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
				(0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(-0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
				(0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
				tile_strKey[i][j].state_obj == BLOCK)
				drawDiamond(tile[i][j], PS_SOLID, RGB(255, 0, 0), tile[init_pos.x][init_pos.y]);
		}
	}

	//A* ���
	{
		//��ã������ �����ֱ�
		if (_isFind)
		{
			for (int i = 0; i < _finalList.size(); i++)
			{
				if (KEYMANAGER->isToggleKey(VK_F7)) this->drawDiamond(tile[_finalList[i]->index.x][_finalList[i]->index.y], PS_SOLID, RGB(255, 0, 255));
				this->drawDiamond(tile[_finalList[i]->index.x][_finalList[i]->index.y], PS_SOLID, RGB(50, 255, 50), tile[init_pos.x][init_pos.y]);
			}
		}
	}

	//�þ� ó��
	if (KEYMANAGER->isToggleKey(VK_F10)) GDIPLUS->gdiRender("viewSight", getMemDC());

	//�÷��̾� hud ���
	{
		IMAGEMANAGER->findImage("uiBar2")->render(getMemDC(), 0, 360);
		hpBar->render();

		IMAGEMANAGER->findImage("mpBar")->render(getMemDC(), 64, 448);
		IMAGEMANAGER->findImage("uiBar")->render(getMemDC(), 0, 360);
		if (state == IDLE) IMAGEMANAGER->findImage("face07a")->render(getMemDC(), -24, 396);
		if (state == RUN) IMAGEMANAGER->findImage("face07c")->render(getMemDC(), -24, 396);
		//IMAGEMANAGER->findImage("face07d")->render(getMemDC(), -24, 396);
		//IMAGEMANAGER->findImage("face07j")->render(getMemDC(), -24, 396);

		IMAGEMANAGER->findImage("skill_frame")->render(getMemDC(), 524, 420);
		IMAGEMANAGER->findImage("skill_frame")->render(getMemDC(), 580, 420);
		IMAGEMANAGER->findImage("skill_icon1")->render(getMemDC(), 528, 424);
		IMAGEMANAGER->findImage("skill_icon2")->render(getMemDC(), 584, 424);

		textOutFont(getMemDC(), 559, 421, "Q", 20, RGB(220, 220, 32), "�ü�");
		textOutFont(getMemDC(), 613, 421, "W", 20, RGB(220, 220, 32), "�ü�");

		textOutFont(getMemDC(), 190, 434, "(", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(), 200, 434, hp, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 230, 434, "/", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(), 240, 434, hpMax, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 270, 434, ")", 16, RGB(200, 200, 0));
	}


	// < ġƮ > ��� Ű2
	if (KEYMANAGER->isToggleKey(VK_F7))
	{
		FrameRect(getMemDC(), rc, RGB(255, 255, 255));
		char str_test2[128];
		sprintf(str_test2, "%d, %d / [%d, %d] / state: %d", x, y, index.x, index.y, state);
		TextOut(getMemDC(), x, y, str_test2, strlen(str_test2));
		IMAGEMANAGER->findImage("���콺�׽�Ʈ")->render(getMemDC(), _ptMouseC.x, _ptMouseC.y);
	}



	//npc ���â ����
	{
		if (npc[0].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ ������ ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "�� ���̳׿�~(����)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("spectate")) SOUNDMANAGER->play("spectate", 0.2f);
		}
		if (npc[1].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ ���� �ٸ� ī���� ���� ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "�ȳ��ϼ���~(����)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("spectate")) SOUNDMANAGER->play("spectate", 0.2f);
		}
		if (npc[2].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ ���� õ�� ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "�������� ã�ƺ���~", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30 + 24, "(ü���� ȸ���մϴ�.)", 20, RGB(255, 155, 155));
			if (hp < hpMax) hp++; 
			if (!SOUNDMANAGER->isPlaySound("magic02")) SOUNDMANAGER->play("magic02", 0.4f);
			if (!SOUNDMANAGER->isPlaySound("church")) SOUNDMANAGER->play("church", 0.4f);
		}
		if (npc[3].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "��~!", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (�մ�� ��������ٳ�)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("cat")) SOUNDMANAGER->play("cat", 0.2f);
		}
		if (sign.isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "- �ϼ� : ���谡�� �ʵ� -", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (���� : �̺� ���� ������ !!)", 20, RGB(255, 255, 255));
			
		}
	}


	//textOutDbug(getMemDC(), rc.right, rc.top, x);
	//textOutDbug(getMemDC(), rc.right, rc.top + 20, y);


	SetBkMode(getMemDC(), 2);

	//���� ��
	//TextOut(getMemDC(), WINSIZEX / 2, 0, "Ÿ��", strlen("Ÿ��"));
	fade(getMemDC(), _startTime, true);
}


//������ Ÿ�� ���� �Լ� PS_DOT
void townScene::drawDiamond(tagTile tile, int iStyle, COLORREF color)
{

	HPEN Pen, oldPen;
	Pen = CreatePen(iStyle, 1, color);
	oldPen = (HPEN)SelectObject(getMemDC(), Pen);

	//������ �׸���
	MoveToEx(getMemDC(), tile.x - (TILEWIDTH / 2), tile.y, NULL);
	LineTo(getMemDC(), tile.x, tile.y - (TILEHEIGHT / 2));
	LineTo(getMemDC(), tile.x + (TILEWIDTH / 2), tile.y);
	LineTo(getMemDC(), tile.x, tile.y + (TILEHEIGHT / 2));
	LineTo(getMemDC(), tile.x - (TILEWIDTH / 2), tile.y);

	SelectObject(getMemDC(), oldPen);
	DeleteObject(Pen);
}
void townScene::drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile)
{

	HPEN Pen, oldPen;
	Pen = CreatePen(iStyle, 1, color);
	oldPen = (HPEN)SelectObject(getMemDC(), Pen);

	//������ �׸���
	MoveToEx(getMemDC(), tile.x + (firstTile.x - x) - (TILEWIDTH / 2), tile.y + (firstTile.y - y), NULL);
	LineTo(getMemDC(), tile.x + (firstTile.x - x), tile.y - (TILEHEIGHT / 2) + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x) + (TILEWIDTH / 2), tile.y + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x), tile.y + (TILEHEIGHT / 2) + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x) - (TILEWIDTH / 2), tile.y + (firstTile.y - y));

	SelectObject(getMemDC(), oldPen);
	DeleteObject(Pen);
}


//�ش� Ÿ�� ��ǥ(�ε���) ǥ�� �Լ�
void townScene::drawTileIndex(tagTile tile, int i, int j)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12, tile.y - 8, str, strlen(str));
}
void townScene::drawTileIndex(tagTile tile, int i, int j, tagTile firstTile)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12 + (firstTile.x - x), tile.y - 8 + (firstTile.y - y), str, strlen(str));
}


//���� ��Ʈ�� �Լ� : ��ũ������(+,-), �ʽ����̵�(wasd), Ÿ�� ����(����Ű)
void townScene::userControl()
{
	//Ÿ�� ũ�� ����
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_PLUS) && tileSize < MAXTILESIZE_TOWN)
	{
		tileSize++;
		SOUNDMANAGER->play("��ư");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_MINUS) && tileSize > 1)
	{
		tileSize--;
		SOUNDMANAGER->play("��ư");
	}


}




//��� ��ä���(���������� �����Լ�)
void townScene::setNodeColor(tagTile tile, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	RECT fill = RectMakeCenter(tile.x, tile.y, TILEWIDTH / 2, TILEHEIGHT / 2);
	FillRect(getMemDC(), &fill, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), fill, RGB(0, 0, 0));
}

//��ã�� �Լ�
void townScene::pathFinding()
{
	//���۳�� �� �����尡 ���� ��� ��ã�� ����
	if (!_startNode || !_endNode) return;

	//��ã�⸦ �غ���!!
	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��Ƶΰ� 
	//F�� H�� �������� ���� ã�Ƽ� �׳��� ������� �����ϰ�
	//���¸���Ʈ���� ������� �����ϰ� 
	//������� Ŭ�����Ʈ�� ��Ƶд�

	//1. ���۳�尡 �־�� ����� �����ϴ� 
	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
	_openList.push_back(_startNode);
	_finalList.push_back(_startNode);

	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ� 
	//�����忡 �����Ҷ����� ���ѷ���
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		//���¸���Ʈ �� ���� F�� �۰ų� F�� ���ٸ� 
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ�����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵Ǵ� 1���� ��������
		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//Ŭ�����Ʈ�� �־�д�
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//�����尡 ���������� ����? (��ã�ҳ�?)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
			while (endNode != _startNode)
			{
				tempNode.push_back(endNode);
				endNode = endNode->parentNode;
			}

			//STL����Ʈ�� �̿��ϸ� reverse()����ϰų�, push_front()�� ����� �� ������,
			//�츰 ���͸� �̿��ϴ� ���ٷ� Ǫ�ù����ش�
			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				_finalList.push_back(tempNode[i]);
			}

			_isFind = true;
			//�����ϰ� �������´�
			return;
		}

		//�����¿� (������ �󱡾��� - ��¥�� �ָ� 4���� ��带 ��� ���¸���Ʈ�� �־ �˻��� ������)
		this->addOpenList(_curNode->index.x, _curNode->index.y - 1); //��
		this->addOpenList(_curNode->index.x, _curNode->index.y + 1); //��
		this->addOpenList(_curNode->index.x - 1, _curNode->index.y); //��
		this->addOpenList(_curNode->index.x + 1, _curNode->index.y); //��

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵȴ� ���

	}


}

//���¸���Ʈ�� �߰� �Լ�
void townScene::addOpenList(int idx, int idy)
{
	//����ó��
	//���¸���Ʈ�� ���� ��尡 ��ü��� �ε��� ������ ������ �߰����� ���ϰ� ó���Ѵ�(�ε��� ������ 0 ~ 4����)
	if (idx < 0 || idx >= MAXTILESIZE_TOWN || idy < 0 || idy >= MAXTILESIZE_TOWN) return;
	//��ֹ��� ���¸���Ʈ�� ���� �� ����
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;
	//Ŭ�����Ʈ�� �ִٸ� �̹� �����±��̴� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��ϱ�
	node* neighborNode = _totalNode[idx][idy];
	int moveCost = _curNode->G + ((_curNode->index.x - idx == 0 || _curNode->index.y - idy == 0) ? 10 : 14);

	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//���������� ���¸���Ʈ���� ���°�� 
	//G, H, ParentNode ���� �� ���¸���Ʈ�� �߰�
	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->index.x - _endNode->index.x) + abs(neighborNode->index.y - _endNode->index.y)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = _curNode;
	_openList.push_back(neighborNode);
}

//���¸���Ʈ ����
void townScene::delOpenList(int index)
{
	_openList.erase(_openList.begin() + index);
}


