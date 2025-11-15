#include "pch.h"
#include "CFurniture.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CItemMgr.h"
#include "CAbstractFactory.h"
#include "CItem_Furniture.h"
#include "CCraft.h"
#include "CUIMgr.h"
#include "CQuestBox.h"

CFurniture::CFurniture()
	:	m_iHP(200),
		m_iFTOption(FT_NONE)
{
}

CFurniture::~CFurniture()
{
	Release();
}

void CFurniture::Initialize()
{
}

int CFurniture::Update()
{
	if (m_bDead)
	{
		if (0 < m_iFTOption && m_iFTOption < 7)
		{
			CItemMgr::Get_Instance()->Add_Item(IT_FURNITURE,
				CAbstractFactory<CItem_Furniture>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_FURNITURE, 0, m_iFTOption, 1)));
		}
		else if (m_iFTOption >= 7)
		{
			int iTemp = int(m_tInfo.fCY / 16.f);
			for (int i = 0; i < iTemp; ++i)
			{
				CItemMgr::Get_Instance()->Add_Item(IT_BLOCK,
					CAbstractFactory<CItem_Furniture>::Create_Item(m_tInfo.fX, m_tRect.bottom - 16.f * i, ITEMINFO(IT_BLOCK, 0, 5, 2)));
			}
		}
		return OBJ_DEAD;
	}
	else if (m_iHP <= 0)
	{
		m_bDead = true;
		return OBJ_NOEVENT;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CFurniture::Late_Update()
{
}

void CFurniture::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CObjMgr::m_hFurnitureDC[m_iFTOption],
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CFurniture::Release()
{
}

void CFurniture::OnCollision(COLMSG _colMsg)
{
	if (_colMsg.pObj == nullptr)
	{
		if (FT_NONE < m_iFTOption && m_iFTOption <= FT_ANVIL)
		{
			if ((_colMsg.eColType == 0) || (_colMsg.eColType == 1))
			{
				m_iHP -= 25;
			}
		}
		else if (m_iFTOption >= FT_WOOD1)
		{
			if (_colMsg.eColType == 2)
			{
				m_iHP -= 25;
			}
		}
	}
	else if(_colMsg.bFunction)
	{
		if (m_iFTOption == FT_FURNACE)
			CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(m_iFTOption);
		else if (m_iFTOption == FT_ANVIL)
			CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(m_iFTOption);
	}
	else if (!_colMsg.bFunction)
	{
		if (m_iFTOption == FT_FURNACE)
			CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(-m_iFTOption);
		else if (m_iFTOption == FT_ANVIL)
			CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(-m_iFTOption);
	}
}

void CFurniture::Set_FurnitureInfo(int iNum)
{
	m_iFTOption = (FURNITUREID)iNum;

	switch (iNum)
	{
	case FT_DOOR:
	{
		m_tInfo.fCX = 28.f;
		m_tInfo.fCY = 48.f;
	}
	break;

	case FT_TABLE:
	{
		m_tInfo.fCX = 48.f;
		m_tInfo.fCY = 34.f;
	}
	break;

	case FT_CHAIR:
	{
		m_tInfo.fCX = 16.f;
		m_tInfo.fCY = 34.f;
	}
	break;

	case FT_FURNACE:
	{
		m_tInfo.fCX = 46.f;
		m_tInfo.fCY = 34.f;
	}
	break;

	case FT_CHEST:
	{
		m_tInfo.fCX = 32.f;
		m_tInfo.fCY = 28.f;
	}
	break;

	case FT_ANVIL:
	{
		m_tInfo.fCX = 32.f;
		m_tInfo.fCY = 18.f;
	}
	break;

	case FT_WOOD1:
	{
		m_tInfo.fCX = 80.f;
		m_tInfo.fCY = 144.f;
	}
	break;

	case FT_WOOD2:
	{
		m_tInfo.fCX = 84.f;
		m_tInfo.fCY = 176.f;
	}
	break;

	case FT_WOOD3:
	{
		m_tInfo.fCX = 84.f;
		m_tInfo.fCY = 208.f;
	}
	break;

	case FT_WOOD4:
	{
		m_tInfo.fCX = 84.f;
		m_tInfo.fCY = 192.f;
	}
	break;
	}
}

void CFurniture::Set_FurnitureInfo(int iNum, INFO& tInfo)
{
	switch (iNum)
	{
	case FT_DOOR:
	{
		tInfo.fCX = 28.f;
		tInfo.fCY = 48.f;
	}
	break;

	case FT_TABLE:
	{
		tInfo.fCX = 48.f;
		tInfo.fCY = 34.f;
	}
	break;

	case FT_CHAIR:
	{
		tInfo.fCX = 16.f;
		tInfo.fCY = 34.f;
	}
	break;

	case FT_FURNACE:
	{
		tInfo.fCX = 46.f;
		tInfo.fCY = 34.f;
	}
	break;

	case FT_CHEST:
	{
		tInfo.fCX = 32.f;
		tInfo.fCY = 28.f;
	}
	break;

	case FT_ANVIL:
	{
		tInfo.fCX = 32.f;
		tInfo.fCY = 18.f;
	}
	break;

	case FT_WOOD1:
	{
		tInfo.fCX = 80.f;
		tInfo.fCY = 144.f;
	}
	break;

	case FT_WOOD2:
	{
		tInfo.fCX = 84.f;
		tInfo.fCY = 176.f;
	}
	break;

	case FT_WOOD3:
	{
		tInfo.fCX = 84.f;
		tInfo.fCY = 208.f;
	}
	break;

	case FT_WOOD4:
	{
		tInfo.fCX = 84.f;
		tInfo.fCY = 192.f;
	}
	break;
	}
}
