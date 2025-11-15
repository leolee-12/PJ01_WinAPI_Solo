#include "pch.h"
#include "CMenu.h"
#include "CBmpMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CButton.h"
#include "CSoundMgr.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"

CMenu::CMenu()
{
	ZeroMemory(m_hBGDC, sizeof(m_hBGDC));
	ZeroMemory(m_fBGScrollX, sizeof(m_fBGScrollX));
	ZeroMemory(m_iBGScrollY, sizeof(m_iBGScrollY));
}

CMenu::~CMenu()
{
	Release();
}

void CMenu::Initialize()
{
	if (!CSoundMgr::Get_Instance()->IsPlayingBGM())
	{
		CSoundMgr::Get_Instance()->Initialize();
		CSoundMgr::Get_Instance()->PlayBGM(L"Sound_Start.wav", g_fVolume);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Button_Start.bmp", L"Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Button_Edit.bmp", L"Edit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Button_Exit.bmp", L"Exit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu0.bmp", L"BG_Menu0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu1.bmp", L"BG_Menu1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu2.bmp", L"BG_Menu2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu3.bmp", L"BG_Menu3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu4.bmp", L"BG_Menu4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Title.bmp", L"Icon_Title");

	m_hBGDC[0] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Menu0");
	m_hBGDC[1] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Menu1");
	m_hBGDC[2] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Menu2");
	m_hBGDC[3] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Menu3");
	m_hBGDC[4] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Menu4");
	m_hTitleDC = CBmpMgr::Get_Instance()->Find_Image(L"Icon_Title");

	m_iBGScrollY[1] = 250;
	m_iBGScrollY[2] = 300;
	m_iBGScrollY[3] = 50;
	m_iBGScrollY[4] = 400;

	CUIMgr::Get_Instance()->Initialize();

	CUI* pButton = CAbstractFactory<CButton>::Create_UI();
	pButton->Set_Pos(float(WINCX >> 1), float(WINCY >> 1));
	pButton->Update_Rect();
	pButton->Set_FrameKey(L"Start");
	CUIMgr::Get_Instance()->Add_UI(UIM_BUTTON_START, pButton);

	pButton = CAbstractFactory<CButton>::Create_UI();
	pButton->Set_Pos(float(WINCX >> 1), float(WINCY >> 1) + 80.f);
	pButton->Update_Rect();
	pButton->Set_FrameKey(L"Edit");
	CUIMgr::Get_Instance()->Add_UI(UIM_BUTTON_EDIT, pButton);

	pButton = CAbstractFactory<CButton>::Create_UI();
	pButton->Set_Pos(float(WINCX >> 1), float(WINCY >> 1) + 160.f);
	pButton->Update_Rect();
	pButton->Set_FrameKey(L"Exit");
	CUIMgr::Get_Instance()->Add_UI(UIM_BUTTON_EXIT, pButton);

}

void CMenu::Update()
{
	if (!m_pMouse)
		m_pMouse = CSceneMgr::Get_Instance()->Get_Mouse();

	m_fBGScrollX[1][0] -= 0.2f;
	m_fBGScrollX[1][1] = m_fBGScrollX[1][0] + (float)WINCX;
	m_fBGScrollX[2][0] -= 0.4f;
	m_fBGScrollX[2][1] = m_fBGScrollX[2][0] + (float)WINCX;
	m_fBGScrollX[3][0] -= 0.6f;
	m_fBGScrollX[3][1] = m_fBGScrollX[3][0] + (float)WINCX * 2.f;
	m_fBGScrollX[4][0] -= 0.8f;
	m_fBGScrollX[4][1] = m_fBGScrollX[4][0] + (float)WINCX;

	for (int i = 1; i < 5; ++i)
	{
		if ((i != 3) && (m_fBGScrollX[i][0] <= -WINCX))
		{
			m_fBGScrollX[i][0] = 0;
		}
		else if (m_fBGScrollX[i][0] <= -2 * WINCX)
		{
			m_fBGScrollX[i][0] = 0;
		}
	}

	CUIMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
}

void CMenu::Late_Update()
{
	CUIMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
}

void CMenu::Render(HDC hDC)
{

	BitBlt(hDC, 0, 0, WINCX, WINCY, m_hBGDC[0], 0, 0, SRCCOPY);

	GdiTransparentBlt(hDC,
		(int)m_fBGScrollX[1][0],
		m_iBGScrollY[1],
		WINCX,
		435,
		m_hBGDC[1],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		(int)m_fBGScrollX[1][1],
		m_iBGScrollY[1],
		WINCX,
		435,
		m_hBGDC[1],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	GdiTransparentBlt(hDC,
		m_fBGScrollX[2][0],
		m_iBGScrollY[2],
		WINCX,
		435,
		m_hBGDC[2],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		m_fBGScrollX[2][1],
		m_iBGScrollY[2],
		WINCX,
		435,
		m_hBGDC[2],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	GdiTransparentBlt(hDC,
		m_fBGScrollX[3][0],
		m_iBGScrollY[3],
		2 * WINCX,
		434,
		m_hBGDC[3],
		0,
		0,
		2 * WINCX,
		434,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		m_fBGScrollX[3][1],
		m_iBGScrollY[3],
		2 * WINCX,
		434,
		m_hBGDC[3],
		0,
		0,
		2 * WINCX,
		434,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	GdiTransparentBlt(hDC,
		m_fBGScrollX[4][0],
		m_iBGScrollY[4],
		WINCX,
		509,
		m_hBGDC[4],
		0,
		0,
		WINCX,
		509,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		m_fBGScrollX[4][1],
		m_iBGScrollY[4],
		WINCX,
		509,
		m_hBGDC[4],
		0,
		0,
		WINCX,
		509,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	CUIMgr::Get_Instance()->Render(hDC);
	CObjMgr::Get_Instance()->Render(hDC);
	
	GdiTransparentBlt(hDC,
		int((WINCX >> 1) - 243.f),
		int(200.f - 71.f),
		486,
		142,
		m_hTitleDC,
		0,
		0,
		486,
		142,
		RGB(121, 230, 234));			// 제거할 픽셀 색상 값
}

void CMenu::Release()
{
	CUIMgr::Get_Instance()->Delete_UI(UIM_ICON);
	CUIMgr::Get_Instance()->Delete_UI(UIM_BUTTON_START);
	CUIMgr::Get_Instance()->Delete_UI(UIM_BUTTON_EDIT);
	CUIMgr::Get_Instance()->Delete_UI(UIM_BUTTON_EXIT);

	CSoundMgr::Get_Instance()->Release();
}
