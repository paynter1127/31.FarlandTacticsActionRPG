#pragma once

//�ڵ� ���̱� define
#define XX (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].x - _data->player.x + shackX)
#define YY (_data->tile[_data->player.init_pos.x][_data->player.init_pos.y].y - _data->player.y + shackY)
#define PICKING (-0.5) * (_ptMouseC.x - _data->tile[i][j].rc.left) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0 && (0.5) * (_ptMouseC.x - _data->tile[i][j].rc.left) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 && (-0.5) * (_ptMouseC.x - _data->tile[i][j].rc.right) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y > 0 && (0.5) * (_ptMouseC.x - _data->tile[i][j].rc.right) + _data->tile[i][j].rc.top + TILEHEIGHT / 2 - _ptMouseC.y <= 0

//Ÿ�� �⺻ ���� ����
#define TILEWIDTH 64
#define TILEHEIGHT 32

#define MAXTILESIZE_TOWN 21 //�ִ� Ÿ�� ����(Ÿ��)
#define MAXTILESIZE_NORMAL 50 //�ִ� Ÿ�� ����(�븻 �ʵ�)
#define MAXTILESIZE_BOSS 28 //�ִ� Ÿ�� ����(���� �ʵ�)


typedef struct tagINDEX
{
	int  x;
	int  y;
} INDEX;




//��� ����
enum NODESTATE { NODE_START, NODE_END, NODE_WALL, NODE_EMPTY };

//��� Ŭ����
class node
{
	//���̽�Ÿ���� ����ؾ� �� 2����
	//1. ���(F, G, H)
	//2. �θ��� ������(��������� ������)
public:
	INDEX index;			//�ε��� x, y
	int F, G, H;			//F = G + H //G = ���� to ���� //H = ���� to ����
	node* parentNode;		//�θ� ����ų ���(�������)
	NODESTATE nodeState;	//����� ����(����, ����, ��, ����)
	//��� ������
	node() {}
	//������ �����ε��� ���� �ʱ�ȭ
	node(int _idx, int _idy)
	{
		index = { _idx, _idy };
		F = G = H = 0;
		nodeState = NODE_EMPTY;
		parentNode = NULL;
	}
};

//Ÿ�� �з�
enum state_TileSort
{
	SORT_TILE, SORT_OBJ, SORT_ENEMY, SORT_BLOCK
};

//������Ʈ �Ӽ�
enum state_OBJECT
{
	NONE,
	BLOCK,
};

//Ÿ�ϱ���ü
struct tagTile
{
	int x, y;
	INDEX index;			//�ε��� x, y
	RECT rc;

	float x_mini, y_mini;
	RECT rc_mini;

	bool z_order;
	bool isFloodFill;
};

//Ÿ�� �̹��� Ű�� ����ü
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

//Ÿ�ϱ���ü �̹���
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



