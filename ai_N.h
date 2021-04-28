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
class Selector : public CompositeNode//selector�� �ڽ��� Ʈ��� ���� ����
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
class Sequence : public CompositeNode//sequence�� �ڽĵ��� ���̸� ����
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

	tagTile tile[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//��ü Ÿ�� ����ü
	node* _totalNode[MAXTILESIZE_NORMAL][MAXTILESIZE_NORMAL];//��ü���
};


//���� Ȯ��
class isDie : public BTNode
{
private:
	tagCharacter* _enemy;
public:
	isDie(tagCharacter* data2) : _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

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

//�� Ȯ��(������ ����)
class checkPhase : public BTNode
{
private:
	tagCharacter* _enemy;
public:
	checkPhase(tagCharacter* data2) : _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		if (_enemy->stat.hp < _enemy->stat.hpMax / 2)
		{
			_enemy->runSpeed = 2.0f; //�ӵ� ����
			return false; //�޽��̸� 2����� �����Ѵ�. //OK
		}
		else
			//2����� ��ȭ�� ���� ó��
			return true; //Ʈ���̸� 1������
	}
};

//�Ÿ� Ȯ��
class checkRange : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	checkRange(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		//�Ÿ� �̳��� ��ų ó��
		if ((abs(_data->player.index.x - _enemy->index.x) + abs(_data->player.index.y - _enemy->index.y)) <= 7)
		{
			//���� ����(�ش� �������� ��ų ����ϱ�)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			//_enemy->state = ATTACK;
			return true;//Ʈ���̸� �������� �ٸ� ���ϵ� ����(���̾�� ��� �����ϰ�, false)�� ���ָ� ������ġ ���Ѵ�.
		}
		else
		{
			//�Ÿ��� �ָ� ��ų ���� ���ϰ� �ٷ� approach ó��
			return true;
		}
	}
};

//fire ��ų
class skill_fire : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_fire(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		//��Ÿ�� üũ(�ð� ������ �������� �����ϰ� �Ѿ�ô�.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) < 3 && _enemy->state == IDLE) //50 ���� Ȯ���� fire ��ų ����
		{
			//���� ����(�ش� �������� ��ų ����ϱ�)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL1;
			return false;
		}
		else
		{
			//�Ÿ��� �ָ� ��ų ���� ���ϰ� �ٷ� approach ó��
			return true;
		}
	}
};

//thunder ��ų
class skill_thunder : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_thunder(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		//��Ÿ�� üũ(�ð� ������ �������� �����ϰ� �Ѿ�ô�.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) < 3 && _enemy->state == IDLE) //50 ���� Ȯ���� skill_thunder ��ų ����
		{
			//���� ����(�ش� �������� ��ų ����ϱ�)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL2;
			return false;
		}
		else
		{
			//�Ÿ��� �ָ� ��ų ���� ���ϰ� �ٷ� approach ó��
			return true;
		}
	}
};

//��ȯ ��ų
class skill_summon : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	skill_summon(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		//��Ÿ�� üũ(�ð� ������ �������� �����ϰ� �Ѿ�ô�.)
		if (_enemy->state == SKILL1 || _enemy->state == SKILL2 || _enemy->state == RUN || _enemy->state == SKILL3)
		{
			return true;
		}
		if (RND->range(8) <= 2 && _enemy->state == IDLE) //60 ���� Ȯ���� ��ȯ ��ų ����
		{
			//���� ����(�ش� �������� ��ų ����ϱ�)
			_enemy->direc_LR = LEFT;
			_enemy->direc_DU = DOWN;
			_enemy->state = SKILL3;
			SOUNDMANAGER->play("summon");
			return false;
		}
		else
		{
			//�Ÿ��� �ָ� ��ų ���� ���ϰ� �ٷ� approach ó��
			return true;
		}
	}
};


