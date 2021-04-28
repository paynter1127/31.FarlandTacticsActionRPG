#pragma once
#include "gameNode.h"
#include "ai_N.h"
#include "skill.h"
#include "progressBar.h"


class bossScene : public gameNode
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

	tagCharacter* _boss; //���� ���̽� ������
	progressBar* _boss_hpBar; //ü�¹� ���̽� ������

	tagCharacter* _enemy; //���ʹ� ���̽� ������
	progressBar* _enemy_hpBar; //ü�¹� ���̽� ������
	vector<tagCharacter> _vEnemy; //���ʹ� + ü�¹�
	vector<progressBar> _vEnemy_hpBar; //���ʹ� + ü�¹�


	// < BT ���� ���� > -----------------------------------------------------------------------------------------------------
	Sequence* _root[128];
	Sequence* _sequence1[128];
	Selector* _selector1[128];
	Approach* _approach[128];
	Attack* _attack[128];



	//����
	Sequence* _root_boss;
	Selector* _selector1_boss;
	isDie* _isDie_boss;
	Selector* _selector2_boss;
	
	//1������
	Sequence* _sequence100_boss; 
	checkPhase* _checkPhase;
	Sequence* _sequence101_boss;
	
	//2������
	Sequence* _sequence200_boss; 
	Sequence* _sequence201_boss;

	checkRange* _checkRange;
	skill_fire* _skill_fire;
	skill_thunder* _skill_thunder;
	skill_summon* _skill_summon;
	Approach* _approach_boss;
	Attack* _attack_boss;


	//���� ��ų ����
	bool isThunder[7];
	INDEX thunderIndex;
	bool isFire[5];
	INDEX fireIndex;
	bool isSummon;
	INDEX summonIndex;


	// < Ÿ�� ���� ���� ���� > -----------------------------------------------------------------------------------------------------
	image* img_bg;	//��� Ÿ�� �̹���
	RECT rc_clipping;//Ŭ���� ��Ʈ
	RECT rc_temp; //�� ��Ʈ
	tagTileStrKey tile_strKey[MAXTILESIZE_BOSS][MAXTILESIZE_BOSS];//��ü Ÿ�� Ű�� ����ü
	tagTileImg tile_img[MAXTILESIZE_BOSS][MAXTILESIZE_BOSS];//��ü Ÿ�� �̹��� ����ü


	// < ??? ���� ���� > -----------------------------------------------------------------------------------------------------
	RECT rc_test;



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


	bossScene() {};
	~bossScene() {};
};

