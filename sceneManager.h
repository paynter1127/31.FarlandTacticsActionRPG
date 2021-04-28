#pragma once
#include "singletonBase.h"
//=============================================================
//	## sceneManager ## (���Ŵ���)
//=============================================================

//�̱����� ��ӹ��� ���̵��� static���� ����� ���⶧����
//�Ϲ�Ŭ������ ���ӳ�带 ����Ϸ��� �ϸ� ������

//���ӳ�� Ŭ���� ���漱��
class gameNode;

class sceneManager : public singletonBase <sceneManager>
{
private:
	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:
	//map<string, gameNode*> _mSceneList;
	mSceneList _mSceneList;		//������ ��(ȭ��)���� ��Ƶ� ��
	gameNode* _currentScene;	//�����

	bool isDone;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���߰�
	gameNode* addScene(string sceneName, gameNode* scene);
	//������ (�ش�� �ε�)
	HRESULT loadScene(string sceneName);


	sceneManager() : _currentScene(NULL) {}
	~sceneManager() {}
};