//����
class Approach : public BTNode
{
public:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	Approach(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		//A* ã�� & �̵� ó��
		bool isStartFind;
		if (_enemy->_finalList.size() == 0 && _enemy->state == IDLE) //ī���� ������ ���������ν� ���� ������ �� �� �ִ�.
		//if (_enemy->_finalList.size() == 0) //ī���� ������ ���������ν� ���� ������ �� �� �ִ�.
		{
			if (PtInRect(&_data->range, { _data->tile[_enemy->index.x][_enemy->index.y].x, _data->tile[_enemy->index.x][_enemy->index.y].y })) //�� ���ʹ̸� �˾Ƽ� üŷ
			{
				if (findEmptyIndex(&_data->player, _enemy).x != 0)
				{
					//_enemy->x = _data->_totalNode[_data->enemy->index.x][_data->enemy->index.y]
					isStartFind = false;
					_enemy->_isFind = false;
					//��� Ŭ����
					_enemy->_openList.clear();
					_enemy->_closeList.clear();
					_enemy->_finalList.clear();
					_enemy->nodeNum = 0;

					//���۳��� ���� ��� ����
					isStartFind = true;
					_data->_totalNode[_enemy->index.x][_enemy->index.y]->nodeState = NODE_START;
					_enemy->_startNode = _data->_totalNode[_enemy->index.x][_enemy->index.y];
					_data->_totalNode[findEmptyIndex(&_data->player, _enemy).x][findEmptyIndex(&_data->player, _enemy).y]->nodeState = NODE_END;
					_enemy->_endNode = _data->_totalNode[findEmptyIndex(&_data->player, _enemy).x][findEmptyIndex(&_data->player, _enemy).y]; //<<end ����� ��ġ�� ĳ���� �ֺ�����
					_enemy->temp_endNode = _enemy->_endNode;
					_enemy->state = RUN;

					//��ã��
					if (isStartFind) this->pathFinding(_enemy);
					if (_enemy->_finalList.size() > 0) _enemy->_finalList.push_back(_enemy->temp_endNode);
				}
			}
		}
		return true;
	}



	//��ã�� �Լ�
	void pathFinding(tagCharacter* character)
	{
		//���۳�� �� �����尡 ���� ��� ��ã�� ����
		if (!character->_startNode || !character->_endNode) return;

		//��ã�⸦ �غ���!!
		//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��Ƶΰ� 
		//F�� H�� �������� ���� ã�Ƽ� �׳��� ������� �����ϰ�
		//���¸���Ʈ���� ������� �����ϰ� 
		//������� Ŭ�����Ʈ�� ��Ƶд�

		//1. ���۳�尡 �־�� ����� �����ϴ� 
		//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
		character->_openList.push_back(character->_startNode);
		character->_finalList.push_back(character->_startNode);

		//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ� 
		//�����忡 �����Ҷ����� ���ѷ���
		while (character->_openList.size() > 0)
		{
			character->_curNode = character->_openList[0];

			//���¸���Ʈ �� ���� F�� �۰ų� F�� ���ٸ� 
			//H�� ���� �� ������� �ϰ�
			//�����带 ���¸���Ʈ���� Ŭ�����Ʈ�� �ű��
			//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
			//��ã�Ⱑ ���۵Ǵ� 1���� ��������
			for (int i = 1; i < character->_openList.size(); i++)
			{
				if (character->_openList[i]->F <= character->_curNode->F && character->_openList[i]->H < character->_curNode->H)
				{
					character->_curNode = character->_openList[i];
				}
			}

			//Ŭ�����Ʈ�� �־�д�
			for (int i = 0; i < character->_openList.size(); i++)
			{
				if (character->_curNode == character->_openList[i])
				{
					delOpenList(i, character);
					//this->delOpenList(i, character);
					character->_closeList.push_back(character->_curNode);
				}
			}

			//�����尡 ���������� ����? (��ã�ҳ�?)
			if (character->_curNode == character->_endNode)
			{
				node* endNode = character->_endNode;
				vector<node*> tempNode;
				//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
				while (endNode != character->_startNode)
				//for(int i = 0; i < 3; i++)
				{
					tempNode.push_back(endNode);
					endNode = endNode->parentNode;
				}

				//STL����Ʈ�� �̿��ϸ� reverse()����ϰų�, push_front()�� ����� �� ������,
				//�츰 ���͸� �̿��ϴ� ���ٷ� Ǫ�ù����ش�
				//for (int i = tempNode.size() - 1; i > tempNode.size() - 3; i--) //2ĭ�� �̵�
				for (int i = tempNode.size() - 1; i > 0; i--)
				{
					character->_finalList.push_back(tempNode[i]);
				}
				//for (int i = 0; i < 2; i++)
				//{
				//	character->_finalList.push_back(tempNode[tempNode.size() - 1 - i]);
				//}

				character->_isFind = true;
				//�����ϰ� �������´�
				return;
			}

			//�����¿� (������ ������� - ��¥�� �ָ� 4���� ��带 ��� ���¸���Ʈ�� �־ �˻��� ������)
			addOpenList(character->_curNode->index.x, character->_curNode->index.y - 1, character); //��
			addOpenList(character->_curNode->index.x, character->_curNode->index.y + 1, character); //��
			addOpenList(character->_curNode->index.x - 1, character->_curNode->index.y, character); //��
			addOpenList(character->_curNode->index.x + 1, character->_curNode->index.y, character); //��

			//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
			//���, �»�, ����, ����
			//����ó���� �����ָ� �ȴ�
			//�����̷� ���� �ȵȴ� ���

		}


	}

