#include "pch.h"
#include "CMouse.h"
#include "CItemMgr.h"
#include "CBmpMgr.h"

CMouse::CMouse()
	:	m_pMouseItem(nullptr),
		m_hMemDC(nullptr)
{
	ZeroMemory(&m_ptMouse, sizeof(POINT));
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_tInfo.fCX = 16.f;
	m_tInfo.fCY = 16.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Mouse/Cursor.bmp", L"Cursor");
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Cursor");
}

int CMouse::Update()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	m_tInfo.fX = (float)m_ptMouse.x;
	m_tInfo.fY = (float)m_ptMouse.y;

	Update_Rect();

	ShowCursor(FALSE);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	if (m_pMouseItem)
	{
		int tempIndex = 10 * m_pMouseItem->Get_ItemSubID() + m_pMouseItem->Get_ItemCode();

		GdiTransparentBlt(hDC,
			m_tRect.right,
			m_tRect.bottom,
			(int)(m_pMouseItem->Get_Info()->fCX),
			(int)(m_pMouseItem->Get_Info()->fCY),
			CItemMgr::m_vecItemDC[m_pMouseItem->Get_ItemID()][tempIndex],
			0,
			0,
			(int)(m_pMouseItem->Get_Info()->fCX),
			(int)(m_pMouseItem->Get_Info()->fCY),
			RGB(255, 255, 255));
	}
}

void CMouse::Release()
{
}

void CMouse::Update_Rect()
{
	m_tRect.left = long(m_tInfo.fX - (m_tInfo.fCX / 2.f));
	m_tRect.top = long(m_tInfo.fY - (m_tInfo.fCY / 2.f));
	m_tRect.right = long(m_tInfo.fX + (m_tInfo.fCX / 2.f));
	m_tRect.bottom = long(m_tInfo.fY + (m_tInfo.fCY / 2.f));
}