#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "character.h"
#include "progressBar.h"

#define TILE_LAND_MAX 28 //Ÿ�� ���� �ִ� ����
#define TILE_OBJ_MAX 18 //������Ʈ ���� �ִ� ����
#define TILE_ENEMY_MAX 5 //���ʹ� ���� �ִ� ����
#define TILE_BLOCK_MAX 2 //��� ���� �ִ� ����

#define NPCSIZE 4 //NPC ��



class townScene : public gameNode
{
private:
	// < ĳ���� ���� ���� > -----------------------------------------------------------------------------------------------------
	POINT _ptMouseC;	//ĳ���� ���� ķ ���� ���콺
	POINT init_pos;	//ĳ���� �ʱ� Ÿ�� �ε���
	RECT rc;	//ĳ���� rc
	int count;	//������ ī��Ʈ
	int x, y;	//zĳ���� x, y ��ǥ
	int runSpeed;
	INDEX index;
	CHARACTERSTATE state;	//ĳ���� ����
	tagDirection1 direc_LR;	//ĳ���� �̵� ����
	tagDirection2 direc_DU;	//ĳ���� �̵� ����
	int hp;
	int hpMax;
	progressBar* hpBar;

	// < npc ���� ���� > -----------------------------------------------------------------------------------------------------
	tagNPC npc[NPCSIZE];
	tagNPC sign;



	// < Ÿ�� ���� ���� ���� > -----------------------------------------------------------------------------------------------------
	image* img_bg;	//��� Ÿ�� �̹���
	RECT rc_clipping;//Ŭ���� ��Ʈ
	RECT rc_temp; //�� ��Ʈ
	int tileSize; //Ÿ�� ũ��
	POINT pt_tileMark;//��ü Ÿ�� ���� ������
	tagTile tile[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//��ü Ÿ�� ����ü
	tagTileStrKey tile_strKey[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//��ü Ÿ�� Ű�� ����ü
	tagTileImg tile_img[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//��ü Ÿ�� �̹��� ����ü


	// < ����ȭ ���� ���� ���� > -----------------------------------------------------------------------------------------------------
	//for�� ���� ����(ȭ��)
	int x_for_start;
	int x_for_end;
	int y_for_start;
	int y_for_end;

	//for�� ���� ����(z-order)
	int x_for_start_z;
	int x_for_end_z;
	int y_for_start_z;
	int y_for_end_z;


	// < A* ���� ���� > -----------------------------------------------------------------------------------------------------
	node* _totalNode[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];		//��ü��� 25��(������ ����)
	node* _startNode;					//���۳��
	node* _endNode;						//������
	node* _curNode;						//������

	vector<node*> _openList;			//���¸���Ʈ(Ž���� ������ ��Ƶ� ����)
	vector<node*> _closeList;			//Ŭ�����Ʈ (���¸���Ʈ�� �ִܰŸ� ��带 ������ Ŭ�����Ʈ ���Ϳ� ��´�)
	vector<node*> _finalList;			//���̳θ���Ʈ (��ã�� �� Ŭ�����Ʈ�� ����ִ� ������ ��������ų�뵵)

	int _count;							//����, ������ �ѹ����� �����ϱ� ���� ����
	bool _isFind;						//�� ã�ҳ�?

	//�ӽ� end ���
	node* temp_endNode;
	bool isNodePick;
	int nodeNum;


	float _startTime;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void drawDiamond(tagTile tile, int iStyle, COLORREF color);
	void drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile);
	void drawTileIndex(tagTile tile, int i, int j);
	void drawTileIndex(tagTile tile, int i, int j, tagTile firstTile);

	void userControl();

	//void drawNodeDiamond(node * node, int iStyle, COLORREF color);
	//void drawNodeDiamond(node * node, int iStyle, COLORREF color, tagTile firstTile);

	//��� ��ä���(���������� �����Լ�)
	void setNodeColor(tagTile tile, COLORREF color);
	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding();
	//���¸���Ʈ�� �߰� �Լ�
	void addOpenList(int idx, int idy);
	//���¸���Ʈ ����
	void delOpenList(int index);

	townScene() : _startNode(NULL), _endNode(NULL) {};
	~townScene() {};
};

