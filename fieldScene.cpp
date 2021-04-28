#include "stdafx.h"
#include "fieldScene.h"

HRESULT fieldScene::init()
{
	SOUNDMANAGER->stop("bgm01");
	//�þ� �ʱ�ȭ
	GDIPLUS->addImage("viewSight", "PNGIMAGE/viewSight.png");
	sightOn = true;

	//������ ����(+ BT AI Ŭ���� ����)
	_data = new receivedData1;




	//�÷��̾� ĳ���� �ʱ� Ÿ�� �ε���
	_data->player.init_pos = { 43, 48 };


	//�� Ÿ�� �ʱ�ȭ
	for (int i = 0; i < MAXTILESIZE_NORMAL; i++)
	{
		for (int j = 0; j < MAXTILESIZE_NORMAL; j++)
		{
			_data->tile[i][j].x = (WINSIZEX / 2) + (((_data->player.init_pos.x - _data->player.init_pos.y) / 2) * TILEWIDTH) - (i * 64 / 2) + (j * 64 / 2) - 32;
			_data->tile[i][j].y = (WINSIZEY / 2) - (_data->player.init_pos.y * TILEHEIGHT) - (((_data->player.init_pos.x - _data->player.init_pos.y) / 2) * TILEHEIGHT) + (i * 32 / 2) + (j * 32 / 2) + 16;
			_data->tile[i][j].rc = RectMakeCenter(_data->tile[i][j].x, _data->tile[i][j].y, 64, 32);
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

			_data->tile[i][j].z_order = false;
			_data->tile[i][j].isFloodFill = false;

			_data->tile[i][j].index = { i, j };
		}
	}


	//A* ���� �ʱ�ȭ
	//Ÿ�ϳ�� �ʱ�ȭ
	for (int i = 0; i < MAXTILESIZE_NORMAL; i++)
	{
		for (int j = 0; j < MAXTILESIZE_NORMAL; j++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			_data->_totalNode[i][j] = new node(i, j); //�ɹ� �̴ϼ� ������� �ε����� ��� ���� �ʱ�ȭ
		}
	}



	//�� ���� �ε�
	{
		HANDLE file;
		DWORD read;

		//file = CreateFile("map_town.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		file = CreateFile("map_field.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE_NORMAL * MAXTILESIZE_NORMAL, &read, NULL);

		//�� Ÿ�� �ε�
		for (int i = 0; i < MAXTILESIZE_NORMAL; i++)
		{
			for (int j = 0; j < MAXTILESIZE_NORMAL; j++)
			{

				tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
				tile_img[i][j].img_floor[0] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[0]);
				tile_img[i][j].img_floor[1] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[1]);
				tile_img[i][j].img_floor[2] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[2]);
				tile_img[i][j].img_floor[3] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor[3]);
				tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //�ʱ� Ÿ��(�� Ÿ��)
				tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //�ʱ� Ÿ��(�� Ÿ��)

				//A* �� ó��
				if (tile_strKey[i][j].state_obj == BLOCK)
				{
					_data->_totalNode[i][j]->nodeState = NODE_WALL;
				}
			}
		}
		CloseHandle(file);
	}


	//���ʹ� �ʱ�ȭ
	{
		//ü�¹� ���̽� Ŭ���� �ʱ�ȭ
		_enemy_hpBar = new progressBar;
		_enemy_hpBar->init("IMAGE/ui/castle_hpBar_1.1496.128", "IMAGE/ui/castle_hpBar_back.1496.128", 300, 300, 40, 8);

		//���ʹ� ���̽� �ʱ�ȭ
		_enemy = new tagCharacter;
		{
			_enemy->state = IDLE;
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->runSpeed = 0.5f;
			_enemy->stat.hp = 100;
			_enemy->stat.hpMax = _enemy->stat.hp;
			_enemy->stat.att = 20;
		}


		//���� �ֱ�(ü�¹�, ĳ����)
		for (int p = 0; p < ENEMYNUM; p++)
		{
			_vEnemy_hpBar.push_back(*_enemy_hpBar);
			_vEnemy.push_back(*_enemy);
		}
		//���� ���ʹ� �ʱ�ȭ
		for (int p = 0; p < _vEnemy.size(); p++)
		{
			while (true)
			{
				int i = RND->range(4, 46);
				int j = RND->range(4, 46);
				if (_data->_totalNode[i][j]->nodeState == NODE_WALL) continue;
				else
				{
					_vEnemy[p].init_pos = { i, j };
					break;
				}
			}
		}
		for (int p = 0; p < _vEnemy.size(); p++)
		{
			_vEnemy[p].index.x = _vEnemy[p].init_pos.x;
			_vEnemy[p].index.y = _vEnemy[p].init_pos.y;
			_vEnemy[p].x = _data->tile[_vEnemy[p].index.x][_vEnemy[p].index.y].x;
			_vEnemy[p].y = _data->tile[_vEnemy[p].index.x][_vEnemy[p].index.y].y;
			_vEnemy[p].rc = RectMakeCenter(_vEnemy[p].x, _vEnemy[p].y, 3, 3);
			_vEnemy[p].count.base = RND->range(80);
			_data->_totalNode[_vEnemy[p].init_pos.x][_vEnemy[p].init_pos.y]->nodeState = NODE_WALL;
		}
	}


	//ĳ���� ���� �ʱ�ȭ
	{
		_data->player.index.x = _data->player.init_pos.x;
		_data->player.index.y = _data->player.init_pos.y;
		_data->player.x = _data->tile[_data->player.index.x][_data->player.index.y].x;
		_data->player.y = _data->tile[_data->player.index.x][_data->player.index.y].y;
		_data->player.rc = RectMakeCenter(_data->player.x, _data->player.y, 3, 3);
		_data->player.state = IDLE;
		_data->player.direc_LR = LEFT;
		_data->player.direc_DU = DOWN;
		_data->player.runSpeed = 1.0f;

		_data->player.stat.hp = 500;
		_data->player.stat.hpMax = _data->player.stat.hp;
		_data->player.stat.att = 10;

		//ü�¹� Ŭ���� �ʱ�ȭ
		hpBar = new progressBar;
		hpBar->init("IMAGE/ui/hpBar.322.8", "IMAGE/shadow", 64, 436, 322, 8);
		hpBar->setGauge(_data->player.stat.hp, _data->player.stat.hpMax);

		_data->_totalNode[_data->player.init_pos.x][_data->player.init_pos.y]->nodeState = NODE_WALL;
	}

	//npc ���� �ʱ�ȭ
	{
		npc.x = 11;
		npc.y = 10;
		npc.img = IMAGEMANAGER->findImage("npc������");
		npc.isTalk = false;

		//���� //41,45 / 24,14 / 18,8
		sign[0].x = 41;
		sign[0].y = 45;
		sign[0].isTalk = false;

		sign[1].x = 24;
		sign[1].y = 14;
		sign[1].isTalk = false;

		sign[2].x = 18;
		sign[2].y = 8;
		sign[2].isTalk = false;
	}



	//��ų ���� �ʱ�ȭ
	{
		ctrQ.i_range = 2;
		ctrQ.j_range = 2;
		ctrQ.shotTime = 2;//4��
		ctrQ.damage = 4;
		ctrQ.range = 3;

		ctrW.i_range = 1;
		ctrW.j_range = 1;
		ctrW.shotTime = 0;//4��
		ctrW.damage = 1;
		ctrW.range = 7;
	}

	world_count = 0;	//������ ī��Ʈ
	//ķ ���� ���콺
	_ptMouseC = { _ptMouse.x + (LONG)_data->player.x, _ptMouse.y + (LONG)_data->player.y };
	shackX = 0;
	shackY = 0;


	//���ʹ� AI ���� ����
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_root[i] = new Sequence;
		_sequence1[i] = new Sequence;
		//_selector1[i] = new Selector;
		//_enemy[i] = new tagCharacter;
		_approach[i] = new Approach(_data, &_vEnemy[i]);
		_attack[i] = new Attack(_data, &_vEnemy[i]);


		//BT AI ���� ����
		_root[i]->addChild(_sequence1[i]);
		_sequence1[i]->addChild(_approach[i]);
		_sequence1[i]->addChild(_attack[i]);
	}


	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void fieldScene::release()
{
	

	//�ε��� Ŭ���� ����
	hpBar->release();
	SAFE_DELETE(hpBar);
	_enemy_hpBar->release();
	SAFE_DELETE(_enemy_hpBar);
	SAFE_DELETE(_enemy);

	/*for (int i = 0; i < MAXTILESIZE_NORMAL; i++)
	{
		for (int j = 0; j < MAXTILESIZE_NORMAL; j++)
		{
			SAFE_DELETE(_data->_totalNode[i][j]);
		}
	}*/
	
}

