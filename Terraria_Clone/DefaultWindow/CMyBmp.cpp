#include "pch.h"
#include "CMyBmp.h"

CMyBmp::CMyBmp()
{
}

CMyBmp::~CMyBmp()
{
	Release();
}

void CMyBmp::Load_Bmp(const TCHAR* pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	// CreateCompatibleDC : 화면 dc와 호환되는 dc를 할당

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,			// 프로그램 인스턴스 핸들(파일로부터 읽어들일 것이기 때문에 NULL)
									pFilePath,		// 파일의 이름을 포함한 경로
									IMAGE_BITMAP,	// 어떤 타입 파일
									0, 0,			// 가로, 세로 크기(파일로부터 읽어들일 것이기 때문에 임의의 사이즈를 제공할 이유가 없음)
									LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일로부터 이미지를 불러들임
																			// LR_CREATEDIBSECTION : 읽어올 파일을 DIB 형태로 변환하여 읽음	

	// SelectObject : 준비한 dc에 해당 gdi 오브젝트로 불러온 비트맵을 선택
	// gdi 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트를 반환
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

}

void CMyBmp::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
