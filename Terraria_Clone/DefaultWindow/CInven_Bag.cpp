#include "pch.h"                                                                   
#include "CInven_Bag.h"
#include "CAbstractFactory.h"
#include "CBmpMgr.h"
#include "CTileSlot.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "CItem_Block.h"
#include "CItem_Furniture.h"
#include "CItem_Nonconsumable.h"
#include "CItem_Consumable.h"
#include "CItem_Weapon.h"
#include "CItem_Armor.h"
#include "CItemMgr.h"
#include "CUIMgr.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CTextMgr.h"
#include "CSoundMgr.h"
#include "CInven_Shop.h"

CInven_Bag::CInven_Bag()
	:	m_iSelectIndex(0),
		m_pPlayer(nullptr)
{
	m_vecSlot.reserve(50);
	m_vecItem.reserve(50);
}

CInven_Bag::~CInven_Bag()
{
	Release();
}

void CInven_Bag::Initialize()
{
	m_eUIID = UIS_INVENBAG;

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			m_vecSlot.push_back(CAbstractFactory<CTileSlot>::Create_Tile((52.f + 2.f) * (j + 1), (52.f + 2.f) * (i + 1)));
			m_vecSlot[10 * i + j]->Set_Size(52.f, 52.f);
			m_vecItem.push_back(nullptr);
		}
	}

	m_vecItem[0] = CAbstractFactory<CItem_Weapon>::Create_Item(0.f, 0.f, ITEMINFO(IT_WEAPON, 1, 0, 1));
	m_vecItem[1] = CAbstractFactory<CItem_Weapon>::Create_Item(0.f, 0.f, ITEMINFO(IT_WEAPON, 4, 0, 1));
	m_vecItem[2] = CAbstractFactory<CItem_Weapon>::Create_Item(0.f, 0.f, ITEMINFO(IT_WEAPON, 4, 1, 1));
	m_vecItem[3] = CAbstractFactory<CItem_Weapon>::Create_Item(0.f, 0.f, ITEMINFO(IT_WEAPON, 4, 2, 1));
	m_vecItem[4] = CAbstractFactory<CItem_Weapon>::Create_Item(0.f, 0.f, ITEMINFO(IT_WEAPON, 4, 3, 1));
	m_vecItem[5] = CAbstractFactory<CItem_Block>::Create_Item(0.f, 0.f, ITEMINFO(IT_BLOCK, 0, 2, 50));
	m_vecItem[6] = CAbstractFactory<CItem_Block>::Create_Item(0.f, 0.f, ITEMINFO(IT_BLOCK, 0, 3, 50));
	m_vecItem[7] = CAbstractFactory<CItem_Block>::Create_Item(0.f, 0.f, ITEMINFO(IT_BLOCK, 0, 5, 50));
}

int CInven_Bag::Update()
{
	if ((m_pPlayer == nullptr) && (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty()))
		m_pPlayer = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();

	if (g_bESC)
	{
		Key_Input();
	}

	for (int i = 0; i < 50; ++i)
	{
		if (m_vecItem[i] == nullptr)
			continue;

		if (m_vecItem[i]->Update() == OBJ_DEAD)
		{
			Safe_Delete<CItem*>(m_vecItem[i]);
		}
	}

	Set_PlayerHand();

	return OBJ_NOEVENT;
}

void CInven_Bag::Late_Update()
{
	for (int i = 0; i < 50; ++i)
	{
		if (m_vecSlot[i] == nullptr)
			continue;

		m_vecSlot[i]->Late_Update();
	}
}

void CInven_Bag::Render(HDC hDC)
{
	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_16);
	SetTextColor(hDC, RGB(255, 255, 0));	// 폰트 색깔
	SetTextAlign(hDC, TA_CENTER);			// 폰트 정렬 기준

	for (int i = 0; i < 50; ++i)
	{
		if ((i >= 10) && (!g_bESC))
			break;

		if (i == m_iSelectIndex)
		{
			static_cast<CTileSlot*>(m_vecSlot[i])->Render_Picked(hDC, 0, 0, 1.1f);

			if (m_vecItem[i])
			{
				m_vecItem[i]->Render_Slot(hDC, m_vecSlot[i]->Get_Info()->fX, m_vecSlot[i]->Get_Info()->fY);
			}
		}
		else
		{
			m_vecSlot[i]->Render_Tile(hDC);

			if (m_vecItem[i])
			{
				m_vecItem[i]->Render_Slot(hDC, m_vecSlot[i]->Get_Info()->fX, m_vecSlot[i]->Get_Info()->fY);
			}
		}
	}

	SetTextColor(hDC, RGB(255, 255, 255));

	for (int i = 0; i < 9; ++i)
	{
		wchar_t wStr[32];
		swprintf(wStr, 32, L"%d", i + 1);
		TextOut(hDC, m_vecSlot[i]->Get_Rect()->left + 6, m_vecSlot[i]->Get_Rect()->top + 2, wStr, lstrlen(wStr));
	}

	wchar_t wStr[32];
	swprintf(wStr, 32, L"%d", 0);
	TextOut(hDC, m_vecSlot[9]->Get_Rect()->left + 6, m_vecSlot[9]->Get_Rect()->top + 2, wStr, lstrlen(wStr));

	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CInven_Bag::Release()
{
	for (int i = 0; i < 50; ++i)
	{
		Safe_Delete<CTile*>(m_vecSlot[i]);
		Safe_Delete<CItem*>(m_vecItem[i]);
	}
}

void CInven_Bag::Set_Data(int iIndex)
{
	m_iSelectIndex = iIndex;
	CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Menu_Tick.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
}