void fieldScene::update()
{
	world_count++;

	//���ʹ� AI üũ
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		_vEnemy[p].count.base++;
		if (_vEnemy[p]._finalList.size() > 0) this->move(&_vEnemy[p]); //move
		if (_vEnemy[p].count.base % 80 == 0) _root[p]->run(); //BT AI
	}

	//���ʹ� ����
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		if (_vEnemy[p].stat.hp <= 0)
		{
			_vEnemy.erase(_vEnemy.begin() + p);
			SOUNDMANAGER->play("geno4");
			_vEnemy_hpBar.erase(_vEnemy_hpBar.begin() + p);
			p--;
		}
	}

	//���ʹ� ü�¹� ������Ʈ
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		_vEnemy_hpBar[p].update();
		_vEnemy_hpBar[p].setGauge(_vEnemy[p].stat.hp, _vEnemy[p].stat.hpMax);
		//���ʹ� rc ������Ʈ
		_vEnemy[p].rc = RectMakeCenter(_vEnemy[p].x, _vEnemy[p].y, 3, 3);
	}

	//�÷��̾� ü�¹� ������Ʈ
	hpBar->update();
	hpBar->setGauge(_data->player.stat.hp, _data->player.stat.hpMax);



	//for�� üũ ����
	{
		//ȭ�� ���� ���� - ���� 10, ���� 14
		x_for_start = _data->player.index.x - 11;
		if (x_for_start < 0) x_for_start = 0;
		x_for_end = _data->player.index.x + 11;
		if (x_for_end > MAXTILESIZE_NORMAL) x_for_end = MAXTILESIZE_NORMAL;
		y_for_start = _data->player.index.y - 11;
		if (y_for_start < 0) y_for_start = 0;
		y_for_end = _data->player.index.y + 11;
		if (y_for_end > MAXTILESIZE_NORMAL) y_for_end = MAXTILESIZE_NORMAL;

		//z-order üũ ���� - ���� 4, ���� 4
		x_for_start_z = _data->player.index.x - 5;
		if (x_for_start_z < 0) x_for_start_z = 0;
		x_for_end_z = _data->player.index.x + 5;
		if (x_for_end_z > MAXTILESIZE_NORMAL) x_for_end_z = MAXTILESIZE_NORMAL;
		y_for_start_z = _data->player.index.y - 5;
		if (y_for_start_z < 0) y_for_start_z = 0;
		y_for_end_z = _data->player.index.y + 5;
		if (y_for_end_z > MAXTILESIZE_NORMAL) y_for_end_z = MAXTILESIZE_NORMAL;
	}


	//���� ��Ŭ�� ��Ʈ�� ��(NPC ���ó��, �̵�, A*)
	this->userSkill();
	//�߰� A* ������Ʈ
	this->move(&_data->player);


	//�÷��̾� ���� z���� ó��
	this->checkZorder(_data->player.index.x, _data->player.index.y, 6);

	//������ �̵�
	if (_data->player.index.x == 43 && _data->player.index.y == 49)
	{
		_data->player._isFind = false;
		//��� Ŭ����
		_data->player._openList.clear();
		_data->player._closeList.clear();
		_data->player._finalList.clear();
		_data->player.nodeNum = 0;
		SOUNDMANAGER->play("close", 0.2f);
		SCENEMANAGER->loadScene("����");
	}

	//���������� �̵�
	if (_data->player.index.x == 3 && _data->player.index.y == 6)
	{
		_data->player._isFind = false;
		//��� Ŭ����
		_data->player._openList.clear();
		_data->player._closeList.clear();
		_data->player._finalList.clear();
		_data->player.nodeNum = 0;
		SOUNDMANAGER->play("unlock", 0.4f);
		SCENEMANAGER->loadScene("�����ʵ�");
	}



	//ĳ������ RC
	_data->player.rc = RectMakeCenter(_data->player.x, _data->player.y, 3, 3);
	rc_clipping = RectMakeCenter(_data->player.x, _data->player.y, WINSIZEX + 8, WINSIZEY + 8);
	//ķ ���� ���콺 ��ǥ ������Ʈ
	_ptMouseC = {
		_ptMouse.x - (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].x - (LONG)_data->player.x),
		_ptMouse.y - (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].y - (LONG)_data->player.y) };
}

