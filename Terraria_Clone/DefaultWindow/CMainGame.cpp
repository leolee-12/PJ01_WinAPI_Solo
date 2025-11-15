#include "pch.h"
#include "CMainGame.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CTileMgr.h"
#include "CSoundMgr.h"
#include "CUIMgr.h"
#include "CMouse.h"
#include "CItemMgr.h"
#include "CEffectMgr.h"
#include "CTextMgr.h"

CMainGame::CMainGame()
	:	m_hDC(nullptr),
		m_pMouse(nullptr),
		m_dwTime(GetTickCount()),
		m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	srand(m_dwTime);

	m_hDC = GetDC(g_hWnd);

	m_pMouse = new CMouse;
	m_pMouse->Initialize();

	CSceneMgr::Get_Instance()->Init_Mouse(m_pMouse);
	CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_LOGO);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	CSoundMgr::Get_Instance()->Initialize();

#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif // _DEBUG
}

void CMainGame::Update()
{
	if (!m_pMouse)
	{
		m_pMouse = new CMouse;
	}

	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update()
{
	Key_Input();

	CSceneMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render()
{
	// FPS Ãâ·Â
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);
		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	if (CSceneMgr::SC_LOGO == CSceneMgr::Get_Instance()->Get_SceneID())
		return;

	HDC     hBackDC = CBmpMgr::Get_Instance()->Find_Image(L"Back");

	CSceneMgr::Get_Instance()->Render(hBackDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
#ifdef _DEBUG

	FreeConsole();

#endif // _DEBUG

	Safe_Delete(m_pMouse);

	ReleaseDC(g_hWnd, m_hDC);

	CTextMgr::Destroy_Instance();
	CEffectMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	CItemMgr::Destroy_Instance();
	CTileMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
}

void	CMainGame::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		g_fVolume -= 0.1f;

		if (g_fVolume < 0.f)
			g_fVolume = 0.f;

		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fVolume);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F3))
	{
		g_fVolume += 0.1f;

		if (g_fVolume > 1.f)
			g_fVolume = 1.f;
		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fVolume);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F10))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
		return;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_F11))
	{
		DestroyWindow(g_hWnd);
		return;
	}
}