	//���¸���Ʈ�� �߰� �Լ�
	void addOpenList(int idx, int idy, tagCharacter* character)
	{
		//����ó��
		//���¸���Ʈ�� ���� ��尡 ��ü��� �ε��� ������ ������ �߰����� ���ϰ� ó���Ѵ�(�ε��� ������ 0 ~ 4����)
		if (idx < 0 || idx >= MAXTILESIZE_NORMAL || idy < 0 || idy >= MAXTILESIZE_NORMAL) return;
		//��ֹ��� ���¸���Ʈ�� ���� �� ����
		if (_data->_totalNode[idx][idy]->nodeState == NODE_WALL) return;
		//Ŭ�����Ʈ�� �ִٸ� �̹� �����±��̴� ���¸���Ʈ�� ������ �ȵȴ�
		for (int i = 0; i < character->_closeList.size(); i++)
		{
			if (_data->_totalNode[idx][idy] == character->_closeList[i]) return;
		}

		//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
		//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��ϱ�
		node* neighborNode = _data->_totalNode[idx][idy];
		int moveCost = character->_curNode->G + ((character->_curNode->index.x - idx == 0 || character->_curNode->index.y - idy == 0) ? 10 : 14);

		//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
		for (int i = 0; i < character->_openList.size(); i++)
		{
			if (character->_openList[i] == neighborNode) return;
		}

		//���������� ���¸���Ʈ���� ���°�� 
		//G, H, ParentNode ���� �� ���¸���Ʈ�� �߰�
		neighborNode->G = moveCost;
		neighborNode->H = (abs(neighborNode->index.x - character->_endNode->index.x) + abs(neighborNode->index.y - character->_endNode->index.y)) * 10;
		neighborNode->F = neighborNode->G + neighborNode->H;
		neighborNode->parentNode = character->_curNode;
		character->_openList.push_back(neighborNode);
	}

	//���¸���Ʈ ����
	void delOpenList(int index, tagCharacter* character)
	{
		character->_openList.erase(character->_openList.begin() + index);
	}

	//�ֺ� �� �ε��� ã��
	INDEX findEmptyIndex(tagCharacter* player, tagCharacter* enemy)
	{
		//�÷��̾� �ֺ� �� ���� ����� ��ǥ ã��
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

//���� ó��
class Attack : public BTNode
{
private:
	receivedData1* _data;
	tagCharacter* _enemy;
public:
	Attack(receivedData1* data1, tagCharacter* data2) : _data(data1), _enemy(data2) {} //������(�ɹ� �̴ϼȶ�����)

	virtual bool run() override
	{
		{
			
			if (abs(_data->player.index.x - _enemy->index.x) + abs(_data->player.index.y - _enemy->index.y) == 1)
			{
				//���� ó��
				//���� ����
				if (_enemy->index.x + 1 == _data->player.index.x && _enemy->index.y == _data->player.index.y)//����
				{
					_enemy->direc_LR = LEFT;
					_enemy->direc_DU = DOWN;
				}
				else if (_enemy->index.x == _data->player.index.x && _enemy->index.y + 1 == _data->player.index.y)//����
				{
					_enemy->direc_LR = RIGHT;
					_enemy->direc_DU = DOWN;
				}
				else if (_enemy->index.x == _data->player.index.x && _enemy->index.y - 1 == _data->player.index.y)//�»�
				{
					_enemy->direc_LR = LEFT;
					_enemy->direc_DU = UP;
				}
				else if (_enemy->index.x - 1 == _data->player.index.x && _enemy->index.y == _data->player.index.y)//���
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
				//��� approach ó��
				return false;
			}
		}
		
	}
};

