#pragma once
#include "gameNode.h"
#include "progressBar.h"

class titleScene : public gameNode
{
private:
	RECT rc_btn[5];
	RECT rc_option;
	bool isPop_option;
	RECT rc_option_close;
	RECT rc_option_button[4];
	progressBar* hpBar;
	int hp;
	int hpMax;
	bool check[2];

	float _startTime;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	titleScene() {};
	~titleScene() {};
};

