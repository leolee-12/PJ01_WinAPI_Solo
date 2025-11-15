#include "pch.h"
#include "CNPCButton.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CTextMgr.h"

CNPCButton::CNPCButton()
	: m_eID(CNPC::NPC_END)
{
}

CNPCButton::~CNPCButton()
{
	Release();
}

void CNPCButton::Initialize()
{
	m_tInfo = { float(WINCX >> 1) - 100, 300.f, 100.f, 24.f };

	m_pText[CNPC::NPC_GUIDE] = L"";
	m_pText[CNPC::NPC_HINTBOX1] = L"";
	m_pText[CNPC::NPC_HINTBOX2] = L"";
	m_pText[CNPC::NPC_HINTBOX3] = L"";
	m_pText[CNPC::NPC_HINTBOX4] = L"";
	m_pText[CNPC::NPC_HINTBOX5] = L"";
	m_pText[CNPC::NPC_MERCHANT] = L"SHOP";
	m_pText[CNPC::NPC_NURSE] = L"HEAL";
}

int CNPCButton::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CNPCButton::Late_Update()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			CNPC::NPC_Function(m_eID);
		}
	}
}

void CNPCButton::Render(HDC hDC)
{
	if (!g_bInteraction)
		return;

	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);
	SetTextAlign(hDC, TA_LEFT);

	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		SetTextColor(hDC, RGB(255, 255, 0));	// ÆùÆ® »ö±ò
	}
	else
	{
		SetTextColor(hDC, RGB(255, 255, 255));	// ÆùÆ® »ö±ò
	}

	wchar_t wStr[64];
	swprintf(wStr, 64, m_pText[m_eID]);
	DrawText(hDC, wStr, -1, &m_tRect, DT_LEFT | DT_WORDBREAK);

	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CNPCButton::Release()
{
}

void CNPCButton::Set_Data(int iNum)
{
	m_eID = CNPC::NPCID(iNum);
}
