#pragma once
#include "tileNode.h"
#include "character.h"




// < base Node > -----------------------------------------------------------------------------------------------------
class BTNode   // This class represents each node in the behaviour tree.
{
public:
	virtual bool run() = 0;
};
class CompositeNode : public BTNode   //  This type of Node follows the Composite Pattern, containing a list of other Nodes.
{
private:
	list<BTNode*> children;
public:
	const list<BTNode*>& getChildren() const { return children; }
	void addChild(BTNode* child) { children.emplace_back(child); }
};

// < Decider > -----------------------------------------------------------------------------------------------------
class Selector : public CompositeNode//selector의 자식이 트루면 진행 종료
{
public:
	virtual bool run() override
	{
		for (BTNode* child : getChildren()) // The generic Selector implementation
		{
			if (child->run())  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
				return true;
		}
		return false;  // All children failed so the entire run() operation fails.
	}
};
class Sequence : public CompositeNode//sequence의 자식들이 낫이면 종료
{
public:
	virtual bool run() override
	{
		for (BTNode* child : getChildren())   // The generic Sequence implementation.
		{
			if (!child->run())  // If one child fails, then enter operation run() fails.  Success only results if all children succeed.
				return false;
		}
		return true;  // All children suceeded, so the entire run() operation succeeds.
	}
};


// < Behavior > -----------------------------------------------------------------------------------------------------
struct receivedData1
{
	tagCharacter player;
	RECT range;

	tagTile tile[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//전체 타일 구조체
	node* _totalNode[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//전체노드
};


//다이 확인
class isDie : public BTNode
{
private:
	tagCharacter* _enemy;
public:
	isDie(tagCharacter* data2) : _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		if (_enemy->stat.hp <= 0)
		{
			if(!SOUNDMANAGER->isPlaySound("die")) SOUNDMANAGER->play("die");
			_enemy->state = DIE;
			return true;
		}
		else
			return false;
	}
};

//피 확인(페이즈 선택)
class checkPhase : public BTNode
{
private:
	tagCharacter* _enemy;
public:
	checkPhase(tagCharacter* data2) : _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		if (_enemy->stat.hp < _enemy->stat.hpMax / 2)
		{
			_enemy->runSpeed = 2.0f; //속도 증가
			return false; //펄스이면 2페이즈를 실행한다. //OK
		}
		else
			//2페이즈에 변화할 상태 처리
			return true; //트루이면 1페이즈
	}
};

//거리 확인
class checkRange : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	checkRange(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		//거리 이내면 스킬 처리
		if ((abs(_data->player.index.x - _enemy->index.x) + abs(_data->player.index.y - _enemy->index.y)) <= 7)
		{
			//방향 지정(해당 방향으로 스킬 사용하기)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			//_enemy->state = ATTACK;
			return true;//트루이면 시퀀스의 다른 차일드 진행(파이어와 썬더 진행하고, false)를 해주면 어프로치 안한다.
		}
		else
		{
			//거리가 멀면 스킬 시전 안하고 바로 approach 처리
			return true;
		}
	}
};

//fire 스킬
class skill_fire : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_fire(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		//쿨타임 체크(시간 없으니 렌덤으로 지정하고 넘어갑시다.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) < 3 && _enemy->state == IDLE) //50 프로 확률로 fire 스킬 시전
		{
			//방향 지정(해당 방향으로 스킬 사용하기)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL1;
			return false;
		}
		else
		{
			//거리가 멀면 스킬 시전 안하고 바로 approach 처리
			return true;
		}
	}
};

//thunder 스킬
class skill_thunder : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_thunder(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		//쿨타임 체크(시간 없으니 렌덤으로 지정하고 넘어갑시다.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) < 3 && _enemy->state == IDLE) //50 프로 확률로 skill_thunder 스킬 시전
		{
			//방향 지정(해당 방향으로 스킬 사용하기)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL2;
			return false;
		}
		else
		{
			//거리가 멀면 스킬 시전 안하고 바로 approach 처리
			return true;
		}
	}
};

