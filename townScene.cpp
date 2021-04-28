#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	SOUNDMANAGER->stop("bgm01");
	//맵 타일 배경
	img_bg = IMAGEMANAGER->findImage("st02a");

	//시야 초기화
	GDIPLUS->addImage("viewSight", "PNGIMAGE/viewSight.png");

	init_pos = { 17, 1 };	//캐릭터 초기 타일 인덱스
	tileSize = MAXTILESIZE_TOWN;	//현재 씬 타일 사이즈

	//타일 렉트 종속 좌표
	pt_tileMark = { (WINSIZEX / 2) + (((init_pos.x - init_pos.y) / 2) * TILEWIDTH), WINSIZEY / 2 - init_pos.y * TILEHEIGHT - (((init_pos.x - init_pos.y) / 2) * TILEHEIGHT) };

	//클리핑 렉트 초기화
	rc_clipping = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 540, 400);

	//맵 타일 초기화
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

			sprintf(tile_strKey[i][j].str_img_obj, "%s", "빈이미지");
			tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //초기 타일(빈 타일)
			sprintf(tile_strKey[i][j].str_img_enemy, "%s", "빈이미지");
			tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //초기 타일(빈 타일)

			tile_strKey[i][j].isImg_obj = false;
			tile_strKey[i][j].isImg_enemy = false;

			tile[i][j].z_order = false;
			tile[i][j].isFloodFill = false;
		}
	}

	count = 0;	//프레임 카운트


	//npc 변수 초기화
	{
		npc[0].x = 2;
		npc[0].y = 12;
		npc[0].img = IMAGEMANAGER->findImage("npc카운터아저씨");

		npc[1].x = 2;
		npc[1].y = 6;
		npc[1].img = IMAGEMANAGER->findImage("npc카운터아가씨");

		npc[2].x = 10;
		npc[2].y = 1;
		npc[2].img = IMAGEMANAGER->findImage("npc세이브");

		npc[3].x = 12;
		npc[3].y = 2;
		npc[3].img = IMAGEMANAGER->findImage("char_15e");

		for (int i = 0; i < NPCSIZE; i++) npc[i].isTalk = false;


		//간판
		sign.x = 20;
		sign.y = 1;
		sign.isTalk = false;
	}


	//캐릭터 변수 초기화
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
	//체력바 클래스 초기화
	hpBar = new progressBar;
	hpBar->init("IMAGE/ui/hpBar.322.8", "IMAGE/shadow", 64, 436, 322, 8);
	hpBar->setGauge(hp, hpMax);

	//A* 변수 초기화
	{
		//전체노드 초기화
		for (int i = 0; i < MAXTILESIZE_TOWN; i++)
		{
			for (int j = 0; j < MAXTILESIZE_TOWN; j++)
			{
				//새로운 노드와 렉트위치 설정
				_totalNode[i][j] = new node(i, j);
				//_totalNode[i][j]->rc = tile[i][j].rc;
			}
		}
		isNodePick = false;	//A* 노드 선택 여부
		nodeNum = 0;		//A* 노드 수

		//첫클릭이 짝수가 되게 하기위해서 -1로 초기화
		_count = -1;
		//길찾았냐?
		_isFind = false;
	}

	//캠 기준 마우스
	_ptMouseC = { _ptMouse.x + x, _ptMouse.y + y };

	//맵 파일 로드
	{
		HANDLE file;
		DWORD read;

		file = CreateFile("map_town.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE_TOWN * MAXTILESIZE_TOWN, &read, NULL);

		//맵 타일 로드
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
				tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //초기 타일(빈 타일)
				tile_img[i][j].img_obj_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj_mini); //초기 타일(빈 타일)
				tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //초기 타일(빈 타일)
				tile_img[i][j].img_enemy_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy_mini); //초기 타일(빈 타일)

				//A* 벽 처리
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

	//플레이어 체력바 업데이트
	hpBar->update();
	hpBar->setGauge(hp, hpMax);
	//체력 다운
	if (KEYMANAGER->isStayKeyDown(VK_DOWN)) hp -= 10;

	//for문 체크 제한
	{
		//화면 렌더 제한 - 가로 10, 세로 14
		x_for_start = index.x - 11;
		if (x_for_start < 0) x_for_start = 0;
		x_for_end = index.x + 11;
		if (x_for_end > tileSize) x_for_end = tileSize;
		y_for_start = index.y - 11;
		if (y_for_start < 0) y_for_start = 0;
		y_for_end = index.y + 11;
		if (y_for_end > tileSize) y_for_end = tileSize;

		//z-order 체크 제한 - 가로 4, 세로 4
		x_for_start_z = index.x - 5;
		if (x_for_start_z < 0) x_for_start_z = 0;
		x_for_end_z = index.x + 5;
		if (x_for_end_z > tileSize) x_for_end_z = tileSize;
		y_for_start_z = index.y - 5;
		if (y_for_start_z < 0) y_for_start_z = 0;
		y_for_end_z = index.y + 5;
		if (y_for_end_z > tileSize) y_for_end_z = tileSize;
	}


	//A*기능 처리 
	{
		//우클릭하면 노드 찾기
		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON) && state == IDLE)
		{
			//대사창 닫기
			for (int i = 0; i < NPCSIZE; i++) npc[i].isTalk = false;
			sign.isTalk = false; //간판

			//대화 가능 npc 대화창 on/off 처리
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
				//간판
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
			//노드 클리어
			_openList.clear();
			_closeList.clear();
			_finalList.clear();
			nodeNum = 0;


			//노드 찾기
			for (int i = x_for_start; i < x_for_end; i++)
			{
				for (int j = y_for_start; j < y_for_end; j++)
				{
					if ((-0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 &&
						(0.5) * (_ptMouseC.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(-0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 &&
						(0.5) * (_ptMouseC.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 && 
						_totalNode[i][j]->nodeState != NODE_WALL)//종료 노드 세팅
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

		//노드 따라 이동하기
		if (_finalList.size() > 0)
		{
			if (_finalList.size() > nodeNum && _finalList.size() != nodeNum + 1)
			{
				//방향 지정
				if (_finalList[nodeNum]->index.x + 1 == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y == _finalList[nodeNum + 1]->index.y)//좌하
				{
					direc_LR = LEFT;
					direc_DU = DOWN;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y + 1 == _finalList[nodeNum + 1]->index.y)//우하
				{
					direc_LR = RIGHT;
					direc_DU = DOWN;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y - 1 == _finalList[nodeNum + 1]->index.y)//좌상
				{
					direc_LR = LEFT;
					direc_DU = UP;
					state = RUN;
				}
				else if (_finalList[nodeNum]->index.x - 1 == _finalList[nodeNum + 1]->index.x &&
					_finalList[nodeNum]->index.y == _finalList[nodeNum + 1]->index.y)//우상
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
				//노드 클리어
				_openList.clear();
				_closeList.clear();
				_finalList.clear();
			}


			if (state == RUN)
			{
				//이동 처리
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

				//인덱스 갱신
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

	//캐릭터 인데스 주변 z-order 체킹
	for (int i = x_for_start_z; i < x_for_end_z; i++)
	{
		for (int j = y_for_start_z; j < y_for_end_z; j++)
		{
			if (y < tile[i][j].y) tile[i][j].z_order = true;
			else tile[i][j].z_order = false;
		}
	}

	this->userControl();


	//일반 필드로 이동
	//if (index.x == 17 && index.y == 0) SCENEMANAGER->loadScene("일반필드");
	if (index.x == 17 && index.y == 0)
	{
		_isFind = false;
		//노드 클리어
		_openList.clear();
		_closeList.clear();
		_finalList.clear();
		nodeNum = 0;
		SOUNDMANAGER->play("close", 0.2f);
		SCENEMANAGER->loadScene("일반필드");
	}

	//캐릭터의 RC
	rc = RectMake(x - IMAGEMANAGER->findImage("run1")->getWidth() / 8, y - IMAGEMANAGER->findImage("run1")->getHeight(), 330 / 4, 67);
	//rc_clipping = RectMakeCenter(x, y, 540, 400);
	rc_clipping = RectMakeCenter(x, y, WINSIZEX, WINSIZEY);
	//캠 기준 마우스 좌표 업데이트
	_ptMouseC = { _ptMouse.x - (tile[init_pos.x][init_pos.y].x - x), _ptMouse.y - (tile[init_pos.x][init_pos.y].y - y) };
}

void townScene::render()
{
	//카메라 기준 맵 렌더
	{
		//타일 렌더
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
				{
					//타일 렌더
					if (tile_strKey[i][j].isImg)
						tile_img[i][j].img->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x), tile[i][j].rc.top + (tile[init_pos.x][init_pos.y].y - y)); //타일 이미지 렌더
				}
			}
		}

		//배경 랜더
		img_bg->render(getMemDC(), tile[20][0].rc.left + 16 + (tile[init_pos.x][init_pos.y].x - x), tile[0][0].rc.top - 204 + (tile[init_pos.x][init_pos.y].y - y));

		//에너미 + 오브젝트 렌더
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				//에너미 렌더
				if (tile_strKey[i][j].isImg_enemy)
					tile_img[i][j].img_enemy->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_enemy->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_enemy->getHeight());

				//오브젝트 렌더 + 높이 처리
				if (!tile[i][j].z_order)
				{
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_obj->getHeight());
				}
			}
		}
	}

	//캐릭터 렌더
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
		//imageNum("숫자2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("idle")->getFrameX(), 20);
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
				//imageNum("숫자2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run1")->getFrameX(), 20);
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
				//imageNum("숫자2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run2")->getFrameX(), 20);
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
				//imageNum("숫자2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run3")->getFrameX(), 20);
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
				//imageNum("숫자2", getMemDC(), 160, 20, IMAGEMANAGER->findImage("run4")->getFrameX(), 20);
				IMAGEMANAGER->findImage("run4")->frameRender(getMemDC(), rc.left + (tile[init_pos.x][init_pos.y].x - x), rc.top + (tile[init_pos.x][init_pos.y].y - y));
				break;
			}
			break;
		}
	}


	//npc랜더
	{
		if (count % 15 == 0) //카운터아저씨
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

		if (count % 15 == 0) //카운터아가씨
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

		if (count % 15 == 0) //세이브
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

		//포탈
		if (count % 4 == 0)
		{
			if (IMAGEMANAGER->findImage("포탈")->getFrameX() == 5)
			{
				IMAGEMANAGER->findImage("포탈")->setFrameX(-1);
			}
			IMAGEMANAGER->findImage("포탈")->setFrameX(IMAGEMANAGER->findImage("포탈")->getFrameX() + 1);
		}
		IMAGEMANAGER->findImage("포탈")->frameRender(getMemDC(),
			tile[17][0].rc.left + (tile[init_pos.x][init_pos.y].x - x),
			tile[17][0].rc.bottom - 180 + (tile[init_pos.x][init_pos.y].y - y));

		//말풍선 렌더
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



	//(Z오더)타일 렌더 + 클리핑 처리 + 오브젝트 렌더
	for (int i = x_for_start; i < x_for_end; i++)
	{
		for (int j = y_for_start; j < y_for_end; j++)
		{
			//오브젝트 렌더 + 높이 처리
			if (tile[i][j].z_order)
			{
				if (tile_strKey[i][j].isImg_obj)
					tile_img[i][j].img_obj->render(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x) - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom + (tile[init_pos.x][init_pos.y].y - y) - tile_img[i][j].img_obj->getHeight());
			}
		}
	}

	// < 치트 > 토글 키1
	{
		for (int i = x_for_start; i < x_for_end; i++)
		{
			for (int j = y_for_start; j < y_for_end; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
				{
					if (KEYMANAGER->isToggleKey(VK_F5)) drawTileIndex(tile[i][j], i, j, tile[init_pos.x][init_pos.y]);//타일 인덱스 렌더
					//if (KEYMANAGER->isToggleKey(VK_F6)) FrameRect(getMemDC(), tile[i][j].rc_picking, RGB(255, 0, 255));//픽킹 렉터 렌더
					if (KEYMANAGER->isToggleKey(VK_F7))
					{
						drawDiamond(tile[i][j], PS_DOT, RGB(255, 255, 0));//아이소 타일 렌더
						//클리핑 렉터 출력<- 요놈을 카메라로?
						FrameRect(getMemDC(), rc_clipping, RGB(255, 0, 255));
						//벽노드 보여주기
						if (_totalNode[i][j]->nodeState == NODE_WALL)
						{
							setNodeColor(tile[i][j], RGB(200, 150, 100));
						}
					}
					if (KEYMANAGER->isToggleKey(VK_F8)) //이동 가능 지역 체크 렌더
					{
						if (tile_strKey[i][j].state_obj == BLOCK)
						{
							IMAGEMANAGER->findImage("tile_block")->alphaRender(getMemDC(), tile[i][j].rc.left + (tile[init_pos.x][init_pos.y].x - x), tile[i][j].rc.top + (tile[init_pos.x][init_pos.y].y - y), 80);
						}
					}
				}
			}
		}
		//토글키 사운드 출력
		if (KEYMANAGER->isOnceKeyUp(VK_F5) || KEYMANAGER->isOnceKeyUp(VK_F6) || KEYMANAGER->isOnceKeyUp(VK_F7) || KEYMANAGER->isOnceKeyUp(VK_F8) || KEYMANAGER->isOnceKeyUp(VK_F9))	SOUNDMANAGER->play("버튼");
	}

	//픽킹 마우스 포인터
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

	//A* 출력
	{
		//길찾았을때 보여주기
		if (_isFind)
		{
			for (int i = 0; i < _finalList.size(); i++)
			{
				if (KEYMANAGER->isToggleKey(VK_F7)) this->drawDiamond(tile[_finalList[i]->index.x][_finalList[i]->index.y], PS_SOLID, RGB(255, 0, 255));
				this->drawDiamond(tile[_finalList[i]->index.x][_finalList[i]->index.y], PS_SOLID, RGB(50, 255, 50), tile[init_pos.x][init_pos.y]);
			}
		}
	}

	//시야 처리
	if (KEYMANAGER->isToggleKey(VK_F10)) GDIPLUS->gdiRender("viewSight", getMemDC());

	//플레이어 hud 출력
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

		textOutFont(getMemDC(), 559, 421, "Q", 20, RGB(220, 220, 32), "궁서");
		textOutFont(getMemDC(), 613, 421, "W", 20, RGB(220, 220, 32), "궁서");

		textOutFont(getMemDC(), 190, 434, "(", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(), 200, 434, hp, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 230, 434, "/", 16, RGB(200, 200, 0));
		textOutFontNum(getMemDC(), 240, 434, hpMax, 16, RGB(200, 200, 100));
		textOutFont(getMemDC(), 270, 434, ")", 16, RGB(200, 200, 0));
	}


	// < 치트 > 토글 키2
	if (KEYMANAGER->isToggleKey(VK_F7))
	{
		FrameRect(getMemDC(), rc, RGB(255, 255, 255));
		char str_test2[128];
		sprintf(str_test2, "%d, %d / [%d, %d] / state: %d", x, y, index.x, index.y, state);
		TextOut(getMemDC(), x, y, str_test2, strlen(str_test2));
		IMAGEMANAGER->findImage("마우스테스트")->render(getMemDC(), _ptMouseC.x, _ptMouseC.y);
	}



	//npc 대사창 렌더
	{
		if (npc[0].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ 바텐터 ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "또 오셨네요~(찡긋)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("spectate")) SOUNDMANAGER->play("spectate", 0.2f);
		}
		if (npc[1].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ 예의 바른 카운터 점원 ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "안녕하세요~(빵긋)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("spectate")) SOUNDMANAGER->play("spectate", 0.2f);
		}
		if (npc[2].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "[ 차원 천사 ]", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "새지구를 찾아보자~", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30 + 24, "(체력을 회복합니다.)", 20, RGB(255, 155, 155));
			if (hp < hpMax) hp++; 
			if (!SOUNDMANAGER->isPlaySound("magic02")) SOUNDMANAGER->play("magic02", 0.4f);
			if (!SOUNDMANAGER->isPlaySound("church")) SOUNDMANAGER->play("church", 0.4f);
		}
		if (npc[3].isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "냥~!", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (손대면 물어버린다냥)", 20, RGB(255, 255, 255));
			if (!SOUNDMANAGER->isPlaySound("cat")) SOUNDMANAGER->play("cat", 0.2f);
		}
		if (sign.isTalk)
		{
			IMAGEMANAGER->findImage("skill_bg")->alphaRender(getMemDC(), 25, 350, 160);
			drawTalk(getMemDC());
			textOutFont(getMemDC(), 40, 360, "- 북서 : 모험가의 필드 -", 20, RGB(255, 255, 255));
			textOutFont(getMemDC(), 40, 360 + 30, "   (주의 : 이불 밖은 위험햇 !!)", 20, RGB(255, 255, 255));
			
		}
	}


	//textOutDbug(getMemDC(), rc.right, rc.top, x);
	//textOutDbug(getMemDC(), rc.right, rc.top + 20, y);


	SetBkMode(getMemDC(), 2);

	//현재 씬
	//TextOut(getMemDC(), WINSIZEX / 2, 0, "타운", strlen("타운"));
	fade(getMemDC(), _startTime, true);
}