void fieldScene::render()
{

	//ī�޶� ���� �� ����
	{
		//Ÿ�� ����
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &_data->tile[i][j].rc))
				{
					if (tile_strKey[i][j].isImg)
					{
						if (tile_strKey[i][j].floor == 0)
						{
							tile_img[i][j].img->render(getMemDC(),
								_data->tile[i][j].rc.left + XX,
								_data->tile[i][j].rc.top - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY); //Ÿ�� �̹��� ����
						}
						if (!_data->tile[i][j].z_order && tile_strKey[i][j].floor > 0)
						{
							tile_img[i][j].img->render(getMemDC(),
								_data->tile[i][j].rc.left + XX,
								_data->tile[i][j].rc.top - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY); //Ÿ�� �̹��� ����

							for (int k = 0; k < tile_strKey[i][j].floor; k++)
								tile_img[i][j].img_floor[k]->render(getMemDC(),
									_data->tile[i][j].rc.left + XX,
									_data->tile[i][j].rc.top - (TILEHEIGHT / 2) * k + YY);
						}
					}

				}
			}
		}

		//���� �� �Ѿ�� ��Ż �̹��� ����
		IMAGEMANAGER->findImage("��Ż")->frameRender(getMemDC(), _data->tile[3][7].rc.left + XX, _data->tile[3][7].rc.bottom - 180 + YY);


		//���ʹ� + ������Ʈ ����
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &_data->tile[i][j].rc))
				{
					//���ʹ� ����
					if (tile_strKey[i][j].isImg_enemy)
						tile_img[i][j].img_enemy->render(getMemDC(),
							_data->tile[i][j].rc.left + XX - tile_img[i][j].img_enemy->getWidth() / 2 + 32,
							_data->tile[i][j].rc.bottom - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY - tile_img[i][j].img_enemy->getHeight());

					//������Ʈ ���� + ���� ó��
					if (!_data->tile[i][j].z_order)
					{
						if (tile_strKey[i][j].isImg_obj)
							tile_img[i][j].img_obj->render(getMemDC(), _data->tile[i][j].rc.left + XX - tile_img[i][j].img_obj->getWidth() / 2 + 32,
								_data->tile[i][j].rc.bottom - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY - tile_img[i][j].img_obj->getHeight());
					}
				}
			}
		}

	}



	//��ų ���� ����(�׽�Ʈ)
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_PLUS))
	{
		ctrQ.i_range++;
		ctrQ.j_range++;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_MINUS))
	{
		ctrQ.i_range--;
		ctrQ.j_range--;
	}


	//Q ��ų ���� ǥ�� skill_hitRange
	if (ctrQ.skill_ready)
	{
		for (int i = _data->player.index.x - ctrQ.range; i < _data->player.index.x + ctrQ.range + 1; i++)
		{
			for (int j = _data->player.index.y - ctrQ.range; j < _data->player.index.y + ctrQ.range + 1; j++)
			{
				IMAGEMANAGER->findImage("skill_range")->render(getMemDC(), _data->tile[i][j].rc.left + XX, _data->tile[i][j].rc.top + YY);
			}
		}

		for (int i = _data->player.index.x - ctrQ.range; i < _data->player.index.x + ctrQ.range + 1; i++)
		{
			for (int j = _data->player.index.y - ctrQ.range; j < _data->player.index.y + ctrQ.range + 1; j++)
			{
				if (PICKING)
				{
					ctrQ.markPoint = { i, j };
					ctrQ.i = { ctrQ.markPoint.x - ctrQ.i_range, ctrQ.markPoint.x + ctrQ.i_range + 1 };
					for (ctrQ.i.x; ctrQ.i.x < ctrQ.i.y; ctrQ.i.x++)
					{
						ctrQ.j = { ctrQ.markPoint.y - ctrQ.j_range, ctrQ.markPoint.y + ctrQ.j_range + 1 };
						for (ctrQ.j.x; ctrQ.j.x < ctrQ.j.y; ctrQ.j.x++)
						{
							IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[ctrQ.i.x][ctrQ.j.x].rc.left + XX, _data->tile[ctrQ.i.x][ctrQ.j.x].rc.top + YY);
						}
					}

					//��ų �߻�
					if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
					{
						ctrQ.skill_shot = true;
						ctrQ.skill_ready = false;
						ctrQ.start = TIMEMANAGER->getWorldTime();
						if (!SOUNDMANAGER->isPlaySound("meteo1")) SOUNDMANAGER->play("meteo1");
					}
				}
			}
		}
	}
	if (ctrQ.skill_shot)
	{
		
		if (TIMEMANAGER->getWorldTime() < ctrQ.start + ctrQ.shotTime)
		{
			if ((world_count / 16) % 2 == 0)
			{
				ctrQ.i = { ctrQ.markPoint.x - ctrQ.i_range, ctrQ.markPoint.x + ctrQ.i_range + 1 };
				for (ctrQ.i.x; ctrQ.i.x < ctrQ.i.y; ctrQ.i.x++)
				{
					ctrQ.j = { ctrQ.markPoint.y - ctrQ.j_range, ctrQ.markPoint.y + ctrQ.j_range + 1 };
					for (ctrQ.j.x; ctrQ.j.x < ctrQ.j.y; ctrQ.j.x++)
					{
						IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[ctrQ.i.x][ctrQ.j.x].rc.left + XX, _data->tile[ctrQ.i.x][ctrQ.j.x].rc.top + YY);
					}
				}
			}
		}
		else if (TIMEMANAGER->getWorldTime() > ctrQ.start + ctrQ.shotTime)
		{
			ctrQ.i = { ctrQ.markPoint.x - ctrQ.i_range, ctrQ.markPoint.x + ctrQ.i_range + 1 };
			for (ctrQ.i.x; ctrQ.i.x < ctrQ.i.y; ctrQ.i.x++)
			{
				ctrQ.j = { ctrQ.markPoint.y - ctrQ.j_range, ctrQ.markPoint.y + ctrQ.j_range + 1 };
				for (ctrQ.j.x; ctrQ.j.x < ctrQ.j.y; ctrQ.j.x++)
				{
					IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[ctrQ.i.x][ctrQ.j.x].rc.left + XX, _data->tile[ctrQ.i.x][ctrQ.j.x].rc.top + YY);
				}
			}
		}
	}
	//W ��ų ���� ǥ�� skill_hitRange
	if (ctrW.skill_ready)
	{
		for (int i = _data->player.index.x - ctrW.range; i < _data->player.index.x + ctrW.range + 1; i++)
		{
			for (int j = _data->player.index.y - ctrW.range; j < _data->player.index.y + ctrW.range + 1; j++)
			{
				IMAGEMANAGER->findImage("skill_range")->render(getMemDC(), _data->tile[i][j].rc.left + XX, _data->tile[i][j].rc.top + YY);
			}
		}

		for (int i = _data->player.index.x - ctrW.range; i < _data->player.index.x + ctrW.range + 1; i++)
		{
			for (int j = _data->player.index.y - ctrW.range; j < _data->player.index.y + ctrW.range + 1; j++)
			{
				if (PICKING)
				{
					ctrW.markPoint = { i, j };
					ctrW.i = { ctrW.markPoint.x - ctrW.i_range, ctrW.markPoint.x + ctrW.i_range + 1 };
					for (ctrW.i.x; ctrW.i.x < ctrW.i.y; ctrW.i.x++)
					{
						ctrW.j = { ctrW.markPoint.y - ctrW.j_range, ctrW.markPoint.y + ctrW.j_range + 1 };
						for (ctrW.j.x; ctrW.j.x < ctrW.j.y; ctrW.j.x++)
						{
							IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[ctrW.i.x][ctrW.j.x].rc.left + XX, _data->tile[ctrW.i.x][ctrW.j.x].rc.top + YY);
						}
					}

					//��ų �߻�
					if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
					{
						ctrW.skill_shot = true;
						ctrW.skill_ready = false;
						ctrW.start = TIMEMANAGER->getWorldTime();
						SOUNDMANAGER->play("magic04_cut");
					}
				}
			}
		}
	}
	if (ctrW.skill_shot)
	{
		ctrW.i = { ctrW.markPoint.x - ctrW.i_range, ctrW.markPoint.x + ctrW.i_range + 1 };
		for (ctrW.i.x; ctrW.i.x < ctrW.i.y; ctrW.i.x++)
		{
			ctrW.j = { ctrW.markPoint.y - ctrW.j_range, ctrW.markPoint.y + ctrW.j_range + 1 };
			for (ctrW.j.x; ctrW.j.x < ctrW.j.y; ctrW.j.x++)
			{
				IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[ctrW.i.x][ctrW.j.x].rc.left + XX, _data->tile[ctrW.i.x][ctrW.j.x].rc.top + YY);
			}
		}
	}




	//ĳ���� ����
	if (_data->player.state == IDLE)
	{
		if (world_count % 16 == 0)
		{
			if (_data->player.count.idle == 2) _data->player.count.idle = (-1);
			_data->player.count.idle++;
		}
		IMAGEMANAGER->findImage("idle")->frameRender(getMemDC(), _data->player.rc.left - 195 / 6 + 8 + XX, _data->player.rc.top - 60 + YY, _data->player.count.idle, 0);
	}
	if (_data->player.state == RUN)
	{
		switch (_data->player.direc_DU)
		{
		case DOWN:
			switch (_data->player.direc_LR)
			{
			case LEFT:
				if (world_count % 8 == 0)
				{
					if (_data->player.count.run1 == 3) _data->player.count.run1 = (-1);
					_data->player.count.run1++;
				}
				IMAGEMANAGER->findImage("run1")->frameRender(getMemDC(), _data->player.rc.left - 330 / 8 + 8 + XX, _data->player.rc.top - 60 + YY, _data->player.count.run1, 0);
				break;
			case RIGHT:
				if (world_count % 8 == 0)
				{
					if (_data->player.count.run2 == 3) _data->player.count.run2 = (-1);
					_data->player.count.run2++;
				}
				IMAGEMANAGER->findImage("run2")->frameRender(getMemDC(), _data->player.rc.left - 330 / 8 + 8 + XX, _data->player.rc.top - 60 + YY, _data->player.count.run2, 0);
				break;
			}
			break;
		case UP:
			switch (_data->player.direc_LR)
			{
			case LEFT:
				if (world_count % 8 == 0)
				{
					if (_data->player.count.run3 == 3) _data->player.count.run3 = (-1);
					_data->player.count.run3++;
				}
				IMAGEMANAGER->findImage("run3")->frameRender(getMemDC(), _data->player.rc.left - 330 / 8 + 8 + XX, _data->player.rc.top - 60 + YY, _data->player.count.run3, 0);
				break;
			case RIGHT:
				if (world_count % 8 == 0)
				{
					if (_data->player.count.run4 == 3) _data->player.count.run4 = (-1);
					_data->player.count.run4++;
				}
				IMAGEMANAGER->findImage("run4")->frameRender(getMemDC(), _data->player.rc.left - 330 / 8 + 8 + XX, _data->player.rc.top - 60 + YY, _data->player.count.run4, 0);
				break;
			}
			break;
		}
	}
	//Rectangle(getMemDC(), _data->player.rc);


	//���ʹ� ����  { IDLE, BEHIT, RUN, ATTACK };
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		if (KEYMANAGER->isToggleKey(VK_F11)) _vEnemy[p].state = BEHIT;
		switch (_vEnemy[p].state)
		{
		case IDLE:
			if (_vEnemy[p].count.base % 16 == 0)
			{
				if (_vEnemy[p].count.idle == 2) _vEnemy[p].count.idle = (-1);
				_vEnemy[p].count.idle++;
			}
			IMAGEMANAGER->findImage("zombie_idle")->frameRender(getMemDC(), _vEnemy[p].rc.left - 128 / 6 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.idle, 0);
			//TextOut(getMemDC(), _vEnemy[p].rc.right + 20 + XX, _vEnemy[p].rc.top + YY, "IDLE", strlen("IDLE"));
			break;
		case BEHIT:
			IMAGEMANAGER->findImage("zombie_beHit")->render(getMemDC(), _vEnemy[p].rc.left - 32 + XX, _vEnemy[p].rc.top - 96 + 8 + YY);
			//TextOut(getMemDC(), _vEnemy[p].rc.right + 20 + XX, _vEnemy[p].rc.top + YY, "BEHIT", strlen("BEHIT"));
			break;
		case RUN:
			switch (_vEnemy[p].direc_DU)
			{
			case DOWN:
				switch (_vEnemy[p].direc_LR)
				{
				case LEFT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.run1 == 3) _vEnemy[p].count.run1 = (-1);
						_vEnemy[p].count.run1++;
					}
					IMAGEMANAGER->findImage("zombie_run1")->frameRender(getMemDC(), _vEnemy[p].rc.left - 256 / 8 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.run1, 0);
					break;
				case RIGHT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.run2 == 3) _vEnemy[p].count.run2 = (-1);
						_vEnemy[p].count.run2++;
					}
					IMAGEMANAGER->findImage("zombie_run2")->frameRender(getMemDC(), _vEnemy[p].rc.left - 256 / 8 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.run2, 0);
					break;
				}
				break;
			case UP:
				switch (_vEnemy[p].direc_LR)
				{
				case LEFT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.run3 == 3) _vEnemy[p].count.run3 = (-1);
						_vEnemy[p].count.run3++;
					}
					IMAGEMANAGER->findImage("zombie_run3")->frameRender(getMemDC(), _vEnemy[p].rc.left - 256 / 8 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.run3, 0);
					break;
				case RIGHT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.run4 == 3) _vEnemy[p].count.run4 = (-1);
						_vEnemy[p].count.run4++;
					}
					IMAGEMANAGER->findImage("zombie_run4")->frameRender(getMemDC(), _vEnemy[p].rc.left - 256 / 8 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.run4, 0);
					break;
				}
				break;
			}
			//TextOut(getMemDC(), _vEnemy[p].rc.right + 20 + XX, _vEnemy[p].rc.top + YY, "RUN", strlen("RUN"));
			break;
		case ATTACK:
			switch (_vEnemy[p].direc_DU)
			{
			case DOWN:
				switch (_vEnemy[p].direc_LR)
				{
				case LEFT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.attack1 == 5)
						{
							_data->player.stat.hp -= _vEnemy[p].stat.att; //ü�� ���
							SOUNDMANAGER->play("attack", 0.4f);
						}
						if (_vEnemy[p].count.attack1 == 7) _vEnemy[p].count.attack1 = (-1);
						_vEnemy[p].count.attack1++;
					}
					IMAGEMANAGER->findImage("zombie_attack1")->frameRender(getMemDC(), _vEnemy[p].rc.left - 576 / 16 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.attack1, 0);
					break;
				case RIGHT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						_vEnemy[p].count.attack2--;
						if (_vEnemy[p].count.attack2 == 2)
						{
							_data->player.stat.hp -= _vEnemy[p].stat.att; //ü�� ���
							SOUNDMANAGER->play("attack", 0.4f);
						}
						if (_vEnemy[p].count.attack2 == -1) _vEnemy[p].count.attack2 = 7;
					}
					IMAGEMANAGER->findImage("zombie_attack2")->frameRender(getMemDC(), _vEnemy[p].rc.left - 576 / 16 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.attack2, 0);
					break;
				}
				break;
			case UP:
				switch (_vEnemy[p].direc_LR)
				{
				case LEFT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						_vEnemy[p].count.attack3--;
						if (_vEnemy[p].count.attack3 == 2)
						{
							_data->player.stat.hp -= _vEnemy[p].stat.att; //ü�� ���
							SOUNDMANAGER->play("attack", 0.4f);
						}
						if (_vEnemy[p].count.attack3 == -1) _vEnemy[p].count.attack3 = 7;
					}
					IMAGEMANAGER->findImage("zombie_attack3")->frameRender(getMemDC(), _vEnemy[p].rc.left - 576 / 16 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.attack3, 0);
					break;
				case RIGHT:
					if (_vEnemy[p].count.base % 8 == 0)
					{
						if (_vEnemy[p].count.attack4 == 5)
						{
							_data->player.stat.hp -= _vEnemy[p].stat.att; //ü�� ���
							SOUNDMANAGER->play("attack", 0.4f);
						}
						if (_vEnemy[p].count.attack4 == 7) _vEnemy[p].count.attack4 = -1;
						_vEnemy[p].count.attack4++;
					}
					IMAGEMANAGER->findImage("zombie_attack4")->frameRender(getMemDC(), _vEnemy[p].rc.left - 576 / 16 + XX, _vEnemy[p].rc.top - 96 + 8 + YY, _vEnemy[p].count.attack4, 0);
					break;
				}
				break;
			}
			//TextOut(getMemDC(), _vEnemy[p].rc.right + 20 + XX, _vEnemy[p].rc.top + YY, "ATTACK", strlen("ATTACK"));
			break;
		}
	}

	
	//���ʹ� ü�¹� ����
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		_vEnemy_hpBar[p].setX(_vEnemy[p].rc.left - 20 + XX);
		_vEnemy_hpBar[p].setY(_vEnemy[p].rc.top - 70 + YY);
		_vEnemy_hpBar[p].render();
		
		//textOutDbug(getMemDC(), _vEnemy[p].rc.right + 30 + XX, _vEnemy[p].rc.top - 70 + YY, _vEnemy[p].stat.hp); //���ʹ� ü�� Ȯ�� �����
		//textOutDbug(getMemDC(), _vEnemy[p].rc.right + 30 + XX, _vEnemy[p].rc.top - 70 + YY + 20, _vEnemy[p].index.x); //���ʹ� �ε��� Ȯ�� �����
		//textOutDbug(getMemDC(), _vEnemy[p].rc.right + 30 + XX, _vEnemy[p].rc.top - 70 + YY + 40, _vEnemy[p].index.y); //���ʹ� �ε��� Ȯ�� �����
	}

	//AI���� Ŭ���� ��Ʈ
	_data->range = RectMakeCenter(_data->player.x, _data->player.y, WINSIZEX - 32, WINSIZEY - 32);


	//npc����
	//��Ż
	if (world_count % 4 == 0)
	{
		if (IMAGEMANAGER->findImage("��Ż")->getFrameX() == 5)
		{
			IMAGEMANAGER->findImage("��Ż")->setFrameX(-1);
		}
		IMAGEMANAGER->findImage("��Ż")->setFrameX(IMAGEMANAGER->findImage("��Ż")->getFrameX() + 1);
	}
	IMAGEMANAGER->findImage("��Ż")->frameRender(getMemDC(), _data->tile[43][49].rc.left + XX, _data->tile[43][49].rc.bottom - 180 + YY);



	//Q ��ų �߻� ���� + ������ ó��
	if (ctrQ.skill_shot)
	{
		if (TIMEMANAGER->getWorldTime() > ctrQ.start + ctrQ.shotTime)
		{
			if (world_count % 4 == 0)
			{
				if (6 <= ctrQ.count)
				{
					sightOn = false;
					shackX = RND->range(-4, 4) * 8;
					shackY = RND->range(-4, 4) * 8;
				}
				ctrQ.count++;
				if (ctrQ.count == 46)
				{
					sightOn = true;
					shackX = 0;
					shackY = 0;
					ctrQ.count = -1;
					ctrQ.skill_shot = false; //��ų ����
				}
			}
			if (0 <= ctrQ.count && ctrQ.count < 6)
			{
				
				IMAGEMANAGER->findImage("skill_meteo")->frameRender(getMemDC(), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.left + XX - 8 + 32 * (5 - ctrQ.count), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.top - 172 - 32 * (5 - ctrQ.count) + YY, ctrQ.count, 0);
			}
			else if (6 <= ctrQ.count  && ctrQ.count < 32)
			{
				if (!SOUNDMANAGER->isPlaySound("meteo2")) SOUNDMANAGER->play("meteo2");
				if (!SOUNDMANAGER->isPlaySound("meteo3")) SOUNDMANAGER->play("meteo3");
				IMAGEMANAGER->findImage("skill_meteo")->frameRender(getMemDC(), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.left + XX - 64, _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.top - 128 + YY, ctrQ.count, 0);

				//������ ó��
				if (world_count % 4 == 0)
				{
					for (int p = 0; p < _vEnemy.size(); p++)
					{
						if (ctrQ.markPoint.x - ctrQ.i_range <= _vEnemy[p].index.x && _vEnemy[p].index.x <= ctrQ.markPoint.x + ctrQ.i_range &&
							ctrQ.markPoint.y - ctrQ.j_range <= _vEnemy[p].index.y && _vEnemy[p].index.y <= ctrQ.markPoint.y + ctrQ.j_range)
						{
							_vEnemy[p].stat.hp -= ctrQ.damage;
						}
					}
				}
			}
		}
	}
	//W ��ų �߻� ���� + ������ ó��
	if (ctrW.skill_shot)
	{
		if (world_count % 2 == 0)
		{
			if (0 <= ctrW.count < 4)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				//������ ó��
				for (int p = 0; p < _vEnemy.size(); p++)
				{
					if (ctrW.markPoint.x - ctrW.i_range <= _vEnemy[p].index.x && _vEnemy[p].index.x <= ctrW.markPoint.x + ctrW.i_range &&
						ctrW.markPoint.y - ctrW.j_range <= _vEnemy[p].index.y && _vEnemy[p].index.y <= ctrW.markPoint.y + ctrW.j_range)
					{
						_vEnemy[p].stat.hp -= ctrW.damage;
					}
				}
			}
			if (ctrW.count == 5) shackY = 0;

			ctrW.count++;
			if (ctrW.count == 12)
			{
				sightOn = true;
				ctrW.count = -1;
				ctrW.skill_shot = false; //��ų ����
			}
		}

		if (0 <= ctrW.count && ctrW.count < 4)
			IMAGEMANAGER->findImage("skill_thunder")->frameRender(getMemDC(), _data->tile[ctrW.markPoint.x][ctrW.markPoint.y].rc.left + XX, _data->tile[ctrW.markPoint.x][ctrW.markPoint.y].rc.top - 416 + YY, ctrW.count, 0);
		else if (4 <= ctrW.count  && ctrW.count < 12)
			IMAGEMANAGER->findImage("skill_thunder2")->frameRender(getMemDC(), _data->tile[ctrW.markPoint.x][ctrW.markPoint.y].rc.left - 32 + XX, _data->tile[ctrW.markPoint.x][ctrW.markPoint.y].rc.top - 48 + YY, ctrW.count, 0);

	}

	//npc ����
	if (world_count % 14 == 0) //������
	{
		if (npc.img->getFrameX() == 2)
		{
			npc.img->setFrameX(-1);
		}
		npc.img->setFrameX(npc.img->getFrameX() + 1);
	}
	npc.img->frameRender(getMemDC(), _data->tile[npc.x][npc.y].rc.left - 4 + XX, _data->tile[npc.x][npc.y].rc.bottom - 4 - npc.img->getHeight() + YY);



	//��ǳ�� ����
	if ((-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0)
	{
		if ((world_count / 20) % 2 == 1) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(0);
		else if ((world_count / 20) % 2 == 0) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(1);
		IMAGEMANAGER->findImage("talkBalloon")->frameRender(getMemDC(), _ptMouse.x - IMAGEMANAGER->findImage("talkBalloon")->getFrameWidth() / 2, _ptMouse.y - npc.img->getFrameHeight());
	}
	for (int i = 0; i < 3; i++)
	{
		if ((-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0)
		{
			if ((world_count / 20) % 2 == 1) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(0);
			else if ((world_count / 20) % 2 == 0) IMAGEMANAGER->findImage("talkBalloon")->setFrameX(1);
			IMAGEMANAGER->findImage("talkBalloon")->frameRender(getMemDC(), _ptMouse.x, _ptMouse.y - 80);
		}
	}





	//(Z����)Ÿ�� ���� + Ŭ���� ó�� + ������Ʈ ����
	for (int i = x_for_start; i < x_for_end; i++)
	{
		for (int j = y_for_start; j < y_for_end; j++)
		{
			if (IntersectRect(&rc_temp, &rc_clipping, &_data->tile[i][j].rc))
			{
				if (tile_strKey[i][j].isImg)
				{
					//Ÿ�� ����
					if (_data->tile[i][j].z_order && tile_strKey[i][j].floor > 0)
					{
						tile_img[i][j].img->alphaRender(getMemDC(),
							_data->tile[i][j].rc.left + XX,
							_data->tile[i][j].rc.top - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY, 128); //Ÿ�� �̹��� ����

						for (int k = 0; k < tile_strKey[i][j].floor; k++)
							tile_img[i][j].img_floor[k]->alphaRender(getMemDC(),
								_data->tile[i][j].rc.left + XX,
								_data->tile[i][j].rc.top - (TILEHEIGHT / 2) * k + YY, 128);
					}
				}

				//������Ʈ
				if (_data->tile[i][j].z_order)
				{
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj->alphaRender(getMemDC(), _data->tile[i][j].rc.left + XX - tile_img[i][j].img_obj->getWidth() / 2 + 32,
							_data->tile[i][j].rc.bottom - (TILEHEIGHT / 2) * tile_strKey[i][j].floor + YY - tile_img[i][j].img_obj->getHeight(), 128);
				}
			}
		}
	}



	


	//textOutDbug(getMemDC(), 20, 40, _vEnemy[1].stat.hp);



	// < ġƮ > ��� Ű1
	{
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &_data->tile[i][j].rc))
				{
					if (KEYMANAGER->isToggleKey(VK_F5)) drawTileIndex(_data->tile[i][j], i, j, _data->tile[_data->player.init_pos.x][_data->player.init_pos.y]);//Ÿ�� �ε��� ����
					//if (KEYMANAGER->isToggleKey(VK_F6)) FrameRect(getMemDC(), tile[i][j].rc_picking, RGB(255, 0, 255));//��ŷ ���� ����
					if (KEYMANAGER->isToggleKey(VK_F7))
					{
						drawDiamond(_data->tile[i][j], PS_DOT, RGB(255, 255, 0));//���̼� Ÿ�� ����
						//Ŭ���� ���� ���<- ����� ī�޶��?
						FrameRect(getMemDC(), rc_clipping, RGB(255, 0, 255));
						//����� �����ֱ�
						if (_data->_totalNode[i][j]->nodeState == NODE_WALL)
						{
							setNodeColor(_data->tile[i][j], RGB(200, 150, 100));
						}
					}
					if (KEYMANAGER->isToggleKey(VK_F8)) //�̵� ���� ���� üũ ���� _data->_totalNode[i][j]->nodeState = NODE_WALL;
					{
						if (_data->_totalNode[i][j]->nodeState == NODE_WALL)
						{
							IMAGEMANAGER->findImage("tile_block")->alphaRender(getMemDC(), _data->tile[i][j].rc.left + XX,
								_data->tile[i][j].rc.top + YY, 80);
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
				if (PICKING)
					drawDiamond(_data->tile[i][j], PS_SOLID, RGB(255, 0, 255));
			}
			if (PICKING && tile_strKey[i][j].state_obj != BLOCK)
				drawDiamond(_data->tile[i][j], PS_SOLID, RGB(0, 255, 0), _data->tile[_data->player.init_pos.x][_data->player.init_pos.y]);
			else if (PICKING && tile_strKey[i][j].state_obj == BLOCK)
				drawDiamond(_data->tile[i][j], PS_SOLID, RGB(255, 0, 0), _data->tile[_data->player.init_pos.x][_data->player.init_pos.y]);
		}
	}


	//A* ���
	if (KEYMANAGER->isToggleKey(VK_F9))
	{
		for (int p = 0; p < _vEnemy.size(); p++)//���ʹ�
		{
			//��ã������ �����ֱ�
			if (_vEnemy[p]._isFind)
			{
				for (int i = 0; i < _vEnemy[p]._finalList.size(); i++)
				{

					if (KEYMANAGER->isToggleKey(VK_F7)) this->drawDiamond(_data->tile[_vEnemy[p]._finalList[i]->index.x][_vEnemy[p]._finalList[i]->index.y], PS_SOLID, RGB(255, 0, 255));
					this->drawDiamond(_data->tile[_vEnemy[p]._finalList[i]->index.x][_vEnemy[p]._finalList[i]->index.y], PS_SOLID, RGB(200, 0, 0), _data->tile[_data->player.init_pos.x][_data->player.init_pos.y]);
				}
			}
		}
		if (_data->player._isFind)//�÷��̾�
		{
			for (int i = 0; i < _data->player._finalList.size(); i++)
			{
				if (KEYMANAGER->isToggleKey(VK_F7)) this->drawDiamond(_data->tile[_data->player._finalList[i]->index.x][_data->player._finalList[i]->index.y], PS_SOLID, RGB(255, 0, 255));
				this->drawDiamond(_data->tile[_data->player._finalList[i]->index.x][_data->player._finalList[i]->index.y], PS_SOLID, RGB(50, 255, 50), _data->tile[_data->player.init_pos.x][_data->player.init_pos.y]);
			}
		}
	}



	//�þ� ó��
	if (sightOn) GDIPLUS->gdiRender("viewSight", getMemDC());
	if (!sightOn) GDIPLUS->gdiRender("viewSight", getMemDC(), -64, -64, WINSIZEX + 128, WINSIZEY + 128, 0);


	//�÷��̾� hud ���
	{
		IMAGEMANAGER->findImage("uiBar2")->render(getMemDC(), 0, 360);
		hpBar->render();

		IMAGEMANAGER->findImage("mpBar")->render(getMemDC(), 64, 448);
		IMAGEMANAGER->findImage("uiBar")->render(getMemDC(), 0, 360);
		if (_data->player.state == IDLE) IMAGEMANAGER->findImage("face07a")->render(getMemDC(), -24, 396);
		if (_data->player.state == RUN) IMAGEMANAGER->findImage("face07c")->render(getMemDC(), -24, 396);
		//IMAGEMANAGER->findImage("face07d")->render(getMemDC(), -24, 396);
		//IMAGEMANAGER->findImage("face07j")->render(getMemDC(), -24, 396);

		IMAGEMANAGER->findImage("skill_frame")->render(getMemDC(), 524, 420);
		IMAGEMANAGER->findImage("skill_frame")->render(getMemDC(), 580, 420);
		IMAGEMANAGER->findImage("skill_icon1")->render(getMemDC(), 528, 424);
		IMAGEMANAGER->findImage("skill_icon2")->render(getMemDC(), 584, 424);

		textOutFont(getMemDC(), 559, 421, "Q", 20, RGB(220, 220, 32), "�ü�");
		textOutFont(getMemDC(), 613, 421, "W", 20, RGB(220, 220, 32), "�ü�");

		textOutFont(getMemDC(), 190, 434, "(", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(),200, 434, _data->player.stat.hp, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 230, 434, "/", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(), 240, 434, _data->player.stat.hpMax, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 270, 434, ")", 16, RGB(200, 200, 0));
	}



	// < ġƮ > ��� Ű2
	if (KEYMANAGER->isToggleKey(VK_F7))
	{
		FrameRect(getMemDC(), _data->player.rc, RGB(255, 255, 255));
		char str_test2[128];
		sprintf(str_test2, "%d, %d / [%d, %d] / state: %d", (int)_data->player.x, (int)_data->player.y, _data->player.index.x, _data->player.index.y, _data->player.state);
		TextOut(getMemDC(), _data->player.x, _data->player.y, str_test2, strlen(str_test2));
		IMAGEMANAGER->findImage("���콺�׽�Ʈ")->render(getMemDC(), _ptMouseC.x, _ptMouseC.y);
	}

	//npc ���â ����
	{

		if (npc.isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);//RectMakeCenter(WINSIZEX / 2, 400, 590, 100)
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "�տ� ���� ǥ������ ���� ���ߴ°�?", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "���ư���... �̰��� �ʹ� ������...", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 60, "�˾��� ���� ��ġŷ               �� �ִ� ���ϼ�...", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 60, "                              �����ء�", 20, RGB(200, 55, 55));
		}
		if (sign[0].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "- ���� ȨŸ�� -", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (���ƿͿ� ȨŸ�...)", 20, RGB(255, 255, 255));
		}
		if (sign[1].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "- �� : ������ -", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (���� : ������� ������ ����� ������ ������ �Դϴ� !!)", 20, RGB(255, 255, 255));
		}
		if (sign[2].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "- �� : ��ɲ��� ���� -", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "- �ϵ� : ......Di..e..... - ", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 60, "   (����� �������� ���ÿ� !!)", 20, RGB(255, 255, 255));
		}
	}

	//textOutDbug(getMemDC(), _data->player.rc.right, _data->player.rc.top, _data->player.x);
	//textOutDbug(getMemDC(), _data->player.rc.right, _data->player.rc.top + 20, _data->player.y);
	//textOutDbug(getMemDC(), _data->player.rc.right + XX, _data->player.rc.top + YY, _data->player.stat.hp);


	SetBkMode(getMemDC(), 2);

	//TextOut(getMemDC(), WINSIZEX / 2, 0, "�Ϲ��ʵ�", strlen("�Ϲ��ʵ�"));
	fade(getMemDC(), _startTime, true);
}