//소환 스킬
class skill_summon : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_summon(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		//쿨타임 체크(시간 없으니 렌덤으로 지정하고 넘어갑시다.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) <= 2 && _enemy->state == IDLE) //60 프로 확률로 소환 스킬 시전
		{
			//방향 지정(해당 방향으로 스킬 사용하기)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL3;
			SOUNDMANAGER->play("summon");
			return false;
		}
		else
		{
			//거리가 멀면 스킬 시전 안하고 바로 approach 처리
			return true;
		}
	}
};


//접근
class Approach : public BTNode
{
public:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	Approach(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		//A* 찾기 & 이동 처리
		bool isStartFind;
		if (_enemy->_finalList.size() == 0 && _enemy->state == IDLE) //카운터 시점을 조정함으로써 각각 돌리게 할 수 있다.
		//if (_enemy->_finalList.size() == 0) //카운터 시점을 조정함으로써 각각 돌리게 할 수 있다.
		{
			if (PtInRect(&_data->range, { _data->tile[_enemy->index.x][_enemy->index.y].x, _data->tile[_enemy->index.x][_enemy->index.y].y })) //각 에너미를 알아서 체킹
			{
				if (findEmptyIndex(&_data->player, _enemy).x != 0)
				{
					//_enemy->x = _data->_totalNode[_data->enemy->index.x][_data->enemy->index.y]
					isStartFind = false;
					_enemy->_isFind = false;
					//노드 클리어
					_enemy->_openList.clear();
					_enemy->_closeList.clear();
					_enemy->_finalList.clear();
					_enemy->nodeNum = 0;

					//시작노드와 종료 노드 설정
					isStartFind = true;
					_data->_totalNode[_enemy->index.x][_enemy->index.y]->nodeState = NODE_START;
					_enemy->_startNode = _data->_totalNode[_enemy->index.x][_enemy->index.y];
					_data->_totalNode[findEmptyIndex(&_data->player, _enemy).x][findEmptyIndex(&_data->player, _enemy).y]->nodeState = NODE_END;
					_enemy->_endNode = _data->_totalNode[findEmptyIndex(&_data->player, _enemy).x][findEmptyIndex(&_data->player, _enemy).y]; //<<end 노드의 위치를 캐릭터 주변으로
					_enemy->temp_endNode = _enemy->_endNode;
					_enemy->state = RUN;

					//길찾기
					if (isStartFind) this->pathFinding(_enemy);
					if (_enemy->_finalList.size() > 0) _enemy->_finalList.push_back(_enemy->temp_endNode);
				}
			}
		}
		return true;
	}