//마름모 타일 렌더 함수 PS_DOT
void townScene::drawDiamond(tagTile tile, int iStyle, COLORREF color)
{

	HPEN Pen, oldPen;
	Pen = CreatePen(iStyle, 1, color);
	oldPen = (HPEN)SelectObject(getMemDC(), Pen);

	//마름모 그리기
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

	//마름모 그리기
	MoveToEx(getMemDC(), tile.x + (firstTile.x - x) - (TILEWIDTH / 2), tile.y + (firstTile.y - y), NULL);
	LineTo(getMemDC(), tile.x + (firstTile.x - x), tile.y - (TILEHEIGHT / 2) + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x) + (TILEWIDTH / 2), tile.y + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x), tile.y + (TILEHEIGHT / 2) + (firstTile.y - y));
	LineTo(getMemDC(), tile.x + (firstTile.x - x) - (TILEWIDTH / 2), tile.y + (firstTile.y - y));

	SelectObject(getMemDC(), oldPen);
	DeleteObject(Pen);
}


//해당 타일 좌표(인덱스) 표시 함수
void townScene::drawTileIndex(tagTile tile, int i, int j)
{
	//좌표 표시
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12, tile.y - 8, str, strlen(str));
}
void townScene::drawTileIndex(tagTile tile, int i, int j, tagTile firstTile)
{
	//좌표 표시
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12 + (firstTile.x - x), tile.y - 8 + (firstTile.y - y), str, strlen(str));
}


