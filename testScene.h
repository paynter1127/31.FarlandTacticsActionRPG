#pragma once
#include "gameNode.h"
#include "ai_N.h"
#include "skill.h"
#include "progressBar.h"



class testScene : public gameNode
{
public:
	

	

	float _startTime;
public:// < �Լ� > -----------------------------------------------------------------------------------------------------
	HRESULT init();
	void release();
	void update();
	void render();

	


	testScene() {};
	~testScene() {};

};



