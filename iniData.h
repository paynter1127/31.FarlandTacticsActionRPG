#pragma once
#include "singletonBase.h"

/* <사용 방법(쌤 썼던거 복사>

if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//섹션, 키, 밸류
		//[플밍15기]
		//선재=50
		//종택=100
		//...

		INIDATA->addData("플밍15기", "선재", "50.5");
		INIDATA->addData("플밍15기", "종택", "100");
		INIDATA->addData("플밍15기", "수현", "8");

		//파일 이름으로 세이브
		INIDATA->saveINI("플밍플밍");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		float num = INIDATA->loadDataFloat("플밍플밍", "플밍15기", "선재");
	}
*/

//ini데이터 구조체
struct tagIniData
{
	char* section; //[Player] => 섹션
	char* key;		//HP=100 => HP가 키값
	char* value;	//HP=100 => 100이 해당 값	
};

class iniData : public singletonBase <iniData>
{
private:
	vector<tagIniData> _vIniData;

public:
	HRESULT init();
	void release();

	//데이터 추가하기
	void addData(const char* section, const char* key, const char* value);
	//세이브 파일이름
	void saveINI(const char* fileName);

	//문자로 값을 로드하기(파일이름, 섹션, 키)
	char* loadDataString(const char* fileName, const char* section, const char* key);
	//정수로 값을 로드하기(파일이름, 섹션, 키)
	int loadDataInteger(const char* fileName, const char* section, const char* key);
	//실수로 값을 로드하기(파일이름, 섹션, 키)
	float loadDataFloat(const char* fileName, const char* section, const char* key);


	iniData() {}
	~iniData() {}
};

