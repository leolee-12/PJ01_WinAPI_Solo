#include "pch.h"
#include "CItem_Armor.h"
#include "CItemMgr.h"
#include "CScrollMgr.h"

CItem_Armor::CItem_Armor()
{
}

CItem_Armor::CItem_Armor(ITEMINFO tItemInfo)
{
}

CItem_Armor::~CItem_Armor()
{
}

void CItem_Armor::Initialize()
{
	CItemMgr::Get_Instance()->Set_ItemInfo(this);
}

void CItem_Armor::Late_Update()
{
}

void CItem_Armor::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_vecItemDC[m_eItemID][m_iItemSubID * 10 + m_iItemCode],
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CItem_Armor::Release()
{
}

void CItem_Armor::Use_Item(CObj* pPlayer)
{
}