//���� ��ų�� �̵� ó��
void fieldScene::userSkill()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		//���â �ݱ�
		npc.isTalk = false;
		for (int i = 0; i < 3; i++) sign[i].isTalk = false; //����


		_data->player._isFind = false;
		//��� Ŭ����
		_data->player._openList.clear();
		_data->player._closeList.clear();
		_data->player._finalList.clear();
		_data->player.nodeNum = 0;

		_data->player.state = IDLE;
		_data->player.x = _data->tile[_data->player.index.x][_data->player.index.y].x;
		_data->player.y = _data->tile[_data->player.index.x][_data->player.index.y].y;
		_data->_totalNode[_data->player.index.x][_data->player.index.y]->nodeState = NODE_WALL;

		this->A(&_data->player);
		this->talk();
	}

	if (!ctrQ.skill_ready && !ctrW.skill_ready)
	{
		if (KEYMANAGER->isOnceKeyDown('Q'))
		{
			ctrQ.skill_ready = true;
			ctrW.skill_ready = false;

		}
		if (KEYMANAGER->isOnceKeyDown('W'))
		{
			ctrQ.skill_ready = false;
			ctrW.skill_ready = true;
		}
	}
	if (ctrQ.skill_ready || ctrW.skill_ready)
	{
		if (KEYMANAGER->isOnceKeyDown('Q'))
		{
			ctrQ.skill_ready = false;
			ctrW.skill_ready = false;
		}
		if (KEYMANAGER->isOnceKeyDown('W'))
		{
			ctrQ.skill_ready = false;
			ctrW.skill_ready = false;
		}
	}
}

