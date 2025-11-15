#include "pch.h"
#include "CBossHPBar.h"
#include "CCharacter.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CTextMgr.h"

CBossHPBar::CBossHPBar()
	:	m_iBossMaxHP(0),
		m_iBossCurHP(0),
		m_iGap(30)
{
}

CBossHPBar::~CBossHPBar()
{
}

void CBossHPBar::Initialize()
{
	m_eUIID = UIS_HPBAR;
	m_tInfo = { float(WINCX >> 1), (float)(WINCY - 100), 516.f, 56.f};

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_BossHPBar.bmp", L"UI_BossHPBar");

	if (!CObjMgr::Get_Instance()->Get_ObjList(BOSS)->empty())
	{
		m_iBossMaxHP = static_cast<CCharacter*>(CObjMgr::Get_Instance()->Get_ObjList(BOSS)->front())->Get_MaxHP();
		m_iBossCurHP = static_cast<CCharacter*>(CObjMgr::Get_Instance()->Get_ObjList(BOSS)->front())->Get_HP();
	}
}

int CBossHPBar::Update()
{
	__super::Update_Rect();

	return 0;
}

void CBossHPBar::Late_Update()
{
}

void CBossHPBar::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_BossHPBar");;

	GdiTransparentBlt(hDC,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	float fTemp = float(m_iBossCurHP) / m_iBossMaxHP;

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left + m_iGap),
		(int)m_tRect.top,
		(int)((m_tInfo.fCX - m_iGap * 2) * fTemp),
		(int)m_tInfo.fCY,
		hMemDC,
		m_iGap,
		(int)((m_tInfo.fCY + 2.f) * 3.f),
		(int)((m_tInfo.fCX - m_iGap * 2) * fTemp),	// 복사할 이미지 가로
		(int)m_tInfo.fCY,			// 복사할 이미지 세로
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);
	const wchar_t* label = L"체력 : ";
	wchar_t wStr[32];
	swprintf(wStr, 32, L"%d / %d", m_iBossCurHP, m_iBossMaxHP);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetTextAlign(hDC, TA_CENTER);
	TextOut(hDC, int(m_tInfo.fX - 80.f), int(m_tInfo.fY - 6.f), label, lstrlen(label));
	TextOut(hDC, int(m_tInfo.fX + 40.f), int(m_tInfo.fY - 6.f), wStr, lstrlen(wStr));
	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CBossHPBar::Release()
{
}
