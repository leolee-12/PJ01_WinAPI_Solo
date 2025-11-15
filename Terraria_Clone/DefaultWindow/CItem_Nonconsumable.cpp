#include "pch.h"
#include "CItem_Nonconsumable.h"
#include "CItemMgr.h"
#include "CScrollMgr.h"
#include "CUIMgr.h"

CItem_Nonconsumable::CItem_Nonconsumable()
{
}

CItem_Nonconsumable::CItem_Nonconsumable(ITEMINFO tItemInfo)
	: CItem(tItemInfo)
{
}

CItem_Nonconsumable::~CItem_Nonconsumable()
{
}

void CItem_Nonconsumable::Render(HDC hDC)
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

void CItem_Nonconsumable::Release()
{
}

void CItem_Nonconsumable::Use_Item(CObj* pPlayer)
{
	// CItem_NC는 사용효과가 없는 아이템
}