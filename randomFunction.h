#pragma once
#include "singletonBase.h"
class randomFunction :
	public singletonBase <randomFunction>
{
public:
	HRESULT init();
	void release();

	//정수 1개 랜덤으로 가져오기
	int range(int num);
	//정수 2개 사이의 값 랜덤으로 가져오기
	int range(int fromNum, int toNum);
	//실수 1개 랜덤으로 가져오기
	float range(float num);
	//실수 2개 사이의 값 랜덤으로 가져오기
	float range(float fromNum, float toNum);

	randomFunction() {}
	~randomFunction() {}
};

