#pragma once

//코드 줄이기 define
#define XX (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].x - _data->player.x + shackX)
#define YY (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].y - _data->player.y + shackY)
#define PICKING (-0.5) * (_ptMouseC.x - _data->tile[i][j].rc.left) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 && (0.5) * (_ptMouseC.x - _data->tile[i][j].rc.left) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 && (-0.5) * (_ptMouseC.x - _data->tile[i][j].rc.right) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 && (0.5) * (_ptMouseC.x - _data->tile[i][j].rc.right) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0

//타일 기본 속정 정의
#define TILEWIDTH 64
#define TILEHEIGHT 32

#define MAXTILESIZE_TOWN 21 //최대 타일 갯수(타운)
#define MAXTILESIZE_NORMAL 50 //최대 타일 갯수(노말 필드)
#define MAXTILESIZE_BOSS 28 //최대 타일 갯수(보스 필드)


typedef struct tagINDEX
{
	int  x;
	int  y;
} INDEX;




//노드 상태
enum NODESTATE { NODE_START, NODE_END, NODE_WALL, NODE_EMPTY };

//노드 클래스
class node
{
	//에이스타에서 기억해야 할 2가지
	//1. 비용(F, G, H)
	//2. 부모노드 포인터(이전노드의 포인터)
public:
	INDEX index;			//인덱스 x, y
	int F, G, H;			//F = G + H //G = 시작 to 현재 //H = 현재 to 종료
	node* parentNode;		//부모를 가리킬 노드(이전노드)
	NODESTATE nodeState;	//노드의 상태(시작, 종료, 벽, 빈노드)
	//노드 생성자
	node() {}
	//생성자 오버로딩을 통한 초기화
	node(int _idx, int _idy)
	{
		index = { _idx, _idy };
		F = G = H = 0;
		nodeState = NODE_EMPTY;
		parentNode = NULL;
	}
};

//타일 분류
enum state_TileSort
{
	SORT_TILE, SORT_OBJ, SORT_ENEMY, SORT_BLOCK
};

//오브젝트 속성
enum state_OBJECT
{
	NONE,
	BLOCK,
};

//타일구조체
struct tagTile
{
	int x, y;
	INDEX index;			//인덱스 x, y
	RECT rc;

	float x_mini, y_mini;
	RECT rc_mini;

	bool z_order;
	bool isFloodFill;
};

//타일 이미지 키값 구조체
struct tagTileStrKey
{
	char str_img[128];
	char str_img_mini[128];
	state_OBJECT state_obj;
	int floor;
	char str_img_floor[4][128];
	char str_img_floor_mini[4][128];

	char str_img_obj[128];
	char str_img_obj_mini[128];
	char str_img_enemy[128];
	char str_img_enemy_mini[128];
	bool isImg;
	bool isImg_obj;
	bool isImg_enemy;
};

//타일구조체 이미지
struct tagTileImg
{
	image* img;
	image* img_mini;
	image* img_floor[4];
	image* img_floor_mini[4];
	image* img_obj;
	image* img_obj_mini;
	image* img_enemy;
	image* img_enemy_mini;
};



