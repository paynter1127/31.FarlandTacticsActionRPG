#pragma once
#include "singletonBase.h"

class playerData : public singletonBase<playerData>
{
private:
	int _hp;
	POINT _pos;

public:
	HRESULT init();
	void release();

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }
	POINT getPos() { return _pos; }
	void setPos(POINT pos) { _pos = pos; }

	playerData() {}
	~playerData() {}
};

