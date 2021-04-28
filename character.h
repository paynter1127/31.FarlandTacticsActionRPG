#pragma once

#define ENEMYNUM 2

//npc 구조체
struct tagNPC
{
	int x, y;
	image* img;
	bool isTalk;
};


enum CHARACTERSTATE { IDLE, BEHIT, RUN, ATTACK, SKILL1, SKILL2, SKILL3, DIE };	//캐릭터 상태
enum tagDirection2 { DOWN, UP };	//캐릭터 이동 방향
enum tagDirection1 { LEFT, RIGHT };	//캐릭터 이동 방향

//캐릭터 이미지
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

//캐릭터 스텟
struct tagStat
{
	int hpMax;
	int hp;
	int att;
};

//캐릭터 구조체
struct tagCharacter
{
	POINT init_pos;	//캐릭터 초기 타일 인덱스
	RECT rc;	//캐릭터 rc
	float x, y;	//z캐릭터 x, y 좌표
	float runSpeed;
	INDEX index;
	CHARACTERSTATE state;	//캐릭터 상태
	tagDirection1 direc_LR;	//캐릭터 이동 방향
	tagDirection2 direc_DU;	//캐릭터 이동 방향

	tagCharacterFrameCount count; //캐릭터 카운트
	tagStat stat;

	//A* 데이터
	node* _startNode;					//시작노드
	node* _endNode;						//종료노드
	node* _curNode;						//현재노드

	vector<node*> _openList;			//오픈리스트(탐색할 노드들을 담아둘 벡터)
	vector<node*> _closeList;			//클로즈리스트 (오픈리스트의 최단거리 노드를 삭제후 클로즈리스트 벡터에 담는다)
	vector<node*> _finalList;			//파이널리스트 (길찾은 후 클로즈리스트에 담겨있는 노드들을 리버스시킬용도)

	//임시 end 노드
	node* temp_endNode;
	bool isNodePick;
	int nodeNum;
	bool _isFind;						//길 찾았냐?

	tagCharacter() : _startNode(NULL), _endNode(NULL)
	{
		isNodePick = false;	//A* 노드 선택 여부
		nodeNum = 0;		//A* 노드 수
		_isFind = false; //길찾았냐?
	}
};

