#include "stdafx.h"
#include "mapToolScene.h"



HRESULT mapToolScene::init()
{
	init_pos = { 0, 0 };


	tileSize = MAXTILESIZE;
	rc_clipping = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 540 * 2, 380 * 2);
	rc_clipping_mini = RectMake(UI.rc_bg.left + 7, UI.rc_bg.top + 24, 140 - 4, 90 - 4);
	pt_tileMark = { 240 + 160, 100 };
	pt_tileMark_mini_x = 80;
	pt_tileMark_mini_y = 40;
	//�� Ÿ�� �ʱ�ȭ
	for (int i = 0; i < MAXTILESIZE; i++)
	{
		for (int j = 0; j < MAXTILESIZE; j++)
		{
			tile[i][j].x = pt_tileMark.x - (i * 64 / 2) + (j * 64 / 2);
			tile[i][j].y = pt_tileMark.y + (i * 32 / 2) + (j * 32 / 2);
			tile[i][j].rc = RectMakeCenter(tile[i][j].x, tile[i][j].y, 64, 32);
			//tile[i][j].rc_picking = RectMakeCenter(tile[i][j].x, tile[i][j].y, 64 / 2, 32 / 2);
			tile[i][j].x_mini = pt_tileMark_mini_x - ((i * 64 / 2) * 0.1) + ((j * 64 / 2) * 0.1);
			tile[i][j].y_mini = pt_tileMark_mini_y + ((i * 32 / 2) * 0.1) + ((j * 32 / 2) * 0.1);
			tile[i][j].rc_mini = RectMake(tile[i][j].x_mini, tile[i][j].y_mini, 64 / 10, 32 / 10);
			sprintf(tile_strKey[i][j].str_img, "%s", "���̹���");
			//sprintf(tile_strKey[i][j].str_img, "%s", "���̹���");
			tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
			sprintf(tile_strKey[i][j].str_img_mini, "%s", "���̹���");
			//sprintf(tile_strKey[i][j].str_img_mini, "%s", "���̹���");
			tile_img[i][j].img_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_mini);
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
			sprintf(tile_strKey[i][j].str_img_floor_mini[0], "%s", "mini_st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor_mini[1], "%s", "mini_st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor_mini[2], "%s", "mini_st51a_37");
			sprintf(tile_strKey[i][j].str_img_floor_mini[3], "%s", "mini_st51a_37");
			tile_img[i][j].img_floor_mini[0] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[0]);
			tile_img[i][j].img_floor_mini[1] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[1]);
			tile_img[i][j].img_floor_mini[2] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[2]);
			tile_img[i][j].img_floor_mini[3] = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_floor_mini[3]);

			sprintf(tile_strKey[i][j].str_img_obj, "%s", "���̹���");
			tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj); //�ʱ� Ÿ��(�� Ÿ��)
			sprintf(tile_strKey[i][j].str_img_obj_mini, "%s", "���̹���");
			tile_img[i][j].img_obj_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj_mini); //�ʱ� Ÿ��(�� Ÿ��)
			sprintf(tile_strKey[i][j].str_img_enemy, "%s", "���̹���");
			tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy); //�ʱ� Ÿ��(�� Ÿ��)
			sprintf(tile_strKey[i][j].str_img_enemy_mini, "%s", "���̹���");
			tile_img[i][j].img_enemy_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy_mini); //�ʱ� Ÿ��(�� Ÿ��)

			tile_strKey[i][j].isImg_obj = false;
			tile_strKey[i][j].isImg_enemy = false;

			tile[i][j].z_order = false;
			tile[i][j].isFloodFill = false;
		}
	}
	
	this->initMenu_tile();
	this->initMenu_obj();
	this->initMenu_enemy();

	now = 0;

	_startTime = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void mapToolScene::release()
{
	
}

void mapToolScene::update()
{
	

	this->userControl();

	//�� Ÿ�� ������Ʈ
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			tile[i][j].x = pt_tileMark.x - (i * 64 / 2) + (j * 64 / 2);
			tile[i][j].y = pt_tileMark.y + (i * 32 / 2) + (j * 32 / 2);
			tile[i][j].rc = RectMakeCenter(tile[i][j].x, tile[i][j].y, 64, 32);
			//tile[i][j].rc_picking = RectMakeCenter(tile[i][j].x, tile[i][j].y, 64 / 2, 32 / 2);
		}
	}

	//�̴ϸ� Ÿ�� ������Ʈ
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			tile[i][j].x_mini = pt_tileMark_mini_x - ((i * 64 / 2) * 0.1) + ((j * 64 / 2) * 0.1);
			tile[i][j].y_mini = pt_tileMark_mini_y + ((i * 32 / 2) * 0.1) + ((j * 32 / 2) * 0.1);
			tile[i][j].rc_mini = RectMake(tile[i][j].x_mini, tile[i][j].y_mini, 64 / 10, 32 / 10);
		}
	}
}