//유저 컨트롤 함수 : 맵크기조정(+,-), 맵슬라이드(wasd), 타일 선택(방향키)
void townScene::userControl()
{
	//타일 크기 조정
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_PLUS) && tileSize < MAXTILESIZE_TOWN)
	{
		tileSize++;
		SOUNDMANAGER->play("버튼");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_MINUS) && tileSize > 1)
	{
		tileSize--;
		SOUNDMANAGER->play("버튼");
	}


}




//노드 색채우기(편리성때문에 만든함수)
void townScene::setNodeColor(tagTile tile, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	RECT fill = RectMakeCenter(tile.x, tile.y, TILEWIDTH / 2, TILEHEIGHT / 2);
	FillRect(getMemDC(), &fill, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), fill, RGB(0, 0, 0));
}

//길찾기 함수
void townScene::pathFinding()
{
	//시작노드 및 종료노드가 없는 경우 길찾기 못함
	if (!_startNode || !_endNode) return;

	//길찾기를 해보자!!
	//검색을 하려면 무조건 오픈리스트에 담아두고 
	//F와 H값 가장작은 놈을 찾아서 그놈을 현재노드로 변경하고
	//오픈리스트에서 현재노드는 삭제하고 
	//현재노드는 클로즈리스트에 담아둔다

	//1. 시작노드가 있어야 출발이 가능하니 
	//시작노드를 오픈리스트에 추가를 해줘야 한다
	_openList.push_back(_startNode);
	_finalList.push_back(_startNode);

	//2. 오픈리스트안에 담겨 있는 벡터를 검사해서 
	//종료노드에 도착할때까지 무한루프
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		//오픈리스트 중 가장 F가 작거나 F가 같다면 
		//H가 작은 걸 현재노드로 하고
		//현재노드를 오픈리스트에서 클로즈리스트로 옮기기
		//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
		//길찾기가 시작되니 1부터 시작하자
		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//클로즈리스트에 넣어둔다
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//현재노드가 마지막노드와 같냐? (길찾았냐?)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
			while (endNode != _startNode)
			{
				tempNode.push_back(endNode);
				endNode = endNode->parentNode;
			}

			//STL리스트를 이용하면 reverse()사용하거나, push_front()를 사용할 수 있지만,
			//우린 벡터를 이용하니 꺼꾸로 푸시백해준다
			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				_finalList.push_back(tempNode[i]);
			}

			_isFind = true;
			//종료하고 빠져나온다
			return;
		}

		//상하좌우 (순서는 상괌없음 - 어짜피 주면 4개의 노드를 모두 오픈리스트에 넣어서 검사할 예정임)
		this->addOpenList(_curNode->index.x, _curNode->index.y - 1); //상
		this->addOpenList(_curNode->index.x, _curNode->index.y + 1); //하
		this->addOpenList(_curNode->index.x - 1, _curNode->index.y); //좌
		this->addOpenList(_curNode->index.x + 1, _curNode->index.y); //우

		//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
		//우상, 좌상, 우하, 좌하
		//예외처리만 잘해주면 된다
		//벽사이로 막가 안된다 등등

	}


}

