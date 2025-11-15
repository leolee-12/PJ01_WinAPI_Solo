#include "pch.h"
#include "CPlayerDeadText.h"
#include "CBmpMgr.h"
#include "CUIMgr.h"

CPlayerDeadText::CPlayerDeadText()
{
}

CPlayerDeadText::~CPlayerDeadText()
{
}

void CPlayerDeadText::Initialize()
{
	m_tInfo.fX = float(WINCX >> 1);
	m_tInfo.fY = float(WINCY >> 1);
	m_tInfo.fCX = 853.f;
	m_tInfo.fCY = 174.f;

	__super::Update_Rect();

	m_dwTextTime = GetTickCount();
}

int CPlayerDeadText::Update()
{
	if (m_dwTextTime + 5000 < GetTickCount())
	{
		return OBJ_DEAD;
	}

    return OBJ_NOEVENT;
}

void CPlayerDeadText::Late_Update()
{
	int iTemp = 1 + (5000 - (GetTickCount() - m_dwTextTime)) / 1000;

	m_sCount = to_string(iTemp);
}

void CPlayerDeadText::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CUIMgr::m_hPlayerDeadTextDC,
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));

	UINT len = m_sCount.size();

	for (size_t i = 0; i < len; ++i)
	{
		GdiTransparentBlt(hDC,
			(int)(m_tRect.left - 26.f),
			(int)(m_tRect.top + m_tInfo.fCY * 0.45f),
			52,
			87,
			CUIMgr::m_hNumDC[CUIMgr::DEADNUM],
			52 * (m_sCount[i] - '0'),
			0,
			52,
			87,
			RGB(0, 0, 0));
	}
}

void CPlayerDeadText::Release()
{
}

void CPlayerDeadText::Set_Data(int _iData)
{
}