//z���� üŷ �Լ�
void fieldScene::checkZorder(int i, int j, int dept)
{
	//���� ó��
	//��� ó��
	if (_data->player.y < _data->tile[i - 1][j - 1].y) _data->tile[i - 1][j - 1].z_order = true;
	if (_data->player.y < _data->tile[i - 1 + 1][j - 1].y) _data->tile[i - 1 + 1][j - 1].z_order = true;
	if (_data->player.y < _data->tile[i - 1][j - 1 + 1].y) _data->tile[i - 1][j - 1 + 1].z_order = true;
	//�ϴ� ó��
	if (_data->player.y < _data->tile[i + dept][j + dept].y) _data->tile[i + dept][j + dept].z_order = true;
	if (_data->player.y < _data->tile[i + dept + 1][j + dept].y) _data->tile[i + dept + 1][j + dept].z_order = true;
	if (_data->player.y < _data->tile[i + dept][j + dept + 1].y) _data->tile[i + dept][j + dept + 1].z_order = true;
	//�¿� ó��
	for (int p = 0; p < dept + 1; p++)
	{
		_data->tile[i + p + 1][j + p - 1].z_order = false;
		_data->tile[i + p - 1][j + p + 1].z_order = false;
	}

	//z���� ó��
	for (int p = 0; p < dept; p++)
	{
		if (_data->player.y < _data->tile[i + p][j + p].y) _data->tile[i + p][j + p].z_order = true;
		else _data->tile[i + p][j + p].z_order = false;
		if (_data->player.y < _data->tile[i + p + 1][j + p].y) _data->tile[i + p + 1][j + p].z_order = true;
		else _data->tile[i + p + 1][j + p].z_order = false;
		if (_data->player.y < _data->tile[i + p][j + p + 1].y) _data->tile[i + p][j + p + 1].z_order = true;
		else _data->tile[i + p][j + p + 1].z_order = false;
	}
	return;
}

