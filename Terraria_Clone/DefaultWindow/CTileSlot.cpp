#include "pch.h"
#include "CTileSlot.h"
#include "CTileMgr.h"

CTileSlot::CTileSlot()
{
}

CTileSlot::~CTileSlot()
{
	Release();
}

void CTileSlot::Initialize()
{
	m_tInfo.fCX = 52.f;
	m_tInfo.fCY = 52.f;
}

int CTileSlot::Update()
{
	Check_Tile();

	Update_Rect();

	return 0;
}

void CTileSlot::Late_Update()
{
}

void CTileSlot::Render_Tile(HDC hDC, int iScrollX, int iScrollY, float fScale)
{
	if (fScale == 1.0f)
	{
		GdiTransparentBlt(hDC,
			(int)(m_tRect.left),
			(int)(m_tRect.top),
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			CTileMgr::m_hSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
	else
	{
		float scaledCX = m_tInfo.fCX * fScale;
		float scaledCY = m_tInfo.fCX * fScale;

		float scaledLeft = m_tInfo.fX - scaledCX * 0.5f;
		float scaledTop = m_tInfo.fY - scaledCY * 0.5f;

		GdiTransparentBlt(hDC,
			(int)(scaledLeft),
			(int)(scaledTop),
			(int)(scaledCX),
			(int)(scaledCY),
			CTileMgr::m_hSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
}

void CTileSlot::Render_Picked(HDC hDC, int iScrollX, int iScrollY, float fScale)
{
	if (fScale == 1.0f)
	{
		GdiTransparentBlt(hDC,
			(int)(m_tRect.left),
			(int)(m_tRect.top),
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			CTileMgr::m_hPickedSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
	else
	{
		float scaledCX = m_tInfo.fCX * fScale;
		float scaledCY = m_tInfo.fCX * fScale;

		float scaledLeft = m_tInfo.fX - scaledCX * 0.5f;
		float scaledTop = m_tInfo.fY - scaledCY * 0.5f;

		GdiTransparentBlt(hDC,
			(int)(scaledLeft),
			(int)(scaledTop),
			(int)(scaledCX),
			(int)(scaledCY),
			CTileMgr::m_hPickedSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
}

void CTileSlot::Render_Shop(HDC hDC, int iScrollX, int iScrollY, float fScale)
{
	if (fScale == 1.0f)
	{
		GdiTransparentBlt(hDC,
			(int)(m_tRect.left),
			(int)(m_tRect.top),
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			CTileMgr::m_hShopSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
	else
	{
		float scaledCX = m_tInfo.fCX * fScale;
		float scaledCY = m_tInfo.fCX * fScale;

		float scaledLeft = m_tInfo.fX - scaledCX * 0.5f;
		float scaledTop = m_tInfo.fY - scaledCY * 0.5f;

		GdiTransparentBlt(hDC,
			(int)(scaledLeft),
			(int)(scaledTop),
			(int)(scaledCX),
			(int)(scaledCY),
			CTileMgr::m_hShopSlotDC,
			0,
			0,
			(int)(m_tInfo.fCX),
			(int)(m_tInfo.fCY),
			RGB(255, 0, 255));
	}
}

void CTileSlot::Release()
{
}

void CTileSlot::Check_Tile()
{
}