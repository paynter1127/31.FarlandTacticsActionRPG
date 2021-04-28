#pragma once
//=============================================================
//	## commonMacroFunction ##
//=============================================================

//POINT
inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

//선 그리기
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//RECT 만들기
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

//RECT 중심좌표에 만들기
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//Rectangle 함수 이용해서 사각형 그리기
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//Ellipse 함수 이용해서 타원 그리기
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//Ellipse 함수 이용해서 타원 그리기
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

//Rectangle 함수 오버로딩
inline void Rectangle(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//Ellipse 함수 오버로딩
inline void Ellipse(HDC hdc, RECT rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//inline int range(int fromNum, int toNum)
//{
//	return rand() % (toNum - fromNum + 1) + fromNum;
//}

//프레임렉트 함수 오버로딩(비어있는 렉트 만들기)
inline void FrameRect(HDC hdc, RECT rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FrameRect(hdc, &rc, brush);
	DeleteObject(brush);
}