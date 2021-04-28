#pragma once
#include "tileNode.h"


struct tagSkillControl
{
	//스킬 발동 시점 저장
	float start;
	float shotTime;
	int count;

	INDEX markPoint; //i, j
	INDEX i;
	INDEX j;
	int i_range;
	int j_range;

	int damage;
	int range;
	bool skill_ready;
	bool skill_shot;

	tagSkillControl() : skill_ready(false), skill_shot(false), count(-1) {}
};