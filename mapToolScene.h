#pragma once
#include "gameNode.h"
#include "tileNode.h"

#define TILE_LAND_MAX 28 //Ÿ�� ���� �ִ� ����
#define TILE_OBJ_MAX 52 + 1 //������Ʈ ���� �ִ� ����
#define TILE_ENEMY_MAX 5 //���ʹ� ���� �ִ� ����
#define TILE_BLOCK_MAX 2 //��� ���� �ִ� ����
#define MAXTILESIZE MAXTILESIZE_NORMAL //�ִ� Ÿ�� ����


//�޴�â �� Ÿ�� �Ӽ�(�ʵ� Ÿ�Ϸ� �������� �̹��� �� ������)
struct tagTileMenu
{
	char str_img[128];
	char str_img_mini[128];
	state_OBJECT state_obj;
};

//�޴�â
struct tagMenuUI
{
	POINT mark;
	RECT rc_bg;
	image* img_bg;
	RECT rc_miniMap;
	RECT rc_miniMap_show;
	RECT rc_save;
	RECT rc_load;
	RECT rc_preview;
	RECT rc_listBG;
	int num_choice;
	RECT rc_choice; //���� ���� Ÿ���� ���� �����ִ� GUI
	RECT rc_sortBG;
	RECT rc_tile;
	RECT rc_obj;
	RECT rc_enemy;
	RECT rc_block;
	state_TileSort tileSort;//Ÿ�� �з�

	RECT rc_popUP_BG;
	RECT rc_popUP_yes;
	RECT rc_popUP_no;
	bool isSave;
	bool isLoad;

	RECT rc_magicTool;
	bool isMagicTool;
	bool isReadyFloodFill;

	RECT rc_manual;
	bool isManual;

	tagMenuUI()
	{
		mark.x = 10;
		mark.y = 10;
		rc_bg = RectMake(mark.x, mark.y, 150, 460);
		img_bg = IMAGEMANAGER->addImage("menu_bg", "IMAGE/menu_bg.bmp", 150, 460);
		rc_miniMap = RectMake(rc_bg.left + 5, rc_bg.top + 5, 140, 110);
		rc_miniMap_show = RectMake(rc_bg.left + 36, rc_bg.left + 23, 68, 46);
		rc_save = RectMake(rc_miniMap.left, rc_miniMap.bottom + 4, 68, 30);
		rc_load = RectMake(rc_miniMap.left + 68 + 4, rc_miniMap.bottom + 4, 68, 30);
		rc_preview = RectMake(rc_miniMap.left, rc_save.bottom + 4, 140, 110);
		rc_listBG = RectMake(rc_miniMap.left, rc_preview.bottom + 4, 140, 104);
		num_choice = 0;
		rc_choice = RectMake(rc_preview.left + 2, rc_preview.bottom + 6 + 20 * (num_choice), 136, 20);
		rc_sortBG = RectMake(rc_listBG.left, rc_listBG.bottom + 1, 140, 20);
		rc_tile = RectMake(rc_listBG.left + 1, rc_listBG.bottom + 2, 24, 18);
		rc_obj = RectMake(rc_tile.right + 2, rc_listBG.bottom + 2, 24, 18);
		rc_enemy = RectMake(rc_obj.right + 2, rc_listBG.bottom + 2, 46, 18);
		rc_block = RectMake(rc_enemy.right + 2, rc_listBG.bottom + 2, 38, 18);
		tileSort = SORT_TILE;

		rc_popUP_BG = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 300, 160);
		rc_popUP_yes = RectMake(rc_popUP_BG.left + 60, rc_popUP_BG.top + 110, 68, 30);
		rc_popUP_no = RectMake(rc_popUP_BG.right - 60 - 68, rc_popUP_BG.top + 110, 68, 30);
		isSave = false;
		isLoad = false;

		rc_magicTool = RectMake(rc_sortBG.left, rc_sortBG.bottom + 4, 140, 20);
		isMagicTool = false;
		isReadyFloodFill = false;

		rc_manual = RectMake(560, 460, WINSIZEX - 560, WINSIZEY - 460);
		isManual = false;
	}
};


class mapToolScene : public gameNode
{
private:
	tagMenuUI UI;//�޴�â UI

	POINT init_pos;

	RECT rc_clipping;//Ŭ���� ��Ʈ
	RECT rc_clipping_mini;//�̴ϸ� Ŭ���� ��Ʈ
	RECT rc_temp; //�� ��Ʈ
	int tileSize; //Ÿ�� ũ��
	tagTile tile[MAXTILESIZE][MAXTILESIZE];//��ü Ÿ�� ����ü
	tagTileStrKey tile_strKey[MAXTILESIZE][MAXTILESIZE];//��ü Ÿ�� ����ü
	tagTileImg tile_img[MAXTILESIZE][MAXTILESIZE];//��ü Ÿ�� �̹��� ����ü
	POINT pt_tileMark;//��ü Ÿ�� ���� ������
	float pt_tileMark_mini_x;//��ü Ÿ�� ���� ������
	float pt_tileMark_mini_y;//��ü Ÿ�� ���� ������

	//�޴� Ÿ��(��)
	char str_tileMenu_land[5][128];
	tagTileMenu tileMenu_land[TILE_LAND_MAX];
	//�޴� Ÿ��(������Ʈ)
	char str_tileMenu_obj[5][128];
	tagTileMenu tileMenu_obj[TILE_OBJ_MAX];
	//�޴� Ÿ��(������Ʈ)
	char str_tileMenu_enemy[5][128];
	tagTileMenu tileMenu_enemy[TILE_ENEMY_MAX];

	int now;
	
	float _startTime;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void drowDiamond(tagTile tile, int iStyle, COLORREF color);
	void drowRect(RECT rc, COLORREF color);
	void drowTileIndex(tagTile tile, int i, int j);

	void userControl();
	void saveMap();
	void loadMap();

	//Ÿ�� �޴� �ʱ�ȭ
	void initMenu_tile();
	void initMenu_obj();
	void initMenu_enemy();

	//�÷��� �� 
	void floodFill1(tagTileImg* initTile, int i, int j);
	void floodFill2(tagTileImg* initTile, int i, int j);

	mapToolScene() {}
	~mapToolScene() {}
};

