#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "character.h"
#include "progressBar.h"

#define TILE_LAND_MAX 28 //타일 종류 최대 갯수
#define TILE_OBJ_MAX 18 //오브젝트 종류 최대 갯수
#define TILE_ENEMY_MAX 5 //에너미 종류 최대 갯수
#define TILE_BLOCK_MAX 2 //블록 종류 최대 갯수

#define NPCSIZE 4 //NPC 수



class townScene : public gameNode
{
private:
	// < 캐릭터 변수 선언 > -----------------------------------------------------------------------------------------------------
	POINT _ptMouseC;	//캐릭터 랜더 캠 기준 마우스
	POINT init_pos;	//캐릭터 초기 타일 인덱스
	RECT rc;	//캐릭터 rc
	int count;	//프레임 카운트
	int x, y;	//z캐릭터 x, y 좌표
	int runSpeed;
	INDEX index;
	CHARACTERSTATE state;	//캐릭터 상태
	tagDirection1 direc_LR;	//캐릭터 이동 방향
	tagDirection2 direc_DU;	//캐릭터 이동 방향
	int hp;
	int hpMax;
	progressBar* hpBar;

	// < npc 변수 선언 > -----------------------------------------------------------------------------------------------------
	tagNPC npc[NPCSIZE];
	tagNPC sign;



	// < 타일 관련 변수 선언 > -----------------------------------------------------------------------------------------------------
	image* img_bg;	//배경 타일 이미지
	RECT rc_clipping;//클리핑 렉트
	RECT rc_temp; //빈 렉트
	int tileSize; //타일 크기
	POINT pt_tileMark;//전체 타일 종속 기준점
	tagTile tile[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//전체 타일 구조체
	tagTileStrKey tile_strKey[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//전체 타일 키값 구조체
	tagTileImg tile_img[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];//전체 타일 이미지 구조체


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


	// < A* 변수 선언 > -----------------------------------------------------------------------------------------------------
	node* _totalNode[MAXTILESIZE_TOWN][MAXTILESIZE_TOWN];		//전체노드 25개(보드판 역할)
	node* _startNode;					//시작노드
	node* _endNode;						//종료노드
	node* _curNode;						//현재노드

	vector<node*> _openList;			//오픈리스트(탐색할 노드들을 담아둘 벡터)
	vector<node*> _closeList;			//클로즈리스트 (오픈리스트의 최단거리 노드를 삭제후 클로즈리스트 벡터에 담는다)
	vector<node*> _finalList;			//파이널리스트 (길찾은 후 클로즈리스트에 담겨있는 노드들을 리버스시킬용도)

	int _count;							//시작, 종료노드 한번씩만 선택하기 위한 변수
	bool _isFind;						//길 찾았냐?

	//임시 end 노드
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

	//노드 색채우기(편리성때문에 만든함수)
	void setNodeColor(tagTile tile, COLORREF color);
	/*중요함수*/
	//길찾기 함수
	void pathFinding();
	//오픈리스트에 추가 함수
	void addOpenList(int idx, int idy);
	//오픈리스트 삭제
	void delOpenList(int index);

	townScene() : _startNode(NULL), _endNode(NULL) {};
	~townScene() {};
};

