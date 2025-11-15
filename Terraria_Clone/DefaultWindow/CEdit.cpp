#include "pch.h"
#include "CEdit.h"
#include "CTileMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CSceneMgr.h"
#include "CObjMgr.h"
#include "CTextMgr.h"
#include "CItemMgr.h"

CEdit::CEdit()
	:	m_iCurBlock(1),
		m_iSetRange(0),
		m_iCurFrameStart(0)
{
	ZeroMemory(&m_tHintRect, sizeof(RECT));
}

CEdit::~CEdit()
{
	Release();
}

void CEdit::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Stage0.bmp", L"BG_Edit0");
	m_pHintText = L"W/A/S/D : 화면 이동 | Q/E : 블록 전환 | Z/C : 설치 범위 조절";
	m_tHintRect = { 0, 0, WINCX, 30 };
	CItemMgr::Get_Instance();
}

void CEdit::Update()
{
	if (!m_pMouse)
		m_pMouse = CSceneMgr::Get_Instance()->Get_Mouse();

	CTileMgr::Get_Instance()->Update();

	CObjMgr::Get_Instance()->Update();

	Key_Input();
}

void CEdit::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();

	CObjMgr::Get_Instance()->Late_Update();
}

void CEdit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	
	HDC     hGroundDC = CBmpMgr::Get_Instance()->Find_Image(L"BG_Edit0");

	BitBlt(hDC, iScrollX, iScrollY, 7200, 4000, hGroundDC, 0, 0, SRCCOPY);

	CTileMgr::Get_Instance()->Render(hDC);

	CObjMgr::Get_Instance()->Render(hDC);

	if (m_iCurBlock)
	{
		INFO tempInfo = CItemMgr::m_mapItemDataBase[ITEMKEY(IT_BLOCK, 0, m_iCurBlock)];

		GdiTransparentBlt(hDC,
			m_pMouse->Get_Rect()->right,
			m_pMouse->Get_Rect()->bottom,
			int(tempInfo.fCX),
			int(tempInfo.fCY),
			CItemMgr::m_vecItemDC[IT_BLOCK][m_iCurBlock],
			0,
			0,
			int(tempInfo.fCX),
			int(tempInfo.fCY), 
			RGB(255, 255, 255));
	}




	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);

	SetTextColor(hDC, RGB(255, 255, 255));	// 폰트 색깔
	SetTextAlign(hDC, TA_LEFT);				// 폰트 정렬 기준

	wchar_t wStr[64];
	swprintf(wStr, 64, m_pHintText);
	DrawText(hDC, wStr, -1, &m_tHintRect, DT_LEFT | DT_WORDBREAK);

	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CEdit::Release()
{
}

void CEdit::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
		CScrollMgr::Get_Instance()->Add_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
		CScrollMgr::Get_Instance()->Add_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
		CScrollMgr::Get_Instance()->Add_ScrollY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		CScrollMgr::Get_Instance()->Add_ScrollY(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		CTileMgr::Get_Instance()->Picking(pt, m_iCurBlock, m_iSetRange);
	}


	if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		m_iCurBlock--;

		if (m_iCurBlock < 0)
			m_iCurBlock += 10;
	}
	
	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		m_iCurBlock++;

		if (m_iCurBlock > 10)
			m_iCurBlock -= 11;
	}

	if (CKeyMgr::Get_Instance()->Key_Down('Z'))
	{
		m_iSetRange--;

		if (m_iSetRange < 0)
			m_iSetRange = 0;
	}
	
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		m_iSetRange++;

		if (m_iSetRange > 3)
			m_iSetRange = 3;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		CTileMgr::Get_Instance()->Picking_RB(pt, m_iCurFrameStart);
	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
		CTileMgr::Get_Instance()->Save_Tile();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
		CTileMgr::Get_Instance()->Load_Tile();
}

