#include "pch.h"
#include "CLogo.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackGround/BG_Logo.bmp", L"Logo");

	Sleep(100);

	m_hVideo = MCIWndCreate(g_hWnd,										// 부모 윈도우 핸들
							nullptr,									// mci 윈도우를 사용하는 인스턴스 핸들
							WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,	// 자식 | 그 즉시 보이도록 재생 | 플레이바 제거
							L"../Video/logo.wmv");						// 파일 경로

	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideo);

	CSoundMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->PlayBGM(L"Sound_Start.wav", g_fVolume);
}

void CLogo::Update()
{
	if (!m_pMouse)
		m_pMouse = CSceneMgr::Get_Instance()->Get_Mouse();
}

void CLogo::Late_Update()
{
	if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo) ||
		CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}

}

void CLogo::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Logo");

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
	if (m_hVideo)
	{
		MCIWndStop(m_hVideo);
		MCIWndClose(m_hVideo);
		DestroyWindow(m_hVideo);
	}
}
