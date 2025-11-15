#include "pch.h"
#include "CInven_Shop.h"
#include "CAbstractFactory.h"
#include "CKeyMgr.h"
#include "CInven_Bag.h"
#include "CUIMgr.h"
#include "CItem_Nonconsumable.h"
#include "CItem_Consumable.h"
#include "CSceneMgr.h"

bool g_bShop = false;

CInven_Shop::CInven_Shop()
{
	m_vecSlot.reserve(4);
	m_vecItemInfo.reserve(4);
}

CInven_Shop::~CInven_Shop()
{
	Release();
}

void CInven_Shop::Initialize()
{
	m_eUIID = UIS_INVENSHOP;

	for (int i = 0; i < 4; ++i)
	{
		m_vecSlot.push_back(CAbstractFactory<CTileSlot>::Create_Tile(400.f + 54.f * i, 370.f));
		m_vecSlot[i]->Set_Size(52.f, 52.f);
	}

	m_vecItemInfo.push_back(ITEMINFO(3, 0, 1, 1));
	m_vecItemInfo.push_back(ITEMINFO(3, 0, 3, 1));
	m_vecItemInfo.push_back(ITEMINFO(0, 0, 0, 0));
	m_vecItemInfo.push_back(ITEMINFO(0, 0, 0, 0));
}

int CInven_Shop::Update()
{
	if (!g_bShop)
		return OBJ_NOEVENT;

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		for (int i = 0; i < 4; ++i)
		{
			if (PtInRect(m_vecSlot[i]->Get_Rect(), pt))
			{
				if (m_vecItemInfo[i] != ITEMINFO(0, 0, 0, 0))
				{
					CInven_Bag* tempInven = static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG));

					if (!tempInven->Check_Item(ITEMINFO(2, 0, 1, 1)))
						return OBJ_NOEVENT;
					
					CItem* pItem = new CItem_Consumable(ITEMINFO(m_vecItemInfo[i].eItemID, m_vecItemInfo[i].iItemSubID, m_vecItemInfo[i].iItemCode, 1));

					if(tempInven->Gain_Item(pItem))
						tempInven->Remove_Item(ITEMINFO{ 2, 0, 1, 1 });

					Safe_Delete(pItem);
				}
				else if (CSceneMgr::Get_Instance()->Get_Mouse()->Get_MouseItem())
				{
					CInven_Bag* tempInven = static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG));



					CItem* pItem = new CItem_Nonconsumable(ITEMINFO(2, 0, 1, CSceneMgr::Get_Instance()->Get_Mouse()->Get_MouseItem()->Get_Count()));

					if (tempInven->Gain_Item(pItem))
					{
						Safe_Delete(CSceneMgr::Get_Instance()->Get_Mouse()->Get_MouseItem());
						CSceneMgr::Get_Instance()->Get_Mouse()->Get_MouseItem() = nullptr;
					}

					Safe_Delete(pItem);
				}
			}
		}
	}

	return OBJ_NOEVENT;
}

void CInven_Shop::Late_Update()
{
	if (!g_bShop)
		return;
}

void CInven_Shop::Render(HDC hDC)
{
	if (!g_bShop)
		return;

	for (int i = 0; i < 4; ++i)
	{
		static_cast<CTileSlot*>(m_vecSlot[i])->Render_Shop(hDC);
	}

	for (int i = 0; i < 2; ++i)
	{
		CItem::Render_Slot(hDC, m_vecItemInfo[i], m_vecSlot[i]->Get_Info()->fX, m_vecSlot[i]->Get_Info()->fY);
	}
}

void CInven_Shop::Release()
{
	for (int i = 0; i < 4; ++i)
	{
		Safe_Delete<CTile*>(m_vecSlot[i]);
	}
}

void CInven_Shop::Set_Data(int iIndex)
{
}