bool CInven_Bag::Gain_Item(CItem* pItem)
{
	if (IT_BLOCK <= pItem->Get_ItemID() && pItem->Get_ItemID() <= IT_CONSUMABLE)
	{
		for (UINT i = 0; i < 50; i++)
		{
			if ((m_vecItem[i]) &&
				(m_vecItem[i]->Get_ItemID() == pItem->Get_ItemID()) &&
				(m_vecItem[i]->Get_ItemSubID() == pItem->Get_ItemSubID()) &&
				(m_vecItem[i]->Get_ItemCode() == pItem->Get_ItemCode()))
			{
				m_vecItem[i]->Add_Count(pItem->Get_Count());
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Grab.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
				return true;
			}
		}
	}

	CItem* temp = nullptr;

	for (UINT i = 0; i < 50; i++)
	{
		if (!m_vecItem[i])
		{
			switch (pItem->Get_ItemID())
			{
			case IT_BLOCK:
			{
				temp = new CItem_Block;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;

			case IT_FURNITURE:
			{
				temp = new CItem_Furniture;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;

			case IT_NONCONSUMABLE:
			{
				temp = new CItem_Nonconsumable;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;

			case IT_CONSUMABLE:
			{
				temp = new CItem_Consumable;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;

			case IT_WEAPON:
			{
				temp = new CItem_Weapon;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;

			case IT_ARMOR:
			{
				temp = new CItem_Armor;
				(*temp) = (*pItem);
				(*temp).Initialize();
			}
			break;
			}

			m_vecItem[i] = temp;
			m_vecItem[i]->Set_Pos(m_vecSlot[i]->Get_Info()->fX, m_vecSlot[i]->Get_Info()->fY);
			m_vecItem[i]->Set_ItemState(CItem::INVEN);
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Grab.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
			return true;
		}
	}

	return false;	// Inventory is Full
}

bool CInven_Bag::Remove_Item(ITEMINFO pItemInfo)
{
	for (UINT i = 0; i < 50; i++)
	{
		if ((m_vecItem[i]) &&
			(m_vecItem[i]->Get_ItemID() == pItemInfo.eItemID) &&
			(m_vecItem[i]->Get_ItemSubID() == pItemInfo.iItemSubID) &&
			(m_vecItem[i]->Get_ItemCode() == pItemInfo.iItemCode) &&
			(m_vecItem[i]->Get_Count() >= pItemInfo.iCount))
		{
			m_vecItem[i]->Add_Count(- int(pItemInfo.iCount));
			return true;
		}
	}

	return false;
}

void CInven_Bag::Set_PlayerHand()
{
	if(!m_pPlayer)
		return;

	CPlayer* tempPlayer = static_cast<CPlayer*>(m_pPlayer);

	if (tempPlayer->Get_HandItem() == m_vecItem[m_iSelectIndex])
		return;

	tempPlayer->Set_HandItem(m_vecItem[m_iSelectIndex]);
}

void CInven_Bag::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		int iTemp = CCollisionMgr::Picked_InvenSlot();

		if (iTemp != -1)
		{
			Swap_Item(m_vecItem[iTemp], static_cast<CPlayer*>(m_pPlayer)->Get_TargetMouse()->Get_MouseItem());
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Grab.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
	{
		int iTemp = CCollisionMgr::Picked_InvenSlot();
		
		if ((static_cast<CPlayer*>(m_pPlayer)->Get_TargetMouse()->Get_MouseItem()) && (iTemp == -1))
		{
			Throw_Item (static_cast<CPlayer*>(m_pPlayer)->Get_TargetMouse()->Get_MouseItem());
		}
	}
}



bool CInven_Bag::Check_Item(ITEMINFO tItemInfo)
{
	for (int i = 0; i < 50; ++i)
	{
		if (m_vecItem[i] == nullptr) continue;
		else
		{
			if (	(m_vecItem[i]->Get_ItemID() == tItemInfo.eItemID)		&&
					(m_vecItem[i]->Get_ItemSubID() == tItemInfo.iItemSubID) &&
					(m_vecItem[i]->Get_ItemCode() == tItemInfo.iItemCode)	&&
					(m_vecItem[i]->Get_Count() >= tItemInfo.iCount)	)
			{
				return true;
			}
		}
	}

	return false;
}

void CInven_Bag::Swap_Item(CItem*& pDst, CItem*& pSrc)
{
	CItem* pTemp = pDst;
	pDst = pSrc;
	pSrc = pTemp;
}

void CInven_Bag::Throw_Item(CItem*& pDst)
{
	ITEMID tempID = pDst->Get_ItemID();

	switch (tempID)
	{
	case IT_BLOCK:
	{
		CItemMgr::Get_Instance()->Add_Item(	tempID, CAbstractFactory<CItem_Block>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;

	case IT_FURNITURE:
	{
		CItemMgr::Get_Instance()->Add_Item(tempID, CAbstractFactory<CItem_Furniture>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;

	case IT_NONCONSUMABLE:
	{
		CItemMgr::Get_Instance()->Add_Item(tempID, CAbstractFactory<CItem_Nonconsumable>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;

	case IT_CONSUMABLE:
	{
		CItemMgr::Get_Instance()->Add_Item(tempID, CAbstractFactory<CItem_Consumable>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;

	case IT_WEAPON:
	{
		CItemMgr::Get_Instance()->Add_Item(tempID, CAbstractFactory<CItem_Weapon>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;

	case IT_ARMOR:
	{
		CItemMgr::Get_Instance()->Add_Item(tempID, CAbstractFactory<CItem_Armor>::
			Create_Item(	m_pPlayer->Get_Info()->fX,
							m_pPlayer->Get_Info()->fY,
							ITEMINFO(tempID, pDst->Get_ItemSubID(), pDst->Get_ItemCode(), pDst->Get_Count())));
	}
	break;
	}

	Safe_Delete(pDst);
}