	//길찾기 함수
	void pathFinding(tagCharacter* character)
	{
		//시작노드 및 종료노드가 없는 경우 길찾기 못함
		if (!character->_startNode || !character->_endNode) return;

		//길찾기를 해보자!!
		//검색을 하려면 무조건 오픈리스트에 담아두고 
		//F와 H값 가장작은 놈을 찾아서 그놈을 현재노드로 변경하고
		//오픈리스트에서 현재노드는 삭제하고 
		//현재노드는 클로즈리스트에 담아둔다

		//1. 시작노드가 있어야 출발이 가능하니 
		//시작노드를 오픈리스트에 추가를 해줘야 한다
		character->_openList.push_back(character->_startNode);
		character->_finalList.push_back(character->_startNode);

		//2. 오픈리스트안에 담겨 있는 벡터를 검사해서 
		//종료노드에 도착할때까지 무한루프
		while (character->_openList.size() > 0)
		{
			character->_curNode = character->_openList[0];

			//오픈리스트 중 가장 F가 작거나 F가 같다면 
			//H가 작은 걸 현재노드로 하고
			//현재노드를 오픈리스트에서 클로즈리스트로 옮기기
			//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
			//길찾기가 시작되니 1부터 시작하자
			for (int i = 1; i < character->_openList.size(); i++)
			{
				if (character->_openList[i]->F <= character->_curNode->F && character->_openList[i]->H < character->_curNode->H)
				{
					character->_curNode = character->_openList[i];
				}
			}

			//클로즈리스트에 넣어둔다
			for (int i = 0; i < character->_openList.size(); i++)
			{
				if (character->_curNode == character->_openList[i])
				{
					delOpenList(i, character);
					//this->delOpenList(i, character);
					character->_closeList.push_back(character->_curNode);
				}
			}

			//현재노드가 마지막노드와 같냐? (길찾았냐?)
			if (character->_curNode == character->_endNode)
			{
				node* endNode = character->_endNode;
				vector<node*> tempNode;
				//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
				while (endNode != character->_startNode)
				//for(int i = 0; i < 3; i++)
				{
					tempNode.push_back(endNode);
					endNode = endNode->parentNode;
				}

				//STL리스트를 이용하면 reverse()사용하거나, push_front()를 사용할 수 있지만,
				//우린 벡터를 이용하니 꺼꾸로 푸시백해준다
				//for (int i = tempNode.size() - 1; i > tempNode.size() - 3; i--) //2칸씩 이동
				for (int i = tempNode.size() - 1; i > 0; i--)
				{
					character->_finalList.push_back(tempNode[i]);
				}
				//for (int i = 0; i < 2; i++)
				//{
				//	character->_finalList.push_back(tempNode[tempNode.size() - 1 - i]);
				//}

				character->_isFind = true;
				//종료하고 빠져나온다
				return;
			}

			//상하좌우 (순서는 상관없음 - 어짜피 주면 4개의 노드를 모두 오픈리스트에 넣어서 검사할 예정임)
			addOpenList(character->_curNode->index.x, character->_curNode->index.y - 1, character); //상
			addOpenList(character->_curNode->index.x, character->_curNode->index.y + 1, character); //하
			addOpenList(character->_curNode->index.x - 1, character->_curNode->index.y, character); //좌
			addOpenList(character->_curNode->index.x + 1, character->_curNode->index.y, character); //우

			//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
			//우상, 좌상, 우하, 좌하
			//예외처리만 잘해주면 된다
			//벽사이로 막가 안된다 등등

		}


	}

	//오픈리스트에 추가 함수
	void addOpenList(int idx, int idy, tagCharacter* character)
	{
		//예외처리
		//오픈리스트에 담을 노드가 전체노드 인덱스 밖으로 나가면 추가하지 못하게 처리한다(인덱스 범위는 0 ~ 4까지)
		if (idx < 0 || idx >= MAXTILESIZE_NORMAL || idy < 0 || idy >= MAXTILESIZE_NORMAL) return;
		//장애물은 오픈리스트에 담을 수 없다
		if (_data->_totalNode[idx][idy]->nodeState == NODE_WALL) return;
		//클로즈리스트에 있다면 이미 지나온길이니 오픈리스트에 담으면 안된다
		for (int i = 0; i < character->_closeList.size(); i++)
		{
			if (_data->_totalNode[idx][idy] == character->_closeList[i]) return;
		}

		//여기까지 왔으면 문제가 없으며 이제 오픈리스트에 추가를 하자
		//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가하기
		node* neighborNode = _data->_totalNode[idx][idy];
		int moveCost = character->_curNode->G + ((character->_curNode->index.x - idx == 0 || character->_curNode->index.y - idy == 0) ? 10 : 14);

		//오픈리스트안에 이웃노드가 있으면 안된다
		for (int i = 0; i < character->_openList.size(); i++)
		{
			if (character->_openList[i] == neighborNode) return;
		}

		//마지막으로 오픈리스트에도 없는경우 
		//G, H, ParentNode 설정 후 오픈리스트에 추가
		neighborNode->G = moveCost;
		neighborNode->H = (abs(neighborNode->index.x - character->_endNode->index.x) + abs(neighborNode->index.y - character->_endNode->index.y)) * 10;
		neighborNode->F = neighborNode->G + neighborNode->H;
		neighborNode->parentNode = character->_curNode;
		character->_openList.push_back(neighborNode);
	}

