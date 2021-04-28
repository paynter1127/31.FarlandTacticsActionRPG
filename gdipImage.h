#pragma once

class gdipImage
{
public:
	typedef struct tagGDI
	{
		HDC			hMemDC;				//메모리 DC
		HBITMAP		hBit;				//비트맵
		HBITMAP		hOBit;				//올드비트맵
		float		x;					//이미지 x좌표(레프트)
		float		y;					//이미지 y좌표(탑)
		int			width;				//이미지 가로크기
		int			height;				//이미지 세로크기
		int			currentFrameX;		//현재프레임 x좌표
		int			currentFrameY;		//현재프레임 y좌표
		int			maxFrameX;			//최대프레임 x갯수
		int			maxFrameY;			//최대프레임 y갯수
		int			frameWidth;			//1프레임 가로크기
		int			frameHeight;		//1프레임 세로크기

		tagGDI()
		{
			hMemDC = nullptr;
			hBit = nullptr;
			hOBit = nullptr;
			x = y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
		}
	}GDI_INFO, *LPGDI_INFO;

private:
	LPGDI_INFO		_gdiInfo;		//GDI정보
	CHAR*			_fileName;		//이미지 이름
	BOOL			_isTrans;		//배경색 유무 묻기(투명과 관련)
	COLORREF		_transColor;	//배경색 없앨 RGB값(마젠타 = RGB(255, 0, 255))
	Graphics*		_gdi;
	Graphics*		_gdiRender;
	Image*			_gdiImg;

public:
	gdipImage();
	~gdipImage();

	//해제
	void release(void);

	//========================================================================================================================================================
	//	초기화
	//========================================================================================================================================================
	HRESULT init(const char* fileName, bool isTrans, COLORREF color = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);
	HRESULT init(const char* fileName, int destX, int destY, bool isTrans, COLORREF color = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);
	HRESULT init(const char* fileName, int destX, int destY, int maxFrameX, int maxFrameY, bool isTrans, COLORREF color = RGB(0, 0, 0), RotateFlipType rotateFlipType = RotateNoneFlipNone);

	//========================================================================================================================================================
	//	이미지 렌더
	//========================================================================================================================================================
	void gdiRender(HDC hdc, int angle = 0);																				//렌더
	void gdiRender(HDC hdc, int destX, int destY, int angle = 0);														//원하는 위치 렌더
	void gdiRender(HDC hdc, int destX, int destY, int width, int height, int angle = 0);								//임의 확대축소 렌더
	void gdiRender(HDC hdc, int destX, int destY, float wRatio, float hRatio, int angle = 0);								//비율 확대축소 렌더
	void gdiRender(HDC hdc, int destX, int destY, float wRatio, float hRatio, InterpolationMode _imode, int angle = 0);	//보간(이미지를 확대하거나 축소할 때는 추가로 픽셀이 삽입되거나 합쳐지는데 이때 적용되는 알고리즘) 모드 
	void gdiRender(HDC hdc, int destX, int destY, Point* ptDest, int angle = 0);										//평행사변형에 렌더
	void gdiRender(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int angle = 0);		//원하는 부분 렌더

//========================================================================================================================================================
//	프레임이미지 렌더
//========================================================================================================================================================
	void gdiFrameRender(HDC hdc, int angle = 0);
	void gdiFrameRender(HDC hdc, int destX, int destY, int angle = 0);
	void gdiFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(HDC hdc, int destX, int destY, int width, int height, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(HDC hdc, int destX, int destY, float wRatio, float hRatio, int currentFrameX, int currentFrameY, int angle = 0);
	void gdiFrameRender(HDC hdc, int destX, int destY, float wRatio, float hRatio, int currentFrameX, int currentFrameY, InterpolationMode _imode, int angle = 0);

	//========================================================================================================================================================
	//	이미지 알파렌더
	//========================================================================================================================================================
	void gdiAlphaRender(HDC hdc, float alpha, int angle = 0);
	void gdiAlphaRender(HDC hdc, int destX, int destY, float alpha, int angle = 0);
	void gdiAlphaFrameRender(HDC hdc, float alpha, int angle = 0);
	void gdiAlphaFrameRender(HDC hdc, int destX, int destY, float alpha, int angle = 0);

	//========================================================================================================================================================
	//	Threshold 렌더
	//========================================================================================================================================================
	void gdiThresholdRender(HDC hdc, float value, int angle = 0);
	void gdiThresholdRender(HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiThresholdFrameRender(HDC hdc, float value, int angle = 0);
	void gdiThresholdFrameRender(HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Gamma 렌더
	//========================================================================================================================================================
	void gdiGammaRender(HDC hdc, float value, int angle = 0);
	void gdiGammaRender(HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiGammaFrameRender(HDC hdc, float value, int angle = 0);
	void gdiGammaFrameRender(HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Lighten 렌더
	//========================================================================================================================================================
	void gdiLightenRender(HDC hdc, float value, int angle = 0);
	void gdiLightenRender(HDC hdc, int destX, int destY, float value, int angle = 0);
	void gdiLightenFrameRender(HDC hdc, float value, int angle = 0);
	void gdiLightenFrameRender(HDC hdc, int destX, int destY, float value, int angle = 0);

	//========================================================================================================================================================
	//	Negative 렌더
	//========================================================================================================================================================
	void gdiNegativeRender(HDC hdc, int angle = 0);
	void gdiNegativeRender(HDC hdc, int destX, int destY, int angle = 0);
	void gdiNegativeFrameRender(HDC hdc, int angle = 0);
	void gdiNegativeFrameRender(HDC hdc, int destX, int destY, int angle = 0);

	//========================================================================================================================================================
	//	접근자,변경자
	//========================================================================================================================================================
	inline int getFrameX()const { return _gdiInfo->currentFrameX; }
	inline void setFrameX(int frameX) { _gdiInfo->currentFrameX = frameX; }
	inline int getFrameY()const { return _gdiInfo->currentFrameY; }
	inline void setFrameY(int frameY) { _gdiInfo->currentFrameY = frameY; }
};