#pragma once
//=============================================================
//	## namespace MY_UTIL ##
//=============================================================

#define PI 3.141592f
#define PI2 (PI * 2)

//나중에 이미지 회전시 사용할 값
#define PI_2 (PI / 2) //90도
#define PI_4 (PI / 4) //45도
#define PI_8 (PI / 8) //22.5도

namespace MY_UTIL //이름은 너희마음대로 변경해도 된다
{
	//두점 사이의 거리
	float getDistance(float startX, float startY, float endX, float endY);

	//두점 사이의 각도
	float getAngle(float startX, float startY, float endX, float endY);

	//씬의 제목보여주기
	void showTitle(HDC hdc, const char* title, int x = 0, int y = 70, COLORREF color = RGB(255, 255, 0));

	// < 이미지넘버 > (string 이미지키, HDC hdc, int 출력x좌표, int 출력y좌표, int 출력숫자, int 숫자간격)
	void imageNum(string keyName, HDC hdc, int x, int y, int num, int gapNum);
	void imagescaleNum(string keyName, HDC hdc, int x, int y, int num, int gapNum, float sclae);

	// < 마우스 좌표 출력 >
	void textOutMouse(HDC hdc);

	// < 디버깅용 텍스트 출력 > (HDC hdc, 출력 숫자(소수2째 자리 까지), int 출력x좌표, int 출력y좌표)
	void textOutDbug(HDC hdc, int x, int y, float num = 999);

	// < 유저 데이터 세이브 >
	void savePlayer();
	// < 유저 데이터 로드 >
	void loadPlayer();


	// < 폰트 > (설명 없어도 알지? 기본 검은색임)
	void textOutFont(HDC hdc, int x, int y, const char* str, int size = 50, COLORREF color = RGB(0, 0, 0), const char* font = "메이플스토리");
	void textOutFontNum(HDC hdc, int x, int y, int Stat, int size = 50, COLORREF color = RGB(0, 0, 0), const char* font = "메이플스토리");
	void textOutFontNum(HDC hdc, int x, int y, float Stat, int size = 50, COLORREF color = RGB(0, 0, 0), const char* font = "메이플스토리");


	//프레임 있는 팝업창 그리기
	void drawPopUp(HDC hdc, RECT rc, COLORREF color, bool isFrame = false);
	void drawTalk(HDC hdc);


	//페이드 기능
	void fade(HDC hdc, float startTime, bool isIn);




};