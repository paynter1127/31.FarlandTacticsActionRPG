#pragma once
#include "gameNode.h"
#include "ai_N.h"
#include "skill.h"
#include "progressBar.h"


class bossScene : public gameNode
{
private:
	int world_count; //월드 프레임 카운트
	POINT _ptMouseC;	//캐릭터 랜더 캠 기준 마우스(플레이어만 필요)
	progressBar* hpBar;
	tagSkillControl ctrQ;
	tagSkillControl ctrW;
	int shackX;
	int shackY;
	bool sightOn;

	// < 데이터 선언(enemy, player, node, tile) 변수 선언 > -----------------------------------------------------------------------------------------------------
	receivedData1* _data; //플레이어, 타일, 노드 정보 데이터

	tagCharacter* _boss; //보스 베이스 데이터
	progressBar* _boss_hpBar; //체력바 베이스 데이터

	tagCharacter* _enemy; //에너미 베이스 데이터
	progressBar* _enemy_hpBar; //체력바 베이스 데이터
	vector<tagCharacter> _vEnemy; //에너미 + 체력바
	vector<progressBar> _vEnemy_hpBar; //에너미 + 체력바


	// < BT 변수 선언 > -----------------------------------------------------------------------------------------------------
	Sequence* _root[128];
	Sequence* _sequence1[128];
	Selector* _selector1[128];
	Approach* _approach[128];
	Attack* _attack[128];



	//보스
	Sequence* _root_boss;
	Selector* _selector1_boss;
	isDie* _isDie_boss;
	Selector* _selector2_boss;
	
	//1페이즈
	Sequence* _sequence100_boss; 
	checkPhase* _checkPhase;
	Sequence* _sequence101_boss;
	
	//2페이즈
	Sequence* _sequence200_boss; 
	Sequence* _sequence201_boss;

	checkRange* _checkRange;
	skill_fire* _skill_fire;
	skill_thunder* _skill_thunder;
	skill_summon* _skill_summon;
	Approach* _approach_boss;
	Attack* _attack_boss;


	//보스 스킬 선언
	bool isThunder[7];
	INDEX thunderIndex;
	bool isFire[5];
	INDEX fireIndex;
	bool isSummon;
	INDEX summonIndex;


	// < 타일 관련 변수 선언 > -----------------------------------------------------------------------------------------------------
	image* img_bg;	//배경 타일 이미지
	RECT rc_clipping;//클리핑 렉트
	RECT rc_temp; //빈 렉트
	tagTileStrKey tile_strKey[MAXTILESIZE_BOSS][MAXTILESIZE_BOSS];//전체 타일 키값 구조체
	tagTileImg tile_img[MAXTILESIZE_BOSS][MAXTILESIZE_BOSS];//전체 타일 이미지 구조체


	// < ??? 변수 선언 > -----------------------------------------------------------------------------------------------------
	RECT rc_test;



	// < 최적화 관련 변수 선언 > -----------------------------------------------------------------------------------------------------
	//for문 제한 인자(화면)
	int x_for_start;
	int x_for_end;
	int y_for_start;
	int y_for_end;

	//for문 제한 인자(z-order)
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

	//공격 스킬
	void userSkill();

	//z오더 체킹 함수
	void checkZorder(int i, int j, int dept);

	void drawDiamond(tagTile tile, int iStyle, COLORREF color);
	void drawDiamond(tagTile tile, int iStyle, COLORREF color, tagTile firstTile);
	void drawTileIndex(tagTile tile, int i, int j);
	void drawTileIndex(tagTile tile, int i, int j, tagTile firstTile);


	//A* 함수
	void A(tagCharacter* character);

	//노드 색채우기(편리성때문에 만든함수)
	void setNodeColor(tagTile tile, COLORREF color);
	/*중요함수*/
	//길찾기 함수
	void pathFinding(tagCharacter* character);
	//오픈리스트에 추가 함수
	void addOpenList(int idx, int idy, tagCharacter* character);
	//오픈리스트 삭제
	void delOpenList(int index, tagCharacter* character);

	//주변 찾기
	INDEX findEmptyIndex(tagCharacter* player, tagCharacter* enemy);

	void move(tagCharacter* character);


	bossScene() {};
	~bossScene() {};
};

