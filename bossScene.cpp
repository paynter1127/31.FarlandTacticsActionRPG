#include "stdafx.h"
#include "bossScene.h"

HRESULT bossScene::init()
{
	SOUNDMANAGER->stop("bgm01");
	//�þ� �ʱ�ȭ
	img_bg = IMAGEMANAGER->findImage("st44a1");
	GDIPLUS->addImage("viewSight", "PNGIMAGE/viewSight.png");
	sightOn = true;

	//������ ����(+ BT AI Ŭ���� ����)
	_data = new receivedData1;




	//�÷��̾� ĳ���� �ʱ� Ÿ�� �ε���
	_data->player.init_pos = { 3, 15 };


	//�� Ÿ�� �ʱ�ȭ
	for (int i = 0; i < MAXTILESIZE_BOSS; i++)
	{
		for (int j = 0; j < MAXTILESIZE_BOSS; j++)
		{
			_data->tile[i][j].x = (WINSIZEX / 2) + (((_data->player.init_pos.x - _data->player.init_pos.y) / 2) * TILEWIDTH) - (i * 64 / 2) + (j * 64 / 2) - 32;
			_data->tile[i][j].y = (WINSIZEY / 2) - (_data->player.init_pos.y * TILEHEIGHT) - (((_data->player.init_pos.x - _data->player.init_pos.y) / 2) * TILEHEIGHT) + (i * 32 / 2) + (j * 32 / 2) + 16;
			_data->tile[i][j].rc = RectMakeCenter(_data->tile[i][j].x, _data->tile[i][j].y, 64, 32);
			sprintf(tile_strKey[i][j].str_img, "%s", "���̹���");
			tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
			tile_strKey[i][j].isImg = true;

			tile_strKey[i][j].state_obj = NONE;
			tile_strKey[i][j].floor = 0;

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
	for (int i = 0; i < MAXTILESIZE_BOSS; i++)
	{
		for (int j = 0; j < MAXTILESIZE_BOSS; j++)
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
		file = CreateFile("map_boss.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE_BOSS * MAXTILESIZE_BOSS, &read, NULL);

		//�� Ÿ�� �ε�0
		for (int i = 0; i < MAXTILESIZE_BOSS; i++)
		{
			for (int j = 0; j < MAXTILESIZE_BOSS; j++)
			{

				tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
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
	_data->_totalNode[3][13]->nodeState = NODE_EMPTY;


	//�븻 ���ʹ� �ʱ�ȭ
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
	}

	
	//���� ���ʹ� �ʱ�ȭ
	{
		//����  ü�¹� ���̽� Ŭ���� �ʱ�ȭ
		_boss_hpBar = new progressBar;
		_boss_hpBar->init("IMAGE/ui/boss_hpBar_3.1496.128", "IMAGE/ui/boss_hpBar_back.1496.128", 300, 300, 64, 12);

		//���� ���̽� �ʱ�ȭ
		_boss = new tagCharacter;
		
		_boss->state = IDLE;
		_boss->direc_LR = LEFT;
		_boss->direc_DU = DOWN;
		_boss->runSpeed = 0.5f;

		_boss->stat.hp = 500;
		_boss->stat.hpMax = _boss->stat.hp;
		_boss->stat.att = 20;

		_boss->init_pos = { 16, 17 };

		_boss->index.x = _boss->init_pos.x;
		_boss->index.y = _boss->init_pos.y;
		_boss->x = _data->tile[_boss->index.x][_boss->index.y].x;
		_boss->y = _data->tile[_boss->index.x][_boss->index.y].y;
		_boss->rc = RectMakeCenter(_boss->x, _boss->y, 3, 3);
		_boss->count.base = RND->range(80);
		
		_data->_totalNode[_boss->init_pos.x][_boss->init_pos.y]->nodeState = NODE_WALL;
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


	//���ʹ� AI ����
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_root[i] = new Sequence;
		_sequence1[i] = new Sequence;
		_selector1[i] = new Selector;
		//_enemy[i] = new tagCharacter;
		_approach[i] = new Approach(_data, &_vEnemy[i]);
		_attack[i] = new Attack(_data, &_vEnemy[i]);


		//BT AI ���� ����
		_root[i]->addChild(_sequence1[i]);
		_sequence1[i]->addChild(_approach[i]);
		_sequence1[i]->addChild(_attack[i]);
	}

	//���� AI ����
	_root_boss = new Sequence;
	_selector1_boss = new Selector;
	_isDie_boss = new isDie(_boss);
	_selector2_boss = new Selector;
	
	_sequence100_boss = new Sequence; //1������ AI
	_checkPhase = new checkPhase(_boss);
	_sequence101_boss = new Sequence;
	_checkRange = new checkRange(_data, _boss);
	_skill_fire = new skill_fire(_data, _boss);
	_skill_thunder = new skill_thunder(_data, _boss);
	_skill_summon = new skill_summon(_data, _boss);

	_sequence200_boss = new Sequence; //2������ AI (1������ �������� üũ ����� false �̸� �ٷ� 2������ ������ �����ϰ� ����)
	_sequence201_boss = new Sequence;


	_approach_boss = new Approach(_data, _boss);
	_attack_boss = new Attack(_data, _boss);
	
	//BT AI ���� ����
	//_root_boss->addChild(_sequence1_boss);
	//_sequence1_boss->addChild(_approach_boss);
	//_sequence1_boss->addChild(_attack_boss);
	_root_boss->addChild(_selector1_boss);
	_selector1_boss->addChild(_isDie_boss);
	_selector1_boss->addChild(_selector2_boss);
	
	_selector2_boss->addChild(_sequence100_boss);
	_sequence100_boss->addChild(_checkPhase);
	_sequence100_boss->addChild(_sequence101_boss);
	_sequence100_boss->addChild(_approach_boss);

	_sequence101_boss->addChild(_checkRange);
	_sequence101_boss->addChild(_skill_thunder);
	_sequence101_boss->addChild(_skill_fire);
	//_sequence100_boss->addChild(_approach_boss);
	//_sequence100_boss->addChild(_attack_boss);

	_selector2_boss->addChild(_sequence200_boss);
	_sequence200_boss->addChild(_sequence201_boss);
	_sequence200_boss->addChild(_approach_boss);

	_sequence201_boss->addChild(_checkRange);
	_sequence201_boss->addChild(_skill_summon);
	_sequence201_boss->addChild(_skill_thunder);
	_sequence201_boss->addChild(_skill_fire);
	//_sequence200_boss->addChild(_attack_boss);

	for (int i = 0; i < 7; i++) isThunder[i] = false;
	for (int i = 0; i < 5; i++) isFire[i] = false;
	isSummon = false;

	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void bossScene::release()
{
	//�ε��� Ŭ���� ����
	hpBar->release();
	SAFE_DELETE(hpBar);
	_enemy_hpBar->release();
	SAFE_DELETE(_enemy_hpBar);
	SAFE_DELETE(_enemy);

}

void bossScene::update()
{
	world_count++;
	//AI���� Ŭ���� ��Ʈ
	_data->range = RectMakeCenter(_data->player.x, _data->player.y, WINSIZEX - 32, WINSIZEY - 32);

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
			_vEnemy_hpBar.erase(_vEnemy_hpBar.begin() + p);
			p--;
		}
	}

	//���ʹ� ü�¹� ������Ʈ +  rc ������Ʈ
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

	//���� AI üũ
	_boss->count.base++;
	if (_boss->_finalList.size() > 0) this->move(_boss); //move
	if (_boss->count.base % 80 == 0) _root_boss->run(); //BT AI

	//���� ü�¹� ������Ʈ
	_boss_hpBar->update();
	_boss_hpBar->setGauge(_boss->stat.hp, _boss->stat.hpMax);
	//���� rc ������Ʈ
	_boss->rc = RectMakeCenter(_boss->x, _boss->y, 3, 3);


	//for�� üũ ����
	{
		//ȭ�� ���� ���� - ���� 10, ���� 14
		x_for_start = _data->player.index.x - 11;
		if (x_for_start < 0) x_for_start = 0;
		x_for_end = _data->player.index.x + 11;
		if (x_for_end > MAXTILESIZE_BOSS) x_for_end = MAXTILESIZE_BOSS;
		y_for_start = _data->player.index.y - 11;
		if (y_for_start < 0) y_for_start = 0;
		y_for_end = _data->player.index.y + 11;
		if (y_for_end > MAXTILESIZE_BOSS) y_for_end = MAXTILESIZE_BOSS;

		//z-order üũ ���� - ���� 4, ���� 4
		x_for_start_z = _data->player.index.x - 5;
		if (x_for_start_z < 0) x_for_start_z = 0;
		x_for_end_z = _data->player.index.x + 5;
		if (x_for_end_z > MAXTILESIZE_BOSS) x_for_end_z = MAXTILESIZE_BOSS;
		y_for_start_z = _data->player.index.y - 5;
		if (y_for_start_z < 0) y_for_start_z = 0;
		y_for_end_z = _data->player.index.y + 5;
		if (y_for_end_z > MAXTILESIZE_BOSS) y_for_end_z = MAXTILESIZE_BOSS;
	}


	//���� ��Ŭ�� ��Ʈ�� ��(NPC ���ó��, �̵�, A*)
	this->userSkill();
	//�߰� A* ������Ʈ
	this->move(&_data->player);




	//�÷��̾� ���� z���� ó��
	this->checkZorder(_data->player.index.x, _data->player.index.y, 6);

	

	//�ʵ������ �̵�
	if (_data->player.index.x == 3 && _data->player.index.y == 13)
	{
		_data->player._isFind = false;
		//��� Ŭ����
		_data->player._openList.clear();
		_data->player._closeList.clear();
		_data->player._finalList.clear();
		_data->player.nodeNum = 0;
		SCENEMANAGER->loadScene("�Ϲ��ʵ�");
	}



	//ĳ������ RC
	_data->player.rc = RectMakeCenter(_data->player.x, _data->player.y, 3, 3);
	rc_clipping = RectMakeCenter(_data->player.x, _data->player.y, WINSIZEX + 8, WINSIZEY + 8);
	//ķ ���� ���콺 ��ǥ ������Ʈ
	_ptMouseC = {
		_ptMouse.x - (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].x - (LONG)_data->player.x),
		_ptMouse.y - (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].y - (LONG)_data->player.y) };
}

void bossScene::render()
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

		//��� ����  1216, 816
		img_bg->render(getMemDC(), -706 + XX, -96 + YY);
		
		

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




	//�÷��̾� ĳ���� ����
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

	//���� 2������ �̹��� ó��(�� ������)
	if (_boss->stat.hp < _boss->stat.hpMax / 2)
	{
		IMAGEMANAGER->findImage("fire")->scaleFrameRender(getMemDC(), _boss->x - 68 + XX, _boss->y - 120 + YY, IMAGEMANAGER->findImage("fire")->getFrameX(), IMAGEMANAGER->findImage("fire")->getFrameY(), 0.7f);
	}

	//���� ����
	switch (_boss->state)
	{
	case IDLE:
		if (_boss->count.base % 16 == 0)
		{
			if (_boss->count.idle == 2) _boss->count.idle = (-1);
			_boss->count.idle++;
		}
		IMAGEMANAGER->findImage("boss_idle")->frameRender(getMemDC(), _boss->rc.left - 256 / 6 + XX, _boss->rc.top - 80 + YY, _boss->count.idle, 0);
		//TextOut(getMemDC(), _boss->rc.right + 20 + XX, _boss->rc.top + YY, "IDLE", strlen("IDLE"));
		break;
	case BEHIT:
		IMAGEMANAGER->findImage("zombie_beHit")->render(getMemDC(), _boss->rc.left - 32 + XX, _boss->rc.top - 96 + 8 + YY);
		//TextOut(getMemDC(), _boss->rc.right + 20 + XX, _boss->rc.top + YY, "BEHIT", strlen("BEHIT"));
		break;
	case RUN:
		switch (_boss->direc_DU)
		{
		case DOWN:
			switch (_boss->direc_LR)
			{
			case LEFT:
				if (_boss->count.base % 8 == 0)
				{
					if (_boss->count.run1 == 3) _boss->count.run1 = (-1);
					_boss->count.run1++;
				}
				IMAGEMANAGER->findImage("boss_run1")->frameRender(getMemDC(), _boss->rc.left - 384 / 8 + XX, _boss->rc.top - 80 + YY, _boss->count.run1, 0);
				break;
			case RIGHT:
				if (_boss->count.base % 8 == 0)
				{
					if (_boss->count.run2 == 3) _boss->count.run2 = (-1);
					_boss->count.run2++;
				}
				IMAGEMANAGER->findImage("boss_run2")->frameRender(getMemDC(), _boss->rc.left - 384 / 8 + XX, _boss->rc.top - 80 + YY, _boss->count.run2, 0);
				break;
			}
			break;
		case UP:
			switch (_boss->direc_LR)
			{
			case LEFT:
				if (_boss->count.base % 8 == 0)
				{
					if (_boss->count.run3 == 3) _boss->count.run3 = (-1);
					_boss->count.run3++;
				}
				IMAGEMANAGER->findImage("boss_run3")->frameRender(getMemDC(), _boss->rc.left - 384 / 8 + XX, _boss->rc.top - 80 + YY, _boss->count.run3, 0);
				break;
			case RIGHT:
				if (_boss->count.base % 8 == 0)
				{
					if (_boss->count.run4 == 3) _boss->count.run4 = (-1);
					_boss->count.run4++;
				}
				IMAGEMANAGER->findImage("boss_run4")->frameRender(getMemDC(), _boss->rc.left - 384 / 8 + XX, _boss->rc.top - 80 + YY, _boss->count.run4, 0);
				break;
			}
			break;
		}
		//TextOut(getMemDC(), _boss->rc.right + 20 + XX, _boss->rc.top + YY, "RUN", strlen("RUN"));
		break;
	case SKILL1: //�� ��ų
		if (world_count % 4 == 0)
		{
			if (IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY() == 18)
			{
				IMAGEMANAGER->findImage("boss_magicCircle_small")->setFrameY(-1);
			}
			IMAGEMANAGER->findImage("boss_magicCircle_small")->setFrameY(IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY() + 1);
		}
		IMAGEMANAGER->findImage("boss_magicCircle_small")->frameRender(getMemDC(),
			_data->tile[_boss->index.x][_boss->index.y].x - 40 + XX,
			_data->tile[_boss->index.x][_boss->index.y].y - 24 + YY,
			0, IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY());

		if (_boss->count.base % 8 == 0)
		{
			
			if (_boss->count.attack4 == 6)
			{
				_boss->count.attack4 = 0;
				SOUNDMANAGER->play("meteo3");
				isFire[0] = true;
			}
			if (_boss->count.attack1 == 2)
			{
				_boss->count.attack1 = (-1);
				_boss->count.attack4++;
			}
			_boss->count.attack1++;

			if (_boss->count.attack4 == 1)
			{
				isFire[4] = true;
				fireIndex = { _data->player.index.x, _data->player.index.y };
			}
		}
		IMAGEMANAGER->findImage("boss_attack1")->frameRender(getMemDC(), _boss->rc.left - 384 / 6 + XX, _boss->rc.top - 112 + YY, _boss->count.attack1, 0);
		break;
	case SKILL2: //���� ��ų
		if (world_count % 4 == 0)
		{
			if (IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY() == 18)
			{
				IMAGEMANAGER->findImage("boss_magicCircle_small")->setFrameY(-1);
			}
			IMAGEMANAGER->findImage("boss_magicCircle_small")->setFrameY(IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY() + 1);
		}
		IMAGEMANAGER->findImage("boss_magicCircle_small")->frameRender(getMemDC(),
			_data->tile[_boss->index.x][_boss->index.y].x - 40 + XX,
			_data->tile[_boss->index.x][_boss->index.y].y - 24 + YY,
			0, IMAGEMANAGER->findImage("boss_magicCircle_small")->getFrameY());
		
		if (_boss->count.base % 8 == 0)
		{
			if (_boss->count.attack4 == 4)
			{
				_boss->count.attack4 = 0;
				isThunder[0] = true;
				SOUNDMANAGER->play("magic04", 0.4f);
				thunderIndex = { _data->player.index.x, _data->player.index.y };
			}
			if (_boss->count.attack2 == 2)
			{
				_boss->count.attack2 = (-1);
				_boss->count.attack4++;
			}
			_boss->count.attack2++;
		}
		IMAGEMANAGER->findImage("boss_attack2")->frameRender(getMemDC(), _boss->rc.left - 384 / 6 + XX, _boss->rc.top - 112 + YY, _boss->count.attack2, 0);
		break;
	case SKILL3: //��ȯ ��ų(���� �ֺ����� ��ȯ)
		
		if (world_count % 16 == 0)
		{
			if (IMAGEMANAGER->findImage("boss_magicCircle_big")->getFrameY() == 4)
			{
				//IMAGEMANAGER->findImage("boss_magicCircle_big")->setFrameY(-1);
				
				isSummon = true;

			}
			if (!isSummon) IMAGEMANAGER->findImage("boss_magicCircle_big")->setFrameY(IMAGEMANAGER->findImage("boss_magicCircle_big")->getFrameY() + 1);
			if (isSummon)
			{
				IMAGEMANAGER->findImage("boss_magicCircle_big")->setFrameY(IMAGEMANAGER->findImage("boss_magicCircle_big")->getFrameY() - 1);

				if (IMAGEMANAGER->findImage("boss_magicCircle_big")->getFrameY() == 0) //��ȯ �� �ʱ�ȭ
				{
					IMAGEMANAGER->findImage("boss_magicCircle_big")->setFrameY(-1);
					isSummon = false;

					int enemyNum = _vEnemy.size();

					//���� �ֱ�(ü�¹�, ĳ����)
					for (int p = 0; p < 3; p++)
					{
						_vEnemy_hpBar.push_back(*_enemy_hpBar);
						_vEnemy.push_back(*_enemy);
					}

					//���� ���ʹ� �ʱ�ȭ
					for (int p = enemyNum; p < _vEnemy.size(); p++)
					{
						while (true)
						{
							//int i = RND->range(_data->player.index.x - 4, _data->player.index.x + 5);
							//int j = RND->range(_data->player.index.x - 4, _data->player.index.x + 5);
							int i = RND->range(_boss->index.x - 4, _boss->index.x + 5);
							int j = RND->range(_boss->index.x - 4, _boss->index.x + 5);
							if (_data->_totalNode[i][j]->nodeState == NODE_WALL) continue;
							else
							{
								_vEnemy[p].init_pos = { i, j };
								break;
							}
						}
					}
					
					for (int p = enemyNum; p < _vEnemy.size(); p++)
					{
						_vEnemy[p].index.x = _vEnemy[p].init_pos.x;
						_vEnemy[p].index.y = _vEnemy[p].init_pos.y;
						_vEnemy[p].x = _data->tile[_vEnemy[p].index.x][_vEnemy[p].index.y].x;
						_vEnemy[p].y = _data->tile[_vEnemy[p].index.x][_vEnemy[p].index.y].y;
						_vEnemy[p].rc = RectMakeCenter(_vEnemy[p].x, _vEnemy[p].y, 3, 3);
						_vEnemy[p].count.base = RND->range(80);
						_data->_totalNode[_vEnemy[p].init_pos.x][_vEnemy[p].init_pos.y]->nodeState = NODE_WALL;
					}

					//���ʹ� AI ����
					for (int i = enemyNum; i < _vEnemy.size(); i++)
					{
						_root[i] = new Sequence;
						_sequence1[i] = new Sequence;
						_selector1[i] = new Selector;
						//_enemy[i] = new tagCharacter;
						_approach[i] = new Approach(_data, &_vEnemy[i]);
						_attack[i] = new Attack(_data, &_vEnemy[i]);


						//BT AI ���� ����
						_root[i]->addChild(_sequence1[i]);
						_sequence1[i]->addChild(_approach[i]);
						_sequence1[i]->addChild(_attack[i]);
					}
				}
			}
		}
		IMAGEMANAGER->findImage("boss_magicCircle_big")->frameRender(getMemDC(),
			_data->tile[_boss->index.x][_boss->index.y].x - 128 + XX,
			_data->tile[_boss->index.x][_boss->index.y].y - 72 + YY, 
			0, IMAGEMANAGER->findImage("boss_magicCircle_big")->getFrameY());

		if (_boss->count.base % 8 == 0)
		{
			if (_boss->count.attack3 == 2)
			{
				_boss->count.attack3 = (-1);
				_boss->count.attack4++;
			}
			_boss->count.attack3++;
		}
		IMAGEMANAGER->findImage("boss_attack3")->frameRender(getMemDC(), _boss->rc.left - 384 / 6 + XX, _boss->rc.top - 112 + YY, _boss->count.attack3, 0);
		break;
	}


	//���ʹ� ü�¹� ����
	for (int p = 0; p < _vEnemy.size(); p++)
	{
		_vEnemy_hpBar[p].setX(_vEnemy[p].rc.left - 20 + XX);
		_vEnemy_hpBar[p].setY(_vEnemy[p].rc.top - 70 + YY);
		_vEnemy_hpBar[p].render();
	}
	
	//���� ü�¹� ����
	_boss_hpBar->setX(_boss->rc.left - 32 + XX);
	_boss_hpBar->setY(_boss->rc.top - 90 + YY);
	_boss_hpBar->render();

	


	







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



	//���ʹ� ü�� Ȯ�� �����
	//for (int p = 0; p < _vEnemy.size(); p++) textOutDbug(getMemDC(), 100, 20 * (1 + p) + 80, _vEnemy[p].stat.hp);
	

	//��ȭ�� ���� ó��
	if (_data->tile[18][9].x < _data->player.x && _data->player.x < _data->tile[9][16].x && _data->tile[18][9].y > _data->player.y) 
		IMAGEMANAGER->findImage("frameMound")->alphaRender(getMemDC(), -706 + 352 + XX, -96 + 176 + YY, 150); //352, 176
	else IMAGEMANAGER->findImage("frameMound")->render(getMemDC(), -706 + 352 + XX, -96 + 176 + YY); //352, 176
	
	//��ȭ �� ó��
	if (world_count % 1 == 0)
	{
		if (IMAGEMANAGER->findImage("fire")->getFrameX() == 9)
		{
			if (IMAGEMANAGER->findImage("fire")->getFrameY() == 5)
			{
				IMAGEMANAGER->findImage("fire")->setFrameY(-1);
			}
			IMAGEMANAGER->findImage("fire")->setFrameY(IMAGEMANAGER->findImage("fire")->getFrameY() + 1);
			IMAGEMANAGER->findImage("fire")->setFrameX(-1);
		}
		IMAGEMANAGER->findImage("fire")->setFrameX(IMAGEMANAGER->findImage("fire")->getFrameX() + 1);
	}
	IMAGEMANAGER->findImage("fire")->frameRender(getMemDC(), -200 + XX, -50 + YY, IMAGEMANAGER->findImage("fire")->getFrameX(), IMAGEMANAGER->findImage("fire")->getFrameY());




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
			if (0 <= ctrQ.count && ctrQ.count < 6) IMAGEMANAGER->findImage("skill_meteo")->frameRender(getMemDC(), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.left + XX - 8 + 32 * (5 - ctrQ.count), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.top - 172 - 32 * (5 - ctrQ.count) + YY, ctrQ.count, 0);
			else if (6 <= ctrQ.count  && ctrQ.count < 32)
			{
				IMAGEMANAGER->findImage("skill_meteo")->frameRender(getMemDC(), _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.left + XX - 64, _data->tile[ctrQ.markPoint.x][ctrQ.markPoint.y].rc.top - 128 + YY, ctrQ.count, 0);
				if (!SOUNDMANAGER->isPlaySound("meteo2")) SOUNDMANAGER->play("meteo2");
				if (!SOUNDMANAGER->isPlaySound("meteo3")) SOUNDMANAGER->play("meteo3");
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
					if (ctrQ.markPoint.x - ctrQ.i_range <= _boss->index.x && _boss->index.x <= ctrQ.markPoint.x + ctrQ.i_range &&
						ctrQ.markPoint.y - ctrQ.j_range <= _boss->index.y && _boss->index.y <= ctrQ.markPoint.y + ctrQ.j_range)
					{
						_boss->stat.hp -= ctrQ.damage;
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
				if (ctrW.markPoint.x - ctrW.i_range <= _boss->index.x && _boss->index.x <= ctrW.markPoint.x + ctrW.i_range &&
					ctrW.markPoint.y - ctrW.j_range <= _boss->index.y && _boss->index.y <= ctrW.markPoint.y + ctrW.j_range)
				{
					_boss->stat.hp -= ctrW.damage;
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



	//���� ��ų ��� + ������ ó��
	{
		

		//���� ��ų
		if (isFire[4]) //���� ���� �˸�
		{
			if ((world_count / 8) % 2 == 0)
			{
				for (int i = fireIndex.x - 3; i < fireIndex.x + 4; i++)
				{
					for (int j = fireIndex.y - 3; j < fireIndex.y + 4; j++)
					{
						IMAGEMANAGER->findImage("skill_hitRange")->render(getMemDC(), _data->tile[i][j].rc.left + XX, _data->tile[i][j].rc.top + YY);
					}
				}
			}
		}

		if (isFire[0]) //���� ����
		{
			if (world_count % 1 == 0)
			{
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 3)
				{
					sightOn = false;
					shackX = RND->range(-3, 3) * 8;
					shackY = RND->range(-3, 3) * 8;
				}
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 6)
				{
					sightOn = true;
					shackX = 0;
					shackY = 0;


					if (fireIndex.x == _data->player.index.x && fireIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 40;
				}

				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 13)
				{
					IMAGEMANAGER->findImage("boss_fire")->setFrameX(-1);
					_boss->state = IDLE;
					isFire[4] = false;
					isFire[0] = false;
					isFire[1] = true;
					SOUNDMANAGER->play("meteo3");
				}
				IMAGEMANAGER->findImage("boss_fire")->setFrameX(IMAGEMANAGER->findImage("boss_fire")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_fire")->frameRender(getMemDC(),
				_data->tile[fireIndex.x][fireIndex.y].x - 22 + XX,
				_data->tile[fireIndex.x][fireIndex.y].y - 112 + YY,
				IMAGEMANAGER->findImage("boss_fire")->getFrameX(), 0);
		}
		if (isFire[1]) //���� ����
		{
			if (world_count % 1 == 0)
			{
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 3)
				{
					sightOn = false;
					shackX = RND->range(-3, 3) * 8;
					shackY = RND->range(-3, 3) * 8;
				}
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 6)
				{
					sightOn = true;
					shackX = 0;
					shackY = 0;

					if (fireIndex.x == _data->player.index.x && fireIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 40;
				}

				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 13)
				{
					IMAGEMANAGER->findImage("boss_fire")->setFrameX(-1);
					isFire[1] = false;
					isFire[2] = true;
					SOUNDMANAGER->play("meteo3");
				}
				IMAGEMANAGER->findImage("boss_fire")->setFrameX(IMAGEMANAGER->findImage("boss_fire")->getFrameX() + 1);
			}
			for (int i = fireIndex.x - 1; i < fireIndex.x + 2; i++)
			{
				for (int j = fireIndex.y - 1; j < fireIndex.y + 2; j++)
				{
					IMAGEMANAGER->findImage("boss_fire")->frameRender(getMemDC(),
						_data->tile[i][j].x - 22 + XX,
						_data->tile[i][j].y - 112 + YY,
						IMAGEMANAGER->findImage("boss_fire")->getFrameX(), 0);
				}
			}
			
		}
		if (isFire[2]) //���� ����
		{
			if (world_count % 1 == 0)
			{
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 3)
				{
					shackX = RND->range(-3, 3) * 8;
					shackY = RND->range(-3, 3) * 8;
				}
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 6)
				{
					sightOn = true;
					shackX = 0;
					shackY = 0;

					if (fireIndex.x == _data->player.index.x && fireIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 40;
				}

				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 13)
				{
					IMAGEMANAGER->findImage("boss_fire")->setFrameX(-1);
					isFire[2] = false;
					isFire[3] = true;
					SOUNDMANAGER->play("meteo3");
				}
				IMAGEMANAGER->findImage("boss_fire")->setFrameX(IMAGEMANAGER->findImage("boss_fire")->getFrameX() + 1);
			}
			for (int i = fireIndex.x - 2; i < fireIndex.x + 3; i++)
			{
				for (int j = fireIndex.y - 2; j < fireIndex.y + 3; j++)
				{
					IMAGEMANAGER->findImage("boss_fire")->frameRender(getMemDC(),
						_data->tile[i][j].x - 22 + XX,
						_data->tile[i][j].y - 112 + YY,
						IMAGEMANAGER->findImage("boss_fire")->getFrameX(), 0);
				}
			}
		}
		if (isFire[3]) //���� ����
		{
			if (world_count % 1 == 0)
			{
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 3)
				{
					shackX = RND->range(-3, 3) * 8;
					shackY = RND->range(-3, 3) * 8;
				}
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 6)
				{
					sightOn = true;
					shackX = 0;
					shackY = 0;

					if (fireIndex.x == _data->player.index.x && fireIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 40;

				}
				if (IMAGEMANAGER->findImage("boss_fire")->getFrameX() == 13)
				{
					IMAGEMANAGER->findImage("boss_fire")->setFrameX(-1);
					isFire[3] = false;
				}
				IMAGEMANAGER->findImage("boss_fire")->setFrameX(IMAGEMANAGER->findImage("boss_fire")->getFrameX() + 1);
			}
			for (int i = fireIndex.x - 3; i < fireIndex.x + 4; i++)
			{
				for (int j = fireIndex.y - 3; j < fireIndex.y + 4; j++)
				{
					IMAGEMANAGER->findImage("boss_fire")->frameRender(getMemDC(),
						_data->tile[i][j].x - 22 + XX,
						_data->tile[i][j].y - 112 + YY,
						IMAGEMANAGER->findImage("boss_fire")->getFrameX(), 0);
				}
			}
		}



	//���� ���� ��ų ��� + ������ ó��
		if (isThunder[0])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(-1);
					isThunder[0] = false;
					isThunder[1] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder1")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder1")->getFrameX(), 0);
		}
		if (isThunder[1])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(-1);
					isThunder[1] = false;
					isThunder[2] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder2")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder2")->getFrameX(), 0);
		}
		if (isThunder[2])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(-1);
					isThunder[2] = false;
					isThunder[3] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder1")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder1")->getFrameX(), 0);
		}
		if (isThunder[3])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(-1);
					isThunder[3] = false;
					isThunder[4] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder2")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder2")->getFrameX(), 0);
		}
		if (isThunder[4])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(-1);
					isThunder[4] = false;
					isThunder[5] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder1")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder1")->getFrameX(), 0);
		}
		if (isThunder[5])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 4)
				{
					sightOn = true;
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(-1);
					isThunder[5] = false;
					isThunder[6] = true;
					SOUNDMANAGER->play("magic04", 0.4f);
					thunderIndex = { _data->player.index.x, _data->player.index.y };
				}
				IMAGEMANAGER->findImage("boss_thunder2")->setFrameX(IMAGEMANAGER->findImage("boss_thunder2")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder2")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder2")->getFrameX(), 0);
		}
		if (isThunder[6])
		{
			if (world_count % 4 == 0)
			{
				sightOn = false;
				shackY = RND->range(-3, 3) * 4;
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 4)
				{
					
					shackY = 0;

					if (thunderIndex.x == _data->player.index.x && thunderIndex.y == _data->player.index.y)
						_data->player.stat.hp -= 8;
				}
				if (IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() == 5)
				{
					IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(-1);
					isThunder[6] = false;
					sightOn = true;
					_boss->state = IDLE;
				}
				IMAGEMANAGER->findImage("boss_thunder1")->setFrameX(IMAGEMANAGER->findImage("boss_thunder1")->getFrameX() + 1);
			}
			IMAGEMANAGER->findImage("boss_thunder1")->frameRender(getMemDC(),
				_data->tile[thunderIndex.x][thunderIndex.y].x - 32 + XX,
				_data->tile[thunderIndex.x][thunderIndex.y].y - 368 + YY,
				IMAGEMANAGER->findImage("boss_thunder1")->getFrameX(), 0);
		}
	}




	
	//textOutDbug(getMemDC(), 20, 40, (abs(_data->player.index.x - _boss->index.x) + abs(_data->player.index.y - _boss->index.y)));
	//textOutDbug(getMemDC(), 20, 60, _boss->state);


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
		textOutFontNum(getMemDC(), 200, 434, _data->player.stat.hp, 16, RGB(200, 200, 100));
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


	

	//textOutDbug(getMemDC(), _data->player.rc.right, _data->player.rc.top, _data->player.x);
	//textOutDbug(getMemDC(), _data->player.rc.right, _data->player.rc.top + 20, _data->player.y);
	//textOutDbug(getMemDC(), _data->player.rc.right + XX, _data->player.rc.top + YY, _data->player.stat.hp);

	SetBkMode(getMemDC(), 2);

	//TextOut(getMemDC(), WINSIZEX / 2, 0, "�����ʵ�", strlen("�����ʵ�"));
	fade(getMemDC(), _startTime, true);
}