//오픈리스트에 추가 함수
void townScene::addOpenList(int idx, int idy)
{
	//예외처리
	//오픈리스트에 담을 노드가 전체노드 인덱스 밖으로 나가면 추가하지 못하게 처리한다(인덱스 범위는 0 ~ 4까지)
	if (idx < 0 || idx >= MAXTILESIZE_TOWN || idy < 0 || idy >= MAXTILESIZE_TOWN) return;
	//장애물은 오픈리스트에 담을 수 없다
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;
	//클로즈리스트에 있다면 이미 지나온길이니 오픈리스트에 담으면 안된다
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//여기까지 왔으면 문제가 없으며 이제 오픈리스트에 추가를 하자
	//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가하기
	node* neighborNode = _totalNode[idx][idy];
	int moveCost = _curNode->G + ((_curNode->index.x - idx == 0 || _curNode->index.y - idy == 0) ? 10 : 14);

	//오픈리스트안에 이웃노드가 있으면 안된다
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//마지막으로 오픈리스트에도 없는경우 
	//G, H, ParentNode 설정 후 오픈리스트에 추가
	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->index.x - _endNode->index.x) + abs(neighborNode->index.y - _endNode->index.y)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = _curNode;
	_openList.push_back(neighborNode);
}

//오픈리스트 삭제
void townScene::delOpenList(int index)
{
	_openList.erase(_openList.begin() + index);
}