//������ Ÿ�� ���� �Լ� PS_DOT
void fieldScene::drawDiamond(tagTile tile, int iStyle, COLORREF color)
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
void fieldScene::drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile)
{

	HPEN Pen, oldPen;
	Pen = CreatePen(iStyle, 1, color);
	oldPen = (HPEN)SelectObject(getMemDC(), Pen);

	//������ �׸���
	MoveToEx(getMemDC(), tile.x + (firstTile.x - _data->player.x) - (TILEWIDTH / 2), tile.y + (firstTile.y - _data->player.y), NULL);
	LineTo(getMemDC(), tile.x + (firstTile.x - _data->player.x), tile.y - (TILEHEIGHT / 2) + (firstTile.y - _data->player.y));
	LineTo(getMemDC(), tile.x + (firstTile.x - _data->player.x) + (TILEWIDTH / 2), tile.y + (firstTile.y - _data->player.y));
	LineTo(getMemDC(), tile.x + (firstTile.x - _data->player.x), tile.y + (TILEHEIGHT / 2) + (firstTile.y - _data->player.y));
	LineTo(getMemDC(), tile.x + (firstTile.x - _data->player.x) - (TILEWIDTH / 2), tile.y + (firstTile.y - _data->player.y));

	SelectObject(getMemDC(), oldPen);
	DeleteObject(Pen);
}