//���� ��ų
void bossScene::userSkill()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		

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
void bossScene::checkZorder(int i, int j, int dept)
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
void bossScene::drawDiamond(tagTile tile, int iStyle, COLORREF color)
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
void bossScene::drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile)
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
void bossScene::drawTileIndex(tagTile tile, int i, int j)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12, tile.y - 8, str, strlen(str));
}
void bossScene::drawTileIndex(tagTile tile, int i, int j, tagTile firstTile)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12 + (firstTile.x - _data->player.x), tile.y - 8 + (firstTile.y - _data->player.y), str, strlen(str));
}


//A*��� ó�� 
void bossScene::A(tagCharacter* character)
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
void bossScene::setNodeColor(tagTile tile, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	RECT fill = RectMakeCenter(tile.x, tile.y, TILEWIDTH / 2, TILEHEIGHT / 2);
	FillRect(getMemDC(), &fill, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), fill, RGB(0, 0, 0));
}

//��ã�� �Լ�
void bossScene::pathFinding(tagCharacter* character)
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
void bossScene::addOpenList(int idx, int idy, tagCharacter* character)
{
	//����ó��
	//���¸���Ʈ�� ���� ��尡 ��ü��� �ε��� ������ ������ �߰����� ���ϰ� ó���Ѵ�(�ε��� ������ 0 ~ 4����)
	if (idx < 0 || idx >= MAXTILESIZE_BOSS || idy < 0 || idy >= MAXTILESIZE_BOSS) return;
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
void bossScene::delOpenList(int index, tagCharacter* character)
{
	character->_openList.erase(character->_openList.begin() + index);
}

//��� �ֺ� 4�� ��ĭ ã��
INDEX bossScene::findEmptyIndex(tagCharacter* player, tagCharacter* enemy)
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
void bossScene::move(tagCharacter* character)
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

