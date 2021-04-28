#include "stdafx.h"
#include "txtData.h"

HRESULT txtData::init()
{
	return S_OK;
}

void txtData::release()
{
}

//세이브
void txtData::txtSave(const char * fileName, vector<string> vStr)
{
	HANDLE file;
	DWORD write;
	char str[128];
	ZeroMemory(str, sizeof(str));

	strcpy(str, vectorArrayCombine(vStr));

	file = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, str, 128, &write, NULL);
	CloseHandle(file);
}

char * txtData::vectorArrayCombine(vector<string> vArray)
{
	//100, 50, 1, ...
	//100501
	char str[128];
	ZeroMemory(str, sizeof(str));
	//[10][10][1][5][7]
	//str = "1010157"
	for (int i = 0; i < vArray.size(); i++)
	{
		//str1 = "가나다", str2 = "라마바"
		strcat(str, vArray[i].c_str());
		if (i + 1 < vArray.size())
		{
			strcat(str, ";");
		}
		//str = 가나다;라마바
	}

	return str;
}

//로드
vector<string> txtData::txtLoad(const char * fileName)
{
	HANDLE file;
	DWORD read;
	char str[128];
	ZeroMemory(str, sizeof(str));

	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, str, 128, &read, NULL);
	CloseHandle(file);

	//가나다;라마바
	return charArraySeparation(str);
}

vector<string> txtData::charArraySeparation(char charArray[])
{
	vector<string> vArray;
	const char* separator = ";";
	char* token;
	//100;52;1;111;
	token = strtok(charArray, separator);
	vArray.push_back(token);
	while (NULL != (token = strtok(NULL, separator)))
	{
		vArray.push_back(token);
	}

	return vArray;
}