//�ش� Ÿ�� ��ǥ(�ε���) ǥ�� �Լ�
void fieldScene::drawTileIndex(tagTile tile, int i, int j)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12, tile.y - 8, str, strlen(str));
}
void fieldScene::drawTileIndex(tagTile tile, int i, int j, tagTile firstTile)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12 + (firstTile.x - _data->player.x), tile.y - 8 + (firstTile.y - _data->player.y), str, strlen(str));
}


//A*��� ó�� 
void fieldScene::A(tagCharacter* character)
{
	bool isStartFind = false;
	character->_isFind = false;
	//��� Ŭ����
	character->_openList.clear();
	character->_closeList.clear();
	character->_finalList.clear();
	character->nodeNum = 0;

	//���۳��� ���� ��� ����
	for (int i = x_for_start; i < x_for_end; i++)
	{
		for (int j = y_for_start; j < y_for_end; j++)
		{
			if (PICKING && _data->_totalNode[i][j]->nodeState != NODE_WALL)
			{
				isStartFind = true;
				_data->_totalNode[character->index.x][character->index.y]->nodeState = NODE_START;
				character->_startNode = _data->_totalNode[character->index.x][character->index.y];
				_data->_totalNode[i][j]->nodeState = NODE_END;
				character->_endNode = _data->_totalNode[i][j];
				character->temp_endNode = character->_endNode;
				character->state = RUN;
			}
		}
	}

	//��ã��
	if (isStartFind) this->pathFinding(character);
	if (character->_finalList.size() > 0) character->_finalList.push_back(character->temp_endNode);
}

//��� ��ä���(���������� �����Լ�)
void fieldScene::setNodeColor(tagTile tile, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	RECT fill = RectMakeCenter(tile.x, tile.y, TILEWIDTH / 2, TILEHEIGHT / 2);
	FillRect(getMemDC(), &fill, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), fill, RGB(0, 0, 0));
}

//��ã�� �Լ�
void fieldScene::pathFinding(tagCharacter* character)
{
	//���۳�� �� �����尡 ���� ��� ��ã�� ����
	if (!character->_startNode || !character->_endNode) return;

	//��ã�⸦ �غ���!!
	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��Ƶΰ� 
	//F�� H�� �������� ���� ã�Ƽ� �׳��� ������� �����ϰ�
	//���¸���Ʈ���� ������� �����ϰ� 
	//������� Ŭ�����Ʈ�� ��Ƶд�

	//1. ���۳�尡 �־�� ����� �����ϴ� 
	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
	character->_openList.push_back(character->_startNode);
	character->_finalList.push_back(character->_startNode);

	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ� 
	//�����忡 �����Ҷ����� ���ѷ���
	while (character->_openList.size() > 0)
	{
		character->_curNode = character->_openList[0];

		//���¸���Ʈ �� ���� F�� �۰ų� F�� ���ٸ� 
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ�����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵Ǵ� 1���� ��������
		for (int i = 1; i < character->_openList.size(); i++)
		{
			if (character->_openList[i]->F <= character->_curNode->F && character->_openList[i]->H < character->_curNode->H)
			{
				character->_curNode = character->_openList[i];
			}
		}

		//Ŭ�����Ʈ�� �־�д�
		for (int i = 0; i < character->_openList.size(); i++)
		{
			if (character->_curNode == character->_openList[i])
			{
				this->delOpenList(i, character);
				character->_closeList.push_back(character->_curNode);
			}
		}

		//�����尡 ���������� ����? (��ã�ҳ�?)
		if (character->_curNode == character->_endNode)
		{
			node* endNode = character->_endNode;
			vector<node*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
			while (endNode != character->_startNode)
			{
				tempNode.push_back(endNode);
				endNode = endNode->parentNode;
			}

			//STL����Ʈ�� �̿��ϸ� reverse()����ϰų�, push_front()�� ����� �� ������,
			//�츰 ���͸� �̿��ϴ� ���ٷ� Ǫ�ù����ش�
			//for (int i = tempNode.size() - 1; i > tempNode.size() - 3; i--) //2ĭ�� �̵�
			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				character->_finalList.push_back(tempNode[i]);
			}

			character->_isFind = true;
			//�����ϰ� �������´�
			return;
		}

		//�����¿� (������ ������� - ��¥�� �ָ� 4���� ��带 ��� ���¸���Ʈ�� �־ �˻��� ������)
		this->addOpenList(character->_curNode->index.x, character->_curNode->index.y - 1, character); //��
		this->addOpenList(character->_curNode->index.x, character->_curNode->index.y + 1, character); //��
		this->addOpenList(character->_curNode->index.x - 1, character->_curNode->index.y, character); //��
		this->addOpenList(character->_curNode->index.x + 1, character->_curNode->index.y, character); //��

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵȴ� ���

	}


}

