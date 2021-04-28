#pragma once
#include "gameNode.h"
#include "ai_N.h"
#include "skill.h"
#include "progressBar.h"





class fieldScene : public gameNode
{
private:
	int world_count; //���� ������ ī��Ʈ
	POINT _ptMouseC;	//ĳ���� ���� ķ ���� ���콺(�÷��̾ �ʿ�)
	progressBar* hpBar;
	tagSkillControl ctrQ;
	tagSkillControl ctrW;
	int shackX;
	int shackY;
	bool sightOn;

	// < ������ ����(enemy, player, node, tile) ���� ���� > -----------------------------------------------------------------------------------------------------
	receivedData1* _data; //�÷��̾�, Ÿ��, ��� ���� ������

	tagCharacter* _enemy; //���ʹ� ���̽� ������
	progressBar* _enemy_hpBar; //ü�¹� ���̽� ������
	vector<tagCharacter> _vEnemy; //���ʹ� + ü�¹�
	vector<progressBar> _vEnemy_hpBar; //���ʹ� + ü�¹�


	// < BT ���� ���� > -----------------------------------------------------------------------------------------------------
	Sequence* _root[ENEMYNUM];
	Sequence* _sequence1[ENEMYNUM];
	Selector* _selector1[ENEMYNUM];
	Approach* _approach[ENEMYNUM];
	Attack* _attack[ENEMYNUM];


	// < Ÿ�� ���� ���� ���� > -----------------------------------------------------------------------------------------------------
	//image* img_bg;	//��� Ÿ�� �̹���
	RECT rc_clipping;//Ŭ���� ��Ʈ
	RECT rc_temp; //�� ��Ʈ
	tagTileStrKey tile_strKey[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//��ü Ÿ�� Ű�� ����ü
	tagTileImg tile_img[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//��ü Ÿ�� �̹��� ����ü


	// < ??? ���� ���� > -----------------------------------------------------------------------------------------------------
	RECT rc_test;


	// < npc ���� ���� > -----------------------------------------------------------------------------------------------------
	tagNPC npc;//11.10
	image* npc_img;
	tagNPC sign[3];//41,45 / 24,14 / 18,8



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

	float _startTime;
public:
	HRESULT init();
	void release();
	void update();
	void render();


	//���� ��ų
	void userSkill();

	//z���� üŷ �Լ�
	void checkZorder(int i, int j, int dept);

	void drawDiamond(tagTile tile, int iStyle, COLORREF color);
	void drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile);
	void drawTileIndex(tagTile tile, int i, int j);
	void drawTileIndex(tagTile tile, int i, int j, tagTile firstTile);


	//A* �Լ�
	void A(tagCharacter* character);

	//��� ��ä���(���������� �����Լ�)
	void setNodeColor(tagTile tile, COLORREF color);
	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding(tagCharacter* character);
	//���¸���Ʈ�� �߰� �Լ�
	void addOpenList(int idx, int idy, tagCharacter* character);
	//���¸���Ʈ ����
	void delOpenList(int index, tagCharacter* character);

	//�ֺ� ã��
	INDEX findEmptyIndex(tagCharacter* player, tagCharacter* enemy);

	void move(tagCharacter* character);
	void talk();

	fieldScene() {};
	~fieldScene() {};
};