void mapToolScene::render()
{
	//Ÿ�� ���� + Ŭ���� ó�� + ������Ʈ ����
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
			{
				//Ÿ�� ���� ���� ����
				if (tile_strKey[i][j].isImg)
					tile_img[i][j].img->render(getMemDC(), tile[i][j].rc.left, tile[i][j].rc.top - (TILEHEIGHT / 2) * tile_strKey[i][j].floor); //Ÿ�� �̹��� ����
				for (int k = 0; k < tile_strKey[i][j].floor; k++)
					tile_img[i][j].img_floor[k]->render(getMemDC(), tile[i][j].rc.left, tile[i][j].rc.top - (TILEHEIGHT / 2) * k);

				//���ʹ� ���� + ���� ó��
				if (tile_strKey[i][j].isImg_enemy)
					tile_img[i][j].img_enemy->render(getMemDC(), tile[i][j].rc.left - tile_img[i][j].img_enemy->getWidth() / 2 + 32, tile[i][j].rc.bottom - tile_img[i][j].img_enemy->getHeight() - (TILEHEIGHT / 2) * tile_strKey[i][j].floor);
				
				if (!tile[i][j].z_order)
				{
					//������Ʈ ���� + ���� ó��
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj->render(getMemDC(), tile[i][j].rc.left - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom - tile_img[i][j].img_obj->getHeight() - (TILEHEIGHT / 2) * tile_strKey[i][j].floor);
				}
			}
		}
	}

	// < ġƮ > ��� Ű
	{
		for (int i = 0; i < tileSize; i++)
		{
			for (int j = 0; j < tileSize; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
				{
					if (KEYMANAGER->isToggleKey(VK_F5)) drowTileIndex(tile[i][j], i, j);//Ÿ�� �ε��� ����
					//if (KEYMANAGER->isToggleKey(VK_F6)) FrameRect(getMemDC(), tile[i][j].rc_picking, RGB(255, 0, 255));//��ŷ ���� ����
					if (KEYMANAGER->isToggleKey(VK_F7)) drowDiamond(tile[i][j], PS_DOT, RGB(255, 255, 0));//���̼� Ÿ�� ����
					if (!KEYMANAGER->isToggleKey(VK_F8)) //�̵� ���� ���� üũ ����
					{
						if (tile_strKey[i][j].state_obj == BLOCK) IMAGEMANAGER->findImage("tile_block")->alphaRender(getMemDC(), tile[i][j].rc.left, tile[i][j].rc.top, 80);
					}
				}
			}
		}
		//���Ű ���� ���
		if (KEYMANAGER->isOnceKeyUp(VK_F5) || KEYMANAGER->isOnceKeyUp(VK_F6) || KEYMANAGER->isOnceKeyUp(VK_F7) || KEYMANAGER->isOnceKeyUp(VK_F8) || KEYMANAGER->isOnceKeyUp(VK_F9)) SOUNDMANAGER->play("��ư");
	}
	
	//��ŷ GUI
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
				(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
				(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
				(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
				drowDiamond(tile[i][j], PS_SOLID, RGB(255, 0, 0));
		}
	}

	
	
	//Ÿ�� ���� + Ŭ���� ó�� + ������Ʈ ����
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			if (IntersectRect(&rc_temp, &rc_clipping, &tile[i][j].rc))
			{
				if (tile[i][j].z_order)
				{
					//������Ʈ ���� + ���� ó��
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj->alphaRender(getMemDC(), tile[i][j].rc.left - tile_img[i][j].img_obj->getWidth() / 2 + 32, tile[i][j].rc.bottom - tile_img[i][j].img_obj->getHeight() - (TILEHEIGHT / 2) * tile_strKey[i][j].floor, 128);
				}
			}
			
			if (tile[i][j].isFloodFill)
			{
				//textOutDbug(getMemDC(), tile[i][j].x, tile[i][j].y, tile[i][j].isFloodFill);
				this->drowDiamond(tile[i][j], PS_DOT, RGB(0, 255, 255));
			}
		}
	}

	//�޴�â ����
	{
		//UI��� ���
		FrameRect(getMemDC(), UI.rc_bg, RGB(200, 200, 200));
		UI.img_bg->alphaRender(getMemDC(), UI.rc_bg.left, UI.rc_bg.top, 200);
		
		//�̴ϸ� ��׶��� ���
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(getMemDC(), &UI.rc_miniMap, brush);
		DeleteObject(brush);

		//�̴ϸ� ���
		for (int i = 0; i < tileSize; i++)
		{
			for (int j = 0; j < tileSize; j++)
			{
				if (IntersectRect(&rc_temp, &rc_clipping_mini, &tile[i][j].rc_mini))
				{
					if (tile_strKey[i][j].isImg)
						tile_img[i][j].img_mini->render(getMemDC(), tile[i][j].rc_mini.left, tile[i][j].rc_mini.top - ((TILEHEIGHT / 2) * 0.1) * tile_strKey[i][j].floor); //Ÿ�� �̹��� ����
					for (int k = 0; k < tile_strKey[i][j].floor; k++)
						tile_img[i][j].img_floor_mini[k]->render(getMemDC(), tile[i][j].rc_mini.left, tile[i][j].rc_mini.top - ((TILEHEIGHT / 2) * 0.1) * k);

					//������Ʈ ���� + ���� ó��
					if (tile_strKey[i][j].isImg_obj)
						tile_img[i][j].img_obj_mini->render(getMemDC(), tile[i][j].rc_mini.left - ((tile_img[i][j].img_obj->getWidth() / 2) / 10) + 32 / 10, tile[i][j].rc_mini.top - (tile_img[i][j].img_obj->getHeight() / 10) + 3 - ((TILEHEIGHT / 2) * 0.1) * tile_strKey[i][j].floor);

					//���ʹ� ���� + ���� ó��
					if (tile_strKey[i][j].isImg_enemy)
						tile_img[i][j].img_enemy_mini->render(getMemDC(), tile[i][j].rc_mini.left - ((tile_img[i][j].img_enemy->getWidth() / 2) / 10) + 32 / 10, tile[i][j].rc_mini.top - (tile_img[i][j].img_enemy->getHeight() / 10) + 3 - ((TILEHEIGHT / 2) * 0.1) * tile_strKey[i][j].floor);
				}
			}
		}

		//�� ������ �ؽ�Ʈ ���
		char str[128];
		sprintf(str, " Map Size : %d x %d", tileSize, tileSize);
		textOutFont(getMemDC(), UI.rc_miniMap.left, UI.rc_miniMap.top, str, 20, RGB(255, 255, 255), "���� ���");
		
		//�� ī�޶� ���
		FrameRect(getMemDC(), UI.rc_miniMap_show, RGB(200, 200, 0));
		
		//���̺�,�ε� ��ư ���
		//IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_save.left, UI.rc_save.top, 40, 40, UI.rc_save.right - UI.rc_save.left, UI.rc_save.bottom - UI.rc_save.top, 210);
		//IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_load.left, UI.rc_load.top, 40, 40, UI.rc_load.right - UI.rc_load.left, UI.rc_load.bottom - UI.rc_load.top, 210);
		FrameRect(getMemDC(), UI.rc_save, RGB(0, 20, 0));
		FrameRect(getMemDC(), UI.rc_load, RGB(0, 20, 0));
		textOutFont(getMemDC(), UI.rc_save.left + 10, UI.rc_save.top + 6, "SAVE", 20);
		textOutFont(getMemDC(), UI.rc_load.left + 10, UI.rc_load.top + 6, "LOAD", 20);

		//���� Ÿ�� ������ ȭ��
		SetBkMode(getMemDC(), 0);
		//IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_preview.left, UI.rc_preview.top, 0, 0, UI.rc_preview.right - UI.rc_preview.left, UI.rc_preview.bottom - UI.rc_preview.top, 160);
		FrameRect(getMemDC(), UI.rc_preview, RGB(0, 0, 0));
		textOutFont(getMemDC(), UI.rc_preview.left, UI.rc_preview.top, "    [ Tile Preview ]", 20, RGB(0, 0, 0), "���� ���");

		//����Ʈ BG ���
		//IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_listBG.left, UI.rc_listBG.top, 0, 0, UI.rc_listBG.right - UI.rc_listBG.left, UI.rc_listBG.bottom - UI.rc_listBG.top, 160);
		FrameRect(getMemDC(), UI.rc_listBG, RGB(0, 0, 0));


		//Ÿ�� ���� �� ���
		SetBkMode(getMemDC(), 0);
		if (UI.tileSort == SORT_TILE)
		{
			//Ÿ�� ������ ȭ�� ���
			IMAGEMANAGER->findImage(tileMenu_land[UI.num_choice + now].str_img)->scaleRender(getMemDC(), UI.rc_preview.left, UI.rc_preview.top + 30, 2.2);
			//Ÿ�� ���ϸ� ���� ǥ�� ��Ʈ ���
			FrameRect(getMemDC(), UI.rc_choice, RGB(0, 0, 0));
			sprintf(str_tileMenu_land[0], "%s.bmp", tileMenu_land[now + 0].str_img);
			sprintf(str_tileMenu_land[1], "%s.bmp", tileMenu_land[now + 1].str_img);
			sprintf(str_tileMenu_land[2], "%s.bmp", tileMenu_land[now + 2].str_img);
			sprintf(str_tileMenu_land[3], "%s.bmp", tileMenu_land[now + 3].str_img);
			sprintf(str_tileMenu_land[4], "%s.bmp", tileMenu_land[now + 4].str_img);
			TextOut(getMemDC(), 20, 280, str_tileMenu_land[0], strlen(str_tileMenu_land[0]));
			TextOut(getMemDC(), 20, 300, str_tileMenu_land[1], strlen(str_tileMenu_land[1]));
			TextOut(getMemDC(), 20, 320, str_tileMenu_land[2], strlen(str_tileMenu_land[2]));
			TextOut(getMemDC(), 20, 340, str_tileMenu_land[3], strlen(str_tileMenu_land[3]));
			TextOut(getMemDC(), 20, 360, str_tileMenu_land[4], strlen(str_tileMenu_land[4]));
		}
		else if (UI.tileSort == SORT_OBJ)
		{
			//Ÿ�� ������ ȭ�� ��� UI.rc_preview = RectMake(UI.rc_miniMap.left, UI.rc_save.bottom + 4, 140, 110);
			if(IMAGEMANAGER->findImage(tileMenu_obj[UI.num_choice + now].str_img)->getHeight() > 110)IMAGEMANAGER->findImage(tileMenu_obj[UI.num_choice + now].str_img_mini)->scaleRender(getMemDC(), UI.rc_preview.left + 36, UI.rc_preview.top + 20, (94 / IMAGEMANAGER->findImage(tileMenu_obj[UI.num_choice + now].str_img_mini)->getHeight()));
			else IMAGEMANAGER->findImage(tileMenu_obj[UI.num_choice + now].str_img)->render(getMemDC(), UI.rc_preview.left + 36, UI.rc_preview.top + 20);
			//Ÿ�� ���ϸ� ���� ǥ�� ��Ʈ ���
			FrameRect(getMemDC(), UI.rc_choice, RGB(0, 0, 0));
			sprintf(str_tileMenu_obj[0], "%s.bmp", tileMenu_obj[now + 0].str_img);
			sprintf(str_tileMenu_obj[1], "%s.bmp", tileMenu_obj[now + 1].str_img);
			sprintf(str_tileMenu_obj[2], "%s.bmp", tileMenu_obj[now + 2].str_img);
			sprintf(str_tileMenu_obj[3], "%s.bmp", tileMenu_obj[now + 3].str_img);
			sprintf(str_tileMenu_obj[4], "%s.bmp", tileMenu_obj[now + 4].str_img);
			TextOut(getMemDC(), 20, 280, str_tileMenu_obj[0], strlen(str_tileMenu_obj[0]));
			TextOut(getMemDC(), 20, 300, str_tileMenu_obj[1], strlen(str_tileMenu_obj[1]));
			TextOut(getMemDC(), 20, 320, str_tileMenu_obj[2], strlen(str_tileMenu_obj[2]));
			TextOut(getMemDC(), 20, 340, str_tileMenu_obj[3], strlen(str_tileMenu_obj[3]));
			TextOut(getMemDC(), 20, 360, str_tileMenu_obj[4], strlen(str_tileMenu_obj[4]));
		}
		else if (UI.tileSort == SORT_ENEMY)
		{
			//Ÿ�� ������ ȭ�� ��� UI.rc_preview = RectMake(UI.rc_miniMap.left, UI.rc_save.bottom + 4, 140, 110);
			if (IMAGEMANAGER->findImage(tileMenu_enemy[UI.num_choice + now].str_img)->getHeight() > 110) IMAGEMANAGER->findImage(tileMenu_enemy[UI.num_choice + now].str_img_mini)->scaleRender(getMemDC(), UI.rc_preview.left + 36, UI.rc_preview.top + 20, (94 / IMAGEMANAGER->findImage(tileMenu_enemy[UI.num_choice + now].str_img_mini)->getHeight()));
			else IMAGEMANAGER->findImage(tileMenu_enemy[UI.num_choice + now].str_img)->render(getMemDC(), UI.rc_preview.left + 36, UI.rc_preview.top + 20);
			//Ÿ�� ���ϸ� ���� ǥ�� ��Ʈ ���
			FrameRect(getMemDC(), UI.rc_choice, RGB(0, 0, 0));
			sprintf(str_tileMenu_enemy[0], "%s.bmp", tileMenu_enemy[now + 0].str_img);
			sprintf(str_tileMenu_enemy[1], "%s.bmp", tileMenu_enemy[now + 1].str_img);
			sprintf(str_tileMenu_enemy[2], "%s.bmp", tileMenu_enemy[now + 2].str_img);
			sprintf(str_tileMenu_enemy[3], "%s.bmp", tileMenu_enemy[now + 3].str_img);
			sprintf(str_tileMenu_enemy[4], "%s.bmp", tileMenu_enemy[now + 4].str_img);
			TextOut(getMemDC(), 20, 280, str_tileMenu_enemy[0], strlen(str_tileMenu_enemy[0]));
			TextOut(getMemDC(), 20, 300, str_tileMenu_enemy[1], strlen(str_tileMenu_enemy[1]));
			TextOut(getMemDC(), 20, 320, str_tileMenu_enemy[2], strlen(str_tileMenu_enemy[2]));
			TextOut(getMemDC(), 20, 340, str_tileMenu_enemy[3], strlen(str_tileMenu_enemy[3]));
			TextOut(getMemDC(), 20, 360, str_tileMenu_enemy[4], strlen(str_tileMenu_enemy[4]));
		}
		else if (UI.tileSort == SORT_BLOCK)
		{
			//Ÿ�� ������ ȭ�� ���
			IMAGEMANAGER->findImage("tile_block")->scaleRender(getMemDC(), UI.rc_preview.left, UI.rc_preview.top + 30, 2.2);
			FrameRect(getMemDC(), RectMake(UI.rc_preview.left + 2, UI.rc_preview.bottom + 6, 136, 20), RGB(0, 0, 0));
			TextOut(getMemDC(), 20, 280, "tile_block.bmp", strlen("tile_block.bmp"));
		}

		//�з� ���
		//IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_sortBG.left, UI.rc_sortBG.top, 0, 0, UI.rc_sortBG.right - UI.rc_sortBG.left, UI.rc_sortBG.bottom - UI.rc_sortBG.top, 160);
		FrameRect(getMemDC(), UI.rc_sortBG, RGB(0, 0, 0));
		TextOut(getMemDC(), UI.rc_tile.left, UI.rc_tile.top, "Tile", strlen("Tile"));
		TextOut(getMemDC(), UI.rc_obj.left, UI.rc_obj.top, "Obj", strlen("Obj"));
		TextOut(getMemDC(), UI.rc_enemy.left, UI.rc_enemy.top, "Enemy", strlen("Enemy"));
		TextOut(getMemDC(), UI.rc_block.left, UI.rc_block.top, "Block", strlen("Block"));
		if (UI.tileSort == SORT_TILE) FrameRect(getMemDC(), UI.rc_tile, RGB(0, 0, 0));
		if (UI.tileSort == SORT_OBJ) FrameRect(getMemDC(), UI.rc_obj, RGB(0, 0, 0));
		if (UI.tileSort == SORT_ENEMY) FrameRect(getMemDC(), UI.rc_enemy, RGB(0, 0, 0));
		if (UI.tileSort == SORT_BLOCK) FrameRect(getMemDC(), UI.rc_block, RGB(0, 0, 0));

		
		if(UI.isMagicTool) FrameRect(getMemDC(), UI.rc_magicTool, RGB(255, 0, 0));
		else FrameRect(getMemDC(), UI.rc_magicTool, RGB(0, 0, 0));
		TextOut(getMemDC(), UI.rc_magicTool.left + 4, UI.rc_magicTool.top + 2, "�� Magic Tool", strlen("�� Magic Tool"));

		//�޴��� ���
		FrameRect(getMemDC(), UI.rc_manual, RGB(200, 200, 200));
		IMAGEMANAGER->findImage("menu_bg")->alphaRender(getMemDC(), UI.rc_manual.left, UI.rc_manual.top, 160);
		if(!UI.isManual) textOutFont(getMemDC(), UI.rc_manual.left + 2, UI.rc_manual.top + 2, "��Manual", 20, RGB(00, 00, 00));
		else 
		{
			IMAGEMANAGER->findImage("keyInfo.1280.350")->alphaRender(getMemDC(), UI.rc_bg.right + 10, UI.rc_bg.top + 340, 160);
			textOutFont(getMemDC(), UI.rc_manual.left + 2, UI.rc_manual.top + 2, "��Manual", 20, RGB(00, 00, 00));
		}
	}

	//���̺�, �ε� �˾�â ����
	if (UI.isSave)
	{
		this->drowRect(UI.rc_popUP_BG, RGB(210, 210, 210));
		textOutFont(getMemDC(), UI.rc_popUP_BG.left + 60, UI.rc_popUP_BG.top + 50, "���� �Ͻðڽ��ϱ�?", 24);
	}
	if (UI.isLoad)
	{
		this->drowRect(UI.rc_popUP_BG, RGB(210, 210, 210));
		textOutFont(getMemDC(), UI.rc_popUP_BG.left + 60, UI.rc_popUP_BG.top + 50, "�ҷ� ���ðڽ��ϱ�?", 24);
	}

	//Ŭ���� ���� ���
	if (KEYMANAGER->isToggleKey(VK_F9))
	{
		FrameRect(getMemDC(), rc_clipping_mini, RGB(255, 0, 255));
		FrameRect(getMemDC(), rc_clipping, RGB(255, 0, 255));
	}
	

	SetBkMode(getMemDC(), 2);
	textOutMouse(getMemDC());
	fade(getMemDC(), _startTime, true);
}