	//오픈리스트 삭제
	void delOpenList(int index, tagCharacter* character)
	{
		character->_openList.erase(character->_openList.begin() + index);
	}

	//주변 빈 인덱스 찾기
	INDEX findEmptyIndex(tagCharacter* player, tagCharacter* enemy)
	{
		//플레이어 주변 중 가장 가까운 좌표 찾기
		INDEX find;
		int judge;
		if (abs(player->index.x - enemy->index.x) + abs(player->index.y - enemy->index.y) == 1) return { 0, 0 };

		if (_data->_totalNode[player->index.x - 1][player->index.y]->nodeState != NODE_WALL)
		{
			find = { player->index.x - 1, player->index.y };
			judge = pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
		}
		else if (_data->_totalNode[player->index.x + 1][player->index.y]->nodeState != NODE_WALL)
		{
			find = { player->index.x + 1, player->index.y };
			judge = pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
		}
		else if (_data->_totalNode[player->index.x][player->index.y - 1]->nodeState != NODE_WALL)
		{
			find = { player->index.x, player->index.y - 1 };
			judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2);
		}
		else if (_data->_totalNode[player->index.x][player->index.y + 1]->nodeState != NODE_WALL)
		{
			find = { player->index.x, player->index.y + 1 };
			judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2);
		}
		

		if (_data->_totalNode[player->index.x - 1][player->index.y]->nodeState != NODE_WALL)
		{
			if (judge > pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2))
			{
				find = { player->index.x - 1, player->index.y };
				judge = pow((player->index.x - 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
			}
		}

		if (_data->_totalNode[player->index.x + 1][player->index.y]->nodeState != NODE_WALL)
		{
			if (judge > pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2))
			{
				find = { player->index.x + 1, player->index.y };
				judge = pow((player->index.x + 1) - enemy->index.x, 2) + pow(player->index.y - enemy->index.y, 2);
			}
		}

		if (_data->_totalNode[player->index.x][player->index.y - 1]->nodeState != NODE_WALL)
		{
			if (judge > pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2))
			{
				find = { player->index.x, player->index.y - 1 };
				judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y - 1) - enemy->index.y, 2);
			}
		}

		if (_data->_totalNode[player->index.x][player->index.y + 1]->nodeState != NODE_WALL)
		{
			if (judge > pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2))
			{
				find = { player->index.x, player->index.y + 1 };
				judge = pow(player->index.x - enemy->index.x, 2) + pow((player->index.y + 1) - enemy->index.y, 2);
			}
		}

		return find;
	}

};

//어택 처리
class Attack : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	Attack(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //생성자(맴버 이니셜라이즈)

	virtual bool run() override
	{
		{
			
			if (abs(_data->player.index.x - _enemy->index.x) + abs(_data->player.index.y - _enemy->index.y) == 1)
			{
				//어택 처리
				//방향 지정
				if (_enemy->index.x + 1 == _data->player.index.x && _enemy->index.y == _data->player.index.y)//좌하
				{
					_enemy->direc_LR = LEFT;
					_enemy->direc_DU = DOWN;
				}
				else if (_enemy->index.x == _data->player.index.x && _enemy->index.y + 1 == _data->player.index.y)//우하
				{
					_enemy->direc_LR = RIGHT;
					_enemy->direc_DU = DOWN;
				}
				else if (_enemy->index.x == _data->player.index.x && _enemy->index.y - 1 == _data->player.index.y)//좌상
				{
					_enemy->direc_LR = LEFT;
					_enemy->direc_DU = UP;
				}
				else if (_enemy->index.x - 1 == _data->player.index.x && _enemy->index.y == _data->player.index.y)//우상
				{
					_enemy->direc_LR = RIGHT;
					_enemy->direc_DU = UP;
				}
				_enemy->state = ATTACK;
				//_data->player.stat.hp -= _enemy->stat.att;
				return true;
			}
			if (abs(_data->player.index.x - _enemy->index.x) + abs(_data->player.index.y - _enemy->index.y) != 1)
			{
				//계속 approach 처리
				return false;
			}
		}
		
	}
};

