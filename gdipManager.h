#pragma once
#include "singletonBase.h"
#include "gdipImage.h"

class gdipManager : public singletonBase<gdipManager>
{
private:
	typedef map<string, gdipImage*> mapGdipImgList;				//맵으로 만든 이미지목록
	typedef map<string, gdipImage*>::iterator mapGdipImgIter;	//맵으로 만든 이미지목록의 반복자

private:
	mapGdipImgList _mGdipImgList;								//실제 이미지클래스를 담을 맵

public:
	HRESULT init();
	void release();

	//========================================================================================================================================================
	//	이미지추가
	//========================================================================================================================================================
	gdipImage * addImage(string strKey, const char* fileName, BOOL isTrans = false, COLORREF _transColor = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);
	gdipImage * addImage(string strKey, const char* fileName, int destX, int destY, BOOL isTrans = false, COLORREF _transColor = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);
	gdipImage * addFrameImage(string strKey, const char* fileName, int destX, int destY, int maxFrameX, int maxFrameY, BOOL isTrans = false, COLORREF transColor = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);

	//========================================================================================================================================================
	//	이미지 찾기
	//========================================================================================================================================================
	gdipImage * findImage(string strKey);

	//========================================================================================================================================================
	//	이미지 삭제
	//========================================================================================================================================================
	BOOL deleteAll();

	//========================================================================================================================================================
	//	이미지 렌더
	//========================================================================================================================================================
	void gdiRender(string strKey, HDC hdc, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, int width, int height, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, float wRatio, float hRatio, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, float wRatio, float hRatio, InterpolationMode _imode, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, Point* ptDest, int angle = 0);
	void gdiRender(string strKey, HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int angle = 0);

	//========================================================================================================================================================
	//	프레임 이미지 렌더
	//========================================================================================================================================================
	void gdiFrameRender(string strKey, HDC hdc, int angle = 0);
	void gdiFrameRender(string strKey, HDC hdc, int destX, int destY, int angle = 0);
	void gdiFrameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(string strKey, HDC hdc, int destX, int destY, int width, int height, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(string strKey, HDC hdc, int destX, int destY, float wRatio, float hRatio, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(string strKey, HDC hdc, int destX, int destY, float wRatio, float hRatio, int currentFrameX, int currentFrameY, InterpolationMode _imode, int angle = 0);

	//========================================================================================================================================================
	//	알파렌더
	//========================================================================================================================================================
	void gdiAlphaRender(string strKey, HDC hdc, float alpha, int angle = 0);
	void gdiAlphaRender(string strKey, HDC hdc, int destX, int destY, float alpha, int angle = 0);
	void gdiAlphaFrameRender(string strKey, HDC hdc, float alpha, int angle = 0);
	void gdiAlphaFrameRender(string strKey, HDC hdc, int destX, int destY, float alpha, int angle = 0);

	//========================================================================================================================================================
	//	Threshold 렌더
	//========================================================================================================================================================
	void gdiThresholdRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiThresholdRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiThresholdFrameRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiThresholdFrameRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Gamma 렌더
	//========================================================================================================================================================
	void gdiGammaRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiGammaRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiGammaFrameRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiGammaFrameRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Lighten 렌더
	//========================================================================================================================================================
	void gdiLightenRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiLightenRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiLightenFrameRender(string strKey, HDC hdc, float value, int angle = 0);
	void gdiLightenFrameRender(string strKey, HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Negative 렌더
	//========================================================================================================================================================
	void gdiNegativeRender(string strKey, HDC hdc, int angle = 0);
	void gdiNegativeRender(string strKey, HDC hdc, int destX, int destY, int angle = 0);
	void gdiNegativeFrameRender(string strKey, HDC hdc, int angle = 0);
	void gdiNegativeFrameRender(string strKey, HDC hdc, int destX, int destY, int angle = 0);

	//========================================================================================================================================================
	//	생성자 소멸자 - 사용안함
	//========================================================================================================================================================
	gdipManager() {}
	~gdipManager() {}
};