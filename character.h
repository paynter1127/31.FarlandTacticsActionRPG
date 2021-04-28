#pragma once

#define ENEMYNUM 2

//npc ����ü
struct tagNPC
{
	int x, y;
	image* img;
	bool isTalk;
};


enum CHARACTERSTATE { IDLE, BEHIT, RUN, ATTACK, SKILL1, SKILL2, SKILL3, DIE };	//ĳ���� ����
enum tagDirection2 { DOWN, UP };	//ĳ���� �̵� ����
enum tagDirection1 { LEFT, RIGHT };	//ĳ���� �̵� ����

//ĳ���� �̹���
struct tagCharacterFrameCount
{
	int base;
	int idle;
	int run1;
	int run2;
	int run3;
	int run4;
	int attack1;
	int attack2;
	int attack3;
	int attack4;
	tagCharacterFrameCount()
	{
		base = 0;
		idle = 0;
		run1 = 0;
		run2 = 0;
		run3 = 0;
		run4 = 0;
		attack1 = 0;
		attack2 = 0;
		attack3 = 0;
		attack4 = 0;
	}
};

//ĳ���� ����
struct tagStat
{
	int hpMax;
	int hp;
	int att;
};

//ĳ���� ����ü
struct tagCharacter
{
	POINT init_pos;	//ĳ���� �ʱ� Ÿ�� �ε���
	RECT rc;	//ĳ���� rc
	float x, y;	//zĳ���� x, y ��ǥ
	float runSpeed;
	INDEX index;
	CHARACTERSTATE state;	//ĳ���� ����
	tagDirection1 direc_LR;	//ĳ���� �̵� ����
	tagDirection2 direc_DU;	//ĳ���� �̵� ����

	tagCharacterFrameCount count; //ĳ���� ī��Ʈ
	tagStat stat;

	//A* ������
	node* _startNode;					//���۳��
	node* _endNode;						//������
	node* _curNode;						//������

	vector<node*> _openList;			//���¸���Ʈ(Ž���� ������ ��Ƶ� ����)
	vector<node*> _closeList;			//Ŭ�����Ʈ (���¸���Ʈ�� �ִܰŸ� ��带 ������ Ŭ�����Ʈ ���Ϳ� ��´�)
	vector<node*> _finalList;			//���̳θ���Ʈ (��ã�� �� Ŭ�����Ʈ�� ����ִ� ������ ��������ų�뵵)

	//�ӽ� end ���
	node* temp_endNode;
	bool isNodePick;
	int nodeNum;
	bool _isFind;						//�� ã�ҳ�?

	tagCharacter() : _startNode(NULL), _endNode(NULL)
	{
		isNodePick = false;	//A* ��� ���� ����
		nodeNum = 0;		//A* ��� ��
		_isFind = false; //��ã�ҳ�?
	}
};