//������ Ÿ�� ���� �Լ� PS_DOT
void mapToolScene::drowDiamond(tagTile tile, int iStyle, COLORREF color)
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

void mapToolScene::drowRect(RECT rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), rc, RGB(0, 0, 0));
	FrameRect(getMemDC(), UI.rc_popUP_yes, RGB(0, 0, 0));
	FrameRect(getMemDC(), UI.rc_popUP_no, RGB(0, 0, 0));
	textOutFont(getMemDC(), UI.rc_popUP_yes.left + 14, UI.rc_popUP_yes.top + 4, "YES", 24);
	textOutFont(getMemDC(), UI.rc_popUP_no.left + 20, UI.rc_popUP_no.top + 4, "NO", 24);
}

//�ش� Ÿ�� ��ǥ(�ε���) ǥ�� �Լ�
void mapToolScene::drowTileIndex(tagTile tile, int i, int j)
{
	//��ǥ ǥ��
	char str[128];
	sprintf(str, "(%d,%d)", i, j);
	TextOut(getMemDC(), tile.x - 12, tile.y - 8, str, strlen(str));
}

//���� ��Ʈ�� �Լ� : ��ũ������(+,-), �ʽ����̵�(wasd), Ÿ�� ����(����Ű)
void mapToolScene::userControl()
{
	//Ÿ�� ũ�� ����
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_PLUS) && tileSize < MAXTILESIZE)
	{
		tileSize++; 
		SOUNDMANAGER->play("��ư");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_OEM_MINUS) && tileSize > 1)
	{
		tileSize--;
		SOUNDMANAGER->play("��ư");
	}

	//�� �����̵�
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		pt_tileMark.x -= 4;
		pt_tileMark_mini_x -= 0.4f;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		pt_tileMark.x += 4;
		pt_tileMark_mini_x += 0.4f;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		pt_tileMark.y -= 4;
		pt_tileMark_mini_y -= 0.4f;
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		pt_tileMark.y += 4;
		pt_tileMark_mini_y += 0.4f;
	}

	//Ÿ�� ����(����Ű)
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && UI.num_choice >= 0)
	{
		if (UI.num_choice > 0)
		{
			UI.num_choice--;
			UI.rc_choice = RectMake(UI.rc_preview.left + 2, UI.rc_preview.bottom + 6 + 20 * (UI.num_choice), 136, 20);
			SOUNDMANAGER->play("meter");
		}
		else if (UI.num_choice == 0)
		{
			now--;
			if (now < 0) now++;
			SOUNDMANAGER->play("meter");
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && UI.num_choice <= 4)
	//if (KEYMANAGER->isStayKeyDown(VK_DOWN) && UI.num_choice <= 4)
	{
		if (UI.num_choice < 4)
		{
			UI.num_choice++;
			UI.rc_choice = RectMake(UI.rc_preview.left + 2, UI.rc_preview.bottom + 6 + 20 * (UI.num_choice), 136, 20);
			SOUNDMANAGER->play("meter");
		}
		else if (UI.num_choice == 4)
		{
			now++;
			if (UI.tileSort == SORT_TILE)
			{
				if (now > TILE_LAND_MAX - 1 - 4) now--;
				SOUNDMANAGER->play("meter");
			}
			else if (UI.tileSort == SORT_OBJ)
			{
				if (now > TILE_OBJ_MAX - 1 - 4) now--;
				SOUNDMANAGER->play("meter");
			}
			else if (UI.tileSort == SORT_ENEMY)
			{
				if (now > TILE_ENEMY_MAX - 1 - 4) now--;
				SOUNDMANAGER->play("meter");
			}
		}
	}

	//�з� ����(����Ű)
	if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		now = 0;
		if(UI.tileSort == SORT_TILE) UI.tileSort = SORT_OBJ;
		else if(UI.tileSort == SORT_OBJ) UI.tileSort = SORT_ENEMY;
		else if(UI.tileSort == SORT_ENEMY) UI.tileSort = SORT_BLOCK;
		SOUNDMANAGER->play("meter");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		now = 0;
		if (UI.tileSort == SORT_BLOCK) UI.tileSort = SORT_ENEMY;
		else if (UI.tileSort == SORT_ENEMY) UI.tileSort = SORT_OBJ;
		else if (UI.tileSort == SORT_OBJ) UI.tileSort = SORT_TILE;
		SOUNDMANAGER->play("meter");
	}

	
	//Ÿ�� ������
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON) && !UI.isMagicTool)
	{

		for (int i = 0; i < tileSize; i++)
		{
			for (int j = 0; j < tileSize; j++)
			{
				if (UI.tileSort == SORT_TILE)
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].isImg = true;
						sprintf(tile_strKey[i][j].str_img, "%s", tileMenu_land[UI.num_choice + now].str_img);
						tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
						sprintf(tile_strKey[i][j].str_img_mini, "%s",tileMenu_land[UI.num_choice + now].str_img_mini);
						tile_img[i][j].img_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_mini);
						if(!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
				else if (UI.tileSort == SORT_OBJ)
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].isImg_obj = true;
						sprintf(tile_strKey[i][j].str_img_obj, "%s", tileMenu_obj[UI.num_choice + now].str_img);
						tile_img[i][j].img_obj = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj);
						sprintf(tile_strKey[i][j].str_img_obj_mini, "%s", tileMenu_obj[UI.num_choice + now].str_img_mini);
						tile_img[i][j].img_obj_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_obj_mini);
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
				else if (UI.tileSort == SORT_ENEMY)
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].isImg_enemy = true;
						sprintf(tile_strKey[i][j].str_img_enemy, "%s", tileMenu_enemy[UI.num_choice + now].str_img);
						tile_img[i][j].img_enemy = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy);
						sprintf(tile_strKey[i][j].str_img_enemy_mini, "%s", tileMenu_enemy[UI.num_choice + now].str_img_mini);
						tile_img[i][j].img_enemy_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_enemy_mini);
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
				else if (UI.tileSort == SORT_BLOCK)
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].state_obj = BLOCK;
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
			}
		}
	}
	

	//Ÿ�� �����
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		for (int i = 0; i < tileSize; i++)
		{
			for (int j = 0; j < tileSize; j++)
			{
				if (UI.tileSort == SORT_TILE) //Ÿ�� ����
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						if (tile_strKey[i][j].floor > 0) tile_strKey[i][j].floor--;
						else if (tile_strKey[i][j].floor == 0)
						{
							tile_strKey[i][j].isImg = false;
							tile_img[i][j].img = IMAGEMANAGER->findImage("���̹���");
							tile_img[i][j].img_mini = IMAGEMANAGER->findImage("���̹���");
							if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
						}
					}
				}
				else if (UI.tileSort == SORT_OBJ) //������Ʈ ����
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].isImg_obj = false;
						tile_img[i][j].img_obj = IMAGEMANAGER->findImage("���̹���");
						tile_img[i][j].img_obj_mini = IMAGEMANAGER->findImage("���̹���");
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
				else if (UI.tileSort == SORT_ENEMY) //���ʹ� ����
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].isImg_enemy = false;
						tile_img[i][j].img_enemy = IMAGEMANAGER->findImage("���̹���");
						tile_img[i][j].img_enemy_mini = IMAGEMANAGER->findImage("���̹���");
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
				else if (UI.tileSort == SORT_BLOCK) //�̵� ���� ��� ����
				{
					if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
						(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
					{
						tile_strKey[i][j].state_obj = NONE;
						if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
					}
				}
			}
		}
	}
	
	//���� ����(4�� ���� ���� ����)
	if (KEYMANAGER->isOnceKeyDown(VK_MBUTTON))
	{
		for (int i = 0; i < tileSize; i++)
		{
			for (int j = 0; j < tileSize; j++)
			{
				if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
					(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
					(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
					(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
					tile_img[i][j].img != IMAGEMANAGER->findImage("���̹���"))
				{
					tile_strKey[i][j].floor++;
					if (!SOUNDMANAGER->isPlaySound("battle08")) SOUNDMANAGER->play("battle08");
					if(tile_strKey[i][j].floor == 5) tile_strKey[i][j].floor--;
				}
			}
		}
	}

	// ����/�ε�, �޴������, ������, �÷�����
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//���̺�, �ε� ó��
		{
			if (PtInRect(&UI.rc_save, _ptMouse))
			{
				UI.isSave = true;
				SOUNDMANAGER->play("��ư");
			}
			if (PtInRect(&UI.rc_load, _ptMouse))
			{
				UI.isLoad = true;
				SOUNDMANAGER->play("��ư");
			}

			//���̺� �˾�
			if (PtInRect(&UI.rc_popUP_yes, _ptMouse) && UI.isSave)
			{
				this->saveMap();
				SOUNDMANAGER->play("find");
				UI.isSave = false;
			}
			if (PtInRect(&UI.rc_popUP_no, _ptMouse) && UI.isSave)
			{
				UI.isSave = false;
				SOUNDMANAGER->play("��ư");
			}
			//�ε� �˾�
			if (PtInRect(&UI.rc_popUP_yes, _ptMouse) && UI.isLoad)
			{
				this->loadMap();
				SOUNDMANAGER->play("find");
				UI.isLoad = false;
			}
			if (PtInRect(&UI.rc_popUP_no, _ptMouse) && UI.isLoad)
			{
				UI.isLoad = false;
				SOUNDMANAGER->play("��ư");
			}
		}

		//�޴��� ���
		if (PtInRect(&UI.rc_manual, _ptMouse) && !UI.isManual)
		{
			UI.isManual = true;
			SOUNDMANAGER->play("��ư");
		}
		else if (PtInRect(&UI.rc_manual, _ptMouse) && UI.isManual)
		{
			UI.isManual = false;
			SOUNDMANAGER->play("��ư");
		}

		//�÷��� ��
		{
			//������ Ȱ��ȭ
			if (PtInRect(&UI.rc_magicTool, _ptMouse))
			{
				SOUNDMANAGER->play("��ư");
				if (!UI.isMagicTool) UI.isMagicTool = true;//������ �� on
				else if (UI.isMagicTool)
				{
					UI.isMagicTool = false; //������ �� off
					//��� �÷��� �� ǥ�� off
					for (int i = 0; i < tileSize; i++)
					{
						for (int j = 0; j < tileSize; j++)
						{
							tile[i][j].isFloodFill = false;
						}
					}
				}
			}

			//������ �� on �� ��
			if (UI.isMagicTool)
			{
				//�÷��� �� ����
				for (int i = 0; i < tileSize; i++)
				{
					for (int j = 0; j < tileSize; j++)
					{
						tile[i][j].isFloodFill = false;
					}
				}
				for (int i = 0; i < tileSize; i++)
				{
					for (int j = 0; j < tileSize; j++)
					{
						if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
							(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
							(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
							(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0)
						{
							this->floodFill1(&tile_img[i][j], i, j);
						}
					}
				}


				//�÷��� �� ĥ�ϱ�
				if (!UI.isReadyFloodFill)
				{
					for (int i = 0; i < tileSize; i++)
					{
						for (int j = 0; j < tileSize; j++)
						{
							if (UI.tileSort == SORT_TILE)
							{
								if ((-0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
									(0.5) * (_ptMouse.x - tile[i][j].rc.left) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
									(-0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y > 0 &&
									(0.5) * (_ptMouse.x - tile[i][j].rc.right) + tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouse.y <= 0 &&
									tile[i][j].isFloodFill)
								{
									UI.isReadyFloodFill = true;
								}
							}
						}
					}
				}
				else if (UI.isReadyFloodFill)
				{
					for (int i = 0; i < tileSize; i++)
					{
						for (int j = 0; j < tileSize; j++)
						{
							if (tile[i][j].isFloodFill)
							{
								tile_strKey[i][j].isImg = true;
								sprintf(tile_strKey[i][j].str_img, "%s", tileMenu_land[UI.num_choice + now].str_img);
								tile_img[i][j].img = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img);
								sprintf(tile_strKey[i][j].str_img_mini, "%s", tileMenu_land[UI.num_choice + now].str_img_mini);
								tile_img[i][j].img_mini = IMAGEMANAGER->findImage(tile_strKey[i][j].str_img_mini);
								if (!SOUNDMANAGER->isPlaySound("battle02")) SOUNDMANAGER->play("battle02", 0.2f);
							}
						}
					}
					UI.isReadyFloodFill = false;
				}
			}
		}
	}
}

//Ÿ�� �� ����
void mapToolScene::saveMap()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("save.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE * MAXTILESIZE, &write, NULL);
	CloseHandle(file);
}
//Ÿ�� �� �ε�
void mapToolScene::loadMap()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("save.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, tile_strKey, sizeof(tagTileStrKey) * MAXTILESIZE * MAXTILESIZE, &read, NULL);
	
	//�� Ÿ�� �ε�
	for (int i = 0; i < MAXTILESIZE; i++)
	{
		for (int j = 0; j < MAXTILESIZE; j++)
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
		}
	}

	CloseHandle(file);
}

//�޴� Ÿ�� �Ӽ� �ʱ�ȭ
void mapToolScene::initMenu_tile()
{
	//�� �̹��� �Ҵ�
	sprintf(tileMenu_land[0].str_img, "%s", "st51a_01");
	sprintf(tileMenu_land[1].str_img, "%s", "st51a_02");
	sprintf(tileMenu_land[2].str_img, "%s", "st51a_03");
	sprintf(tileMenu_land[3].str_img, "%s", "st51a_04");
	sprintf(tileMenu_land[4].str_img, "%s", "st51a_08");
	sprintf(tileMenu_land[5].str_img, "%s", "st51a_09");
	sprintf(tileMenu_land[6].str_img, "%s", "st51a_10");
	sprintf(tileMenu_land[7].str_img, "%s", "st51a_11");
	sprintf(tileMenu_land[8].str_img, "%s", "st51a_15");
	sprintf(tileMenu_land[9].str_img, "%s", "st51a_16");
	sprintf(tileMenu_land[10].str_img, "%s",  "st51a_17");
	sprintf(tileMenu_land[11].str_img, "%s",  "st51a_18");
	sprintf(tileMenu_land[12].str_img, "%s",  "st51a_22");
	sprintf(tileMenu_land[13].str_img, "%s",  "st51a_23");
	sprintf(tileMenu_land[14].str_img, "%s",  "st51a_24");
	sprintf(tileMenu_land[15].str_img, "%s",  "st51a_25");
	sprintf(tileMenu_land[16].str_img, "%s",  "st51a_30");
	sprintf(tileMenu_land[17].str_img, "%s",  "st51a_31");
	sprintf(tileMenu_land[18].str_img, "%s",  "st51a_32");
	sprintf(tileMenu_land[19].str_img, "%s",  "st51a_37");
	sprintf(tileMenu_land[20].str_img, "%s",  "st51a_38");
	sprintf(tileMenu_land[21].str_img, "%s",  "st51a_39");
	sprintf(tileMenu_land[22].str_img, "%s",  "st51a_45");
	sprintf(tileMenu_land[23].str_img, "%s",  "st51a_46");
	sprintf(tileMenu_land[24].str_img, "%s",  "st51a_52");
	sprintf(tileMenu_land[25].str_img, "%s",  "st51a_53");
	sprintf(tileMenu_land[26].str_img, "%s",  "st51a_59");
	sprintf(tileMenu_land[27].str_img, "%s",  "st51a_60");
	
	//�̴ϸ� �̹��� �Ҵ�
	sprintf(tileMenu_land[0].str_img_mini , "%s", "mini_st51a_01");
	sprintf(tileMenu_land[1].str_img_mini , "%s", "mini_st51a_02");
	sprintf(tileMenu_land[2].str_img_mini , "%s", "mini_st51a_03");
	sprintf(tileMenu_land[3].str_img_mini , "%s", "mini_st51a_04");
	sprintf(tileMenu_land[4].str_img_mini , "%s", "mini_st51a_08");
	sprintf(tileMenu_land[5].str_img_mini , "%s", "mini_st51a_09");
	sprintf(tileMenu_land[6].str_img_mini , "%s", "mini_st51a_10");
	sprintf(tileMenu_land[7].str_img_mini , "%s", "mini_st51a_11");
	sprintf(tileMenu_land[8].str_img_mini , "%s", "mini_st51a_15");
	sprintf(tileMenu_land[9].str_img_mini , "%s", "mini_st51a_16");
	sprintf(tileMenu_land[10].str_img_mini, "%s", "mini_st51a_17");
	sprintf(tileMenu_land[11].str_img_mini, "%s", "mini_st51a_18");
	sprintf(tileMenu_land[12].str_img_mini, "%s", "mini_st51a_22");
	sprintf(tileMenu_land[13].str_img_mini, "%s", "mini_st51a_23");
	sprintf(tileMenu_land[14].str_img_mini, "%s", "mini_st51a_24");
	sprintf(tileMenu_land[15].str_img_mini, "%s", "mini_st51a_25");
	sprintf(tileMenu_land[16].str_img_mini, "%s", "mini_st51a_30");
	sprintf(tileMenu_land[17].str_img_mini, "%s", "mini_st51a_31");
	sprintf(tileMenu_land[18].str_img_mini, "%s", "mini_st51a_32");
	sprintf(tileMenu_land[19].str_img_mini, "%s", "mini_st51a_37");
	sprintf(tileMenu_land[20].str_img_mini, "%s", "mini_st51a_38");
	sprintf(tileMenu_land[21].str_img_mini, "%s", "mini_st51a_39");
	sprintf(tileMenu_land[22].str_img_mini, "%s", "mini_st51a_45");
	sprintf(tileMenu_land[23].str_img_mini, "%s", "mini_st51a_46");
	sprintf(tileMenu_land[24].str_img_mini, "%s", "mini_st51a_52");
	sprintf(tileMenu_land[25].str_img_mini, "%s", "mini_st51a_53");
	sprintf(tileMenu_land[26].str_img_mini, "%s", "mini_st51a_59");
	sprintf(tileMenu_land[27].str_img_mini, "%s", "mini_st51a_60");

	//Ÿ�� ������Ʈ �Ӽ�
	for (int i = 0; i < TILE_LAND_MAX; i++) tileMenu_land[i].state_obj = NONE;
}

//�޴� ������Ʈ �Ӽ� �ʱ�ȭ
void mapToolScene::initMenu_obj()
{
	//������Ʈ �̹��� �Ҵ�
	sprintf(tileMenu_obj[0].str_img , "%s", "sign_2_04");
	sprintf(tileMenu_obj[1].str_img , "%s", "sign_2_05");
	sprintf(tileMenu_obj[2].str_img , "%s", "sign_2_06");
	sprintf(tileMenu_obj[3].str_img , "%s", "tree.64.138");
	sprintf(tileMenu_obj[4].str_img , "%s", "bush1.64.55");
	sprintf(tileMenu_obj[5].str_img , "%s", "bush2.64.45");
	sprintf(tileMenu_obj[6].str_img , "%s", "bush3.64.48");
	sprintf(tileMenu_obj[7].str_img , "%s", "bush4.64.37");
	sprintf(tileMenu_obj[8].str_img , "%s", "bush5.64.58");
	sprintf(tileMenu_obj[9].str_img , "%s", "bush6.64.48");
	sprintf(tileMenu_obj[10].str_img, "%s", "bush7.64.45");
	sprintf(tileMenu_obj[11].str_img, "%s", "bush8.64.37");
	sprintf(tileMenu_obj[12].str_img, "%s", "barrel.64.58");
	sprintf(tileMenu_obj[13].str_img, "%s", "palmtree1.440.252");
	sprintf(tileMenu_obj[14].str_img, "%s", "palmtree2.344.318");
	sprintf(tileMenu_obj[15].str_img, "%s", "st50a");
	sprintf(tileMenu_obj[16].str_img, "%s", "st50b");
	sprintf(tileMenu_obj[17].str_img, "%s", "st50c");
	sprintf(tileMenu_obj[18].str_img, "%s", "st02a");
	sprintf(tileMenu_obj[19].str_img, "%s", "chair01_1");
	sprintf(tileMenu_obj[20].str_img, "%s", "chair01_2");
	sprintf(tileMenu_obj[21].str_img, "%s", "chair02_1");
	sprintf(tileMenu_obj[22].str_img, "%s", "chair02_2");
	sprintf(tileMenu_obj[23].str_img, "%s", "chair03_1");
	sprintf(tileMenu_obj[24].str_img, "%s", "chair03_2");
	sprintf(tileMenu_obj[25].str_img, "%s", "chair04_1");
	sprintf(tileMenu_obj[26].str_img, "%s", "chair04_2");
	sprintf(tileMenu_obj[27].str_img, "%s", "table01_1");
	sprintf(tileMenu_obj[28].str_img, "%s", "table01_2");
	sprintf(tileMenu_obj[29].str_img, "%s", "table02_1");
	sprintf(tileMenu_obj[30].str_img, "%s", "table02_2");
	sprintf(tileMenu_obj[31].str_img, "%s", "table03_1");
	sprintf(tileMenu_obj[32].str_img, "%s", "table04_1");
	sprintf(tileMenu_obj[33].str_img, "%s", "table05_1");
	sprintf(tileMenu_obj[34].str_img, "%s", "chair05");
	sprintf(tileMenu_obj[35].str_img, "%s", "chair06");
	sprintf(tileMenu_obj[36].str_img, "%s", "chair07");
	sprintf(tileMenu_obj[37].str_img, "%s", "chair08");
	sprintf(tileMenu_obj[38].str_img, "%s", "chair09");
	sprintf(tileMenu_obj[39].str_img, "%s", "chair10");
	sprintf(tileMenu_obj[40].str_img, "%s", "chair11");
	sprintf(tileMenu_obj[41].str_img, "%s", "chair12");
	sprintf(tileMenu_obj[42].str_img, "%s", "counter1");
	sprintf(tileMenu_obj[43].str_img, "%s", "counter2");
	sprintf(tileMenu_obj[44].str_img, "%s", "counter3");
	sprintf(tileMenu_obj[45].str_img, "%s", "counter4");
	sprintf(tileMenu_obj[46].str_img, "%s", "counter5");
	sprintf(tileMenu_obj[47].str_img, "%s", "counter6");
	sprintf(tileMenu_obj[48].str_img, "%s", "counter7");
	sprintf(tileMenu_obj[49].str_img, "%s", "counter8");
	sprintf(tileMenu_obj[50].str_img, "%s", "counter9");
	sprintf(tileMenu_obj[51].str_img, "%s", "counter10");
	sprintf(tileMenu_obj[52].str_img, "%s", "st44a1");
	

	//�̴ϸ� �̹��� �Ҵ�
	sprintf(tileMenu_obj[0].str_img_mini , "%s", "mini_sign_2_04");
	sprintf(tileMenu_obj[1].str_img_mini , "%s", "mini_sign_2_05");
	sprintf(tileMenu_obj[2].str_img_mini , "%s", "mini_sign_2_06");
	sprintf(tileMenu_obj[3].str_img_mini , "%s", "mini_tree.64.138");
	sprintf(tileMenu_obj[4].str_img_mini , "%s", "mini_bush1.64.55");
	sprintf(tileMenu_obj[5].str_img_mini , "%s", "mini_bush2.64.45");
	sprintf(tileMenu_obj[6].str_img_mini , "%s", "mini_bush3.64.48");
	sprintf(tileMenu_obj[7].str_img_mini , "%s", "mini_bush4.64.37");
	sprintf(tileMenu_obj[8].str_img_mini , "%s", "mini_bush5.64.58");
	sprintf(tileMenu_obj[9].str_img_mini , "%s", "mini_bush6.64.48");
	sprintf(tileMenu_obj[10].str_img_mini, "%s", "mini_bush7.64.45");
	sprintf(tileMenu_obj[11].str_img_mini, "%s", "mini_bush8.64.37");
	sprintf(tileMenu_obj[12].str_img_mini, "%s", "mini_barrel.64.58");
	sprintf(tileMenu_obj[13].str_img_mini, "%s", "mini_palmtree1.440.252");
	sprintf(tileMenu_obj[14].str_img_mini, "%s", "mini_palmtree2.344.318");
	sprintf(tileMenu_obj[15].str_img_mini, "%s", "mini_st50a");
	sprintf(tileMenu_obj[16].str_img_mini, "%s", "mini_st50b");
	sprintf(tileMenu_obj[17].str_img_mini, "%s", "mini_st50c");
	sprintf(tileMenu_obj[18].str_img_mini, "%s", "mini_st02a");
	sprintf(tileMenu_obj[19].str_img_mini, "%s", "mini_chair01_1");
	sprintf(tileMenu_obj[20].str_img_mini, "%s", "mini_chair01_2");
	sprintf(tileMenu_obj[21].str_img_mini, "%s", "mini_chair02_1");
	sprintf(tileMenu_obj[22].str_img_mini, "%s", "mini_chair02_2");
	sprintf(tileMenu_obj[23].str_img_mini, "%s", "mini_chair03_1");
	sprintf(tileMenu_obj[24].str_img_mini, "%s", "mini_chair03_2");
	sprintf(tileMenu_obj[25].str_img_mini, "%s", "mini_chair04_1");
	sprintf(tileMenu_obj[26].str_img_mini, "%s", "mini_chair04_2");
	sprintf(tileMenu_obj[27].str_img_mini, "%s", "mini_table01_1");
	sprintf(tileMenu_obj[28].str_img_mini, "%s", "mini_table01_2");
	sprintf(tileMenu_obj[29].str_img_mini, "%s", "mini_table02_1");
	sprintf(tileMenu_obj[30].str_img_mini, "%s", "mini_table02_2");
	sprintf(tileMenu_obj[31].str_img_mini, "%s", "mini_table03_1");
	sprintf(tileMenu_obj[32].str_img_mini, "%s", "mini_table04_1");
	sprintf(tileMenu_obj[33].str_img_mini, "%s", "mini_table05_1");
	sprintf(tileMenu_obj[34].str_img_mini, "%s", "mini_chair05");
	sprintf(tileMenu_obj[35].str_img_mini, "%s", "mini_chair06");
	sprintf(tileMenu_obj[36].str_img_mini, "%s", "mini_chair07");
	sprintf(tileMenu_obj[37].str_img_mini, "%s", "mini_chair08");
	sprintf(tileMenu_obj[38].str_img_mini, "%s", "mini_chair09");
	sprintf(tileMenu_obj[39].str_img_mini, "%s", "mini_chair10");
	sprintf(tileMenu_obj[40].str_img_mini, "%s", "mini_chair11");
	sprintf(tileMenu_obj[41].str_img_mini, "%s", "mini_chair12");
	sprintf(tileMenu_obj[42].str_img_mini, "%s", "mini_counter1");
	sprintf(tileMenu_obj[43].str_img_mini, "%s", "mini_counter2");
	sprintf(tileMenu_obj[44].str_img_mini, "%s", "mini_counter3");
	sprintf(tileMenu_obj[45].str_img_mini, "%s", "mini_counter4");
	sprintf(tileMenu_obj[46].str_img_mini, "%s", "mini_counter5");
	sprintf(tileMenu_obj[47].str_img_mini, "%s", "mini_counter6");
	sprintf(tileMenu_obj[48].str_img_mini, "%s", "mini_counter7");
	sprintf(tileMenu_obj[49].str_img_mini, "%s", "mini_counter8");
	sprintf(tileMenu_obj[50].str_img_mini, "%s", "mini_counter9");
	sprintf(tileMenu_obj[51].str_img_mini, "%s", "mini_counter10");
	sprintf(tileMenu_obj[52].str_img_mini, "%s", "mini_st44a1");
	

}

//�޴� ���ʹ� �Ӽ� �ʱ�ȭ
void mapToolScene::initMenu_enemy()
{
	//������Ʈ �̹��� �Ҵ�
	sprintf(tileMenu_enemy[0].str_img, "%s", "char_15e");
	sprintf(tileMenu_enemy[1].str_img, "%s", "necro1");
	sprintf(tileMenu_enemy[2].str_img, "%s", "char_33a");
	sprintf(tileMenu_enemy[3].str_img, "%s", "char_68e");
	sprintf(tileMenu_enemy[4].str_img, "%s", "char_15e");

	//�̴ϸ� �̹��� �Ҵ�
	sprintf(tileMenu_enemy[0].str_img_mini, "%s", "mini_char_15e");
	sprintf(tileMenu_enemy[1].str_img_mini, "%s", "mini_necro1");
	sprintf(tileMenu_enemy[2].str_img_mini, "%s", "mini_char_33a");
	sprintf(tileMenu_enemy[3].str_img_mini, "%s", "mini_char_68e");
	sprintf(tileMenu_enemy[4].str_img_mini, "%s", "mini_char_15e");
}


//�÷��� ��
void mapToolScene::floodFill1(tagTileImg* initTile, int i, int j)
{
	if (tile_img[i][j].img == initTile->img)
	{
		tile[i][j].isFloodFill = true;
		
		//if (tile[i][j + 1].isFloodFill) return;
		//else if (j + 1 > tileSize) return;
		if (j + 1 <= tileSize) { floodFill1(initTile, i, j + 1); }
		
		//if (tile[i + 1][j].isFloodFill) return;
		//else if (i + 1 > tileSize) return;
		if (i + 1 <= tileSize) { floodFill1(initTile, i + 1, j); }
		floodFill2(initTile, i, j);
	}
}

void mapToolScene::floodFill2(tagTileImg* initTile, int i, int j)
{
	if (tile_img[i][j].img == initTile->img)
	{
		tile[i][j].isFloodFill = true;
		
		//if (tile[i][j - 1].isFloodFill) return;
		//else if (j - 1 < 0) return;
		if (j - 1 >= 0) { floodFill2(initTile, i, j - 1); }
		
		//if (tile[i - 1][j].isFloodFill) return;
		//else if (i - 1 < 0) return;
		if (i - 1 >= 0) { floodFill2(initTile, i - 1, j); }
	}
}