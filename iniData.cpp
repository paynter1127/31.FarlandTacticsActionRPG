#include "stdafx.h"
#include "iniData.h"

HRESULT iniData::init()
{
	return S_OK;
}

void iniData::release()
{
}

//데이터 추가하기(섹션, 키, 밸류)
void iniData::addData(const char * section, const char * key, const char * value)
{
	//ini데이터 구조체 선언후 초기화
	tagIniData data;
	data.section = (char*)section;
	data.key = (char*)key;
	data.value = (char*)value;

	//벡터에 ini데이터 구조체 담기
	_vIniData.push_back(data);
}

void iniData::saveINI(const char * fileName)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	//=> c:/사용자/박성우/~~ + fileName.ini
	strcat(dir, str);
	//c:/사용자/박성우/fileName.ini

	for (int i = 0; i < _vIniData.size(); i++)
	{
		WritePrivateProfileString(_vIniData[i].section, _vIniData[i].key, _vIniData[i].value, dir);
	}
}

char * iniData::loadDataString(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	//=> c:/사용자/박성우/~~ + fileName.ini
	strcat(dir, str);
	//c:/사용자/박성우/fileName.ini

	char data[64] = { NULL };
	GetPrivateProfileString(section, key, "", data, 64, dir);

	return data;
}

int iniData::loadDataInteger(const char * fileName, const char * section, const char * key)
{
	char str[256];
	char dir[256];
	ZeroMemory(str, sizeof(str));
	ZeroMemory(dir, sizeof(dir));
	sprintf(str, "\\%s.ini", fileName);
	GetCurrentDirectory(256, dir);
	strcat(dir, str);

	return GetPrivateProfileInt(section, key, 0, dir);
}


//직접 구현하기
float iniData::loadDataFloat(const char * fileName, const char * section, const char * key)
{
	return atof(loadDataString(fileName, section, key));
}