//���¸���Ʈ�� �߰� �Լ�
void fieldScene::addOpenList(int idx, int idy, tagCharacter* character)
{
	//����ó��
	//���¸���Ʈ�� ���� ��尡 ��ü��� �ε��� ������ ������ �߰����� ���ϰ� ó���Ѵ�(�ε��� ������ 0 ~ 4����)
	if (idx < 0 || idx >= MAXTILESIZE_NORMAL || idy < 0 || idy >= MAXTILESIZE_NORMAL) return;
	//��ֹ��� ���¸���Ʈ�� ���� �� ����
	if (_data->_totalNode[idx][idy]->nodeState == NODE_WALL) return;
	//Ŭ�����Ʈ�� �ִٸ� �̹� �����±��̴� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < character->_closeList.size(); i++)
	{
		if (_data->_totalNode[idx][idy] == character->_closeList[i]) return;
	}

	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��ϱ�
	node* neighborNode = _data->_totalNode[idx][idy];
	int moveCost = character->_curNode->G + ((character->_curNode->index.x - idx == 0 || character->_curNode->index.y - idy == 0) ? 10 : 14);

	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
	for (int i = 0; i < character->_openList.size(); i++)
	{
		if (character->_openList[i] == neighborNode) return;
	}

	//���������� ���¸���Ʈ���� ���°�� 
	//G, H, ParentNode ���� �� ���¸���Ʈ�� �߰�
	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->index.x - character->_endNode->index.x) + abs(neighborNode->index.y - character->_endNode->index.y)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = character->_curNode;
	character->_openList.push_back(neighborNode);
}

//���¸���Ʈ ����
void fieldScene::delOpenList(int index, tagCharacter* character)
{
	character->_openList.erase(character->_openList.begin() + index);
}

//��� �ֺ� 4�� ��ĭ ã��
INDEX fieldScene::findEmptyIndex(tagCharacter* player, tagCharacter* enemy)
{
	//�÷��̾� �ֺ� �� ���� ����� ��ǥ ã��
	INDEX find;
	int judge;

	if (_data->_totalNode[player->index.x - 1][player->index.y]->nodeState != NODE_WALL)
	{
		find = { player->index.x - 1, player->index.y };
		judge = pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
	}
	else if (_data->_totalNode[player->index.x + 1][player->index.y]->nodeState != NODE_WALL)
	{
		find = { player->index.x + 1, player->index.y };
		judge = pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
	}
	else if (_data->_totalNode[player->index.x][player->index.y - 1]->nodeState != NODE_WALL)
	{
		find = { player->index.x, player->index.y - 1 };
		judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2);
	}
	else if (_data->_totalNode[player->index.x][player->index.y + 1]->nodeState != NODE_WALL)
	{
		find = { player->index.x, player->index.y + 1 };
		judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2);
	}
	else
	{
		return { -1, -1 };
	}

	if (_data->_totalNode[player->index.x - 1][player->index.y]->nodeState != NODE_WALL)
	{
		if (judge > pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2))
		{
			find = { player->index.x - 1, player->index.y };
			judge = pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
		}
	}

	if (_data->_totalNode[player->index.x + 1][player->index.y]->nodeState != NODE_WALL)
	{
		if (judge > pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2))
		{
			find = { player->index.x + 1, player->index.y };
			judge = pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
		}
	}

	if (_data->_totalNode[player->index.x][player->index.y - 1]->nodeState != NODE_WALL)
	{
		if (judge > pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2))
		{
			find = { player->index.x, player->index.y - 1 };
			judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2);
		}
	}

	if (_data->_totalNode[player->index.x][player->index.y + 1]->nodeState != NODE_WALL)
	{
		if (judge > pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2))
		{
			find = { player->index.x, player->index.y + 1 };
			judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2);
		}
	}

	return find;

}


//ĳ���� �̵�ó��
void fieldScene::move(tagCharacter* character)
{

	

	//��� ���� �̵��ϱ�
	if (character->_finalList.size() > 0)
	{
		if (character->_finalList.size() > character->nodeNum && character->_finalList.size() != character->nodeNum + 1)
		{
			_data->_totalNode[character->_finalList[character->nodeNum]->index.x][character->_finalList[character->nodeNum]->index.y]->nodeState = NODE_EMPTY;

			//���� ����
			if (character->_finalList[character->nodeNum]->index.x + 1 == character->_finalList[character->nodeNum + 1]->index.x &&
				character->_finalList[character->nodeNum]->index.y == character->_finalList[character->nodeNum + 1]->index.y)//����
			{
				//_data->_totalNode[character->_finalList[character->nodeNum + 1]->index.x][character->_finalList[character->nodeNum + 1]->index.y]->nodeState = NODE_WALL;
				character->direc_LR = LEFT;
				character->direc_DU = DOWN;
				character->state = RUN;
			}
			else if (character->_finalList[character->nodeNum]->index.x == character->_finalList[character->nodeNum + 1]->index.x &&
				character->_finalList[character->nodeNum]->index.y + 1 == character->_finalList[character->nodeNum + 1]->index.y)//����
			{
				//_data->_totalNode[character->_finalList[character->nodeNum + 1]->index.x][character->_finalList[character->nodeNum + 1]->index.y]->nodeState = NODE_WALL;
				character->direc_LR = RIGHT;
				character->direc_DU = DOWN;
				character->state = RUN;
			}
			else if (character->_finalList[character->nodeNum]->index.x == character->_finalList[character->nodeNum + 1]->index.x &&
				character->_finalList[character->nodeNum]->index.y - 1 == character->_finalList[character->nodeNum + 1]->index.y)//�»�
			{
				//_data->_totalNode[character->_finalList[character->nodeNum + 1]->index.x][character->_finalList[character->nodeNum + 1]->index.y]->nodeState = NODE_WALL;
				character->direc_LR = LEFT;
				character->direc_DU = UP;
				character->state = RUN;
			}
			else if (character->_finalList[character->nodeNum]->index.x - 1 == character->_finalList[character->nodeNum + 1]->index.x &&
				character->_finalList[character->nodeNum]->index.y == character->_finalList[character->nodeNum + 1]->index.y)//���
			{
				//_data->_totalNode[character->_finalList[character->nodeNum + 1]->index.x][character->_finalList[character->nodeNum + 1]->index.y]->nodeState = NODE_WALL;
				character->direc_LR = RIGHT;
				character->direc_DU = UP;
				character->state = RUN;
			}

		}
		//else if (character->_finalList.size() == character->nodeNum + 1 && character->state != ATTACK)
		else if (character->_finalList.size() == character->nodeNum + 1)
		{
			character->state = IDLE;
			character->nodeNum = 0;
			character->_isFind = false;
			//��� Ŭ����
			character->_openList.clear();
			character->_closeList.clear();
			character->_finalList.clear();
		}


		if (character->state == RUN)
		{
			//�̵� ó��
			switch (character->direc_DU)
			{
			case DOWN:
				character->y += character->runSpeed;
				switch (character->direc_LR)
				{
				case LEFT:
					character->x -= character->runSpeed * 2;
					break;
				case RIGHT:
					character->x += character->runSpeed * 2;
					break;
				}
				break;
			case UP:
				character->y -= character->runSpeed;
				switch (character->direc_LR)
				{
				case LEFT:
					character->x -= character->runSpeed * 2;
					break;
				case RIGHT:
					character->x += character->runSpeed * 2;
					break;
				}
				break;
			}


			//�ε��� ����
			for (int i = x_for_start; i < x_for_end; i++)
			{
				for (int j = y_for_start; j < y_for_end; j++)
				{
					if (character->x == _data->tile[i][j].x && character->y == _data->tile[i][j].y)
					{
						//state = IDLE;
						character->nodeNum++;
						character->index.x = i;
						character->index.y = j;
						_data->_totalNode[i][j]->nodeState = NODE_WALL;
					}
				}
			}

		}
	}
}

//��ȭ ���� npc ��ȭâ on/off ó��
void fieldScene::talk()
{
	//npc
	if ((-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
		!npc.isTalk) npc.isTalk = true;
	else if ((-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.left) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(-0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
		(0.5) * (_ptMouseC.x - _data->tile[npc.x][npc.y].rc.right) + _data->tile[npc.x][npc.y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
		npc.isTalk) npc.isTalk = false;

	//����
	for (int i = 0; i < 3; i++)
	{
		if ((-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
			!sign[i].isTalk) sign[i].isTalk = true;
		else if ((-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.left) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(-0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
			(0.5) * (_ptMouseC.x - _data->tile[sign[i].x][sign[i].y].rc.right) + _data->tile[sign[i].x][sign[i].y].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
			sign[i].isTalk) sign[i].isTalk = false;
	}
}
