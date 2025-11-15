#include "pch.h"
#include "CItemMgr.h"
#include "CCollisionMgr.h"
#include "CItem_Block.h"
#include "CItem_Weapon.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"

CItemMgr*			CItemMgr::m_pInstance = nullptr;
vector<HDC>			CItemMgr::m_vecItemDC[IT_END];
map<ITEMKEY, INFO>	CItemMgr::m_mapItemDataBase;
HDC					CItemMgr::m_hPlgDC = nullptr;
HDC					CItemMgr::m_hResetDC = nullptr;

CItemMgr::CItemMgr()
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		m_vecItemDC[i].reserve(40);
	}
}

CItemMgr::~CItemMgr()
{
	Release();
}

void CItemMgr::Add_Item(ITEMID eID, CItem* pItem)
{
	if (eID >= IT_END || nullptr == pItem)
		return;

	m_FieldItemList[eID].push_back(pItem);
}

void CItemMgr::Initialize()
{
	Init_HDCs();
	Init_DataBase();
}

void CItemMgr::Update()
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		for (auto iter = m_FieldItemList[i].begin();
			iter != m_FieldItemList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (iResult == OBJ_DEAD)
			{
				Safe_Delete<CItem*>(*iter);
				iter = m_FieldItemList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CItemMgr::Late_Update()
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		for (auto& pObj : m_FieldItemList[i])
		{
			pObj->Late_Update();

			if (m_FieldItemList[i].empty())
				break;
		}
	}
	for (UINT i = 0; i < IT_END; ++i)
	{
		CCollisionMgr::Check_Item(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front(), m_FieldItemList[i]);
	}
}

void CItemMgr::Render(HDC hDC)
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		for (auto& pObj : m_FieldItemList[i])
		{
			pObj->Render(hDC);
		}
	}
}

void CItemMgr::Release()
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		for_each(m_FieldItemList[i].begin(), m_FieldItemList[i].end(), Safe_Delete<CItem*>);
		m_FieldItemList[i].clear();
	}
}



void CItemMgr::Set_ItemInfo(CItem* pItem)
{
	// 아이템 크기 입력
	pItem->Set_Info(m_mapItemDataBase[ITEMKEY(pItem->Get_ItemID(), pItem->Get_ItemSubID(), pItem->Get_ItemCode())]);

	switch (pItem->Get_ItemID())
	{
	case IT_BLOCK:
		Set_BlockInfo(pItem);
		break;

	case IT_FURNITURE:
		Set_FurnitureInfo(pItem);
		break;

	case IT_NONCONSUMABLE:
		Set_NonconsumableInfo(pItem);
		break;

	case IT_CONSUMABLE:
		Set_ConsumableInfo(pItem);
		break;

	case IT_WEAPON:
		Set_WeaponInfo(pItem);
		break;

	case IT_ARMOR:
		Set_ArmorInfo(pItem);
		break;
	}
}

void CItemMgr::Set_BlockInfo(CItem* pItem)
{
	CItem_Block* tempBlock = static_cast<CItem_Block*>(pItem);

	switch (pItem->Get_ItemSubID())
	{
	case CItem_Block::BLOCK:
		tempBlock->Set_BlockOption(pItem->Get_ItemCode());
		break;

	case CItem_Block::WALL:
		tempBlock->Set_WallOption(pItem->Get_ItemCode());
		break;

	case CItem_Block::ROPE:
		tempBlock->Set_RopeOption(pItem->Get_ItemCode());
		break;
	}
}

void CItemMgr::Set_FurnitureInfo(CItem* pItem)
{
}

void CItemMgr::Set_NonconsumableInfo(CItem* pItem)
{
}

void CItemMgr::Set_ConsumableInfo(CItem* pItem)
{
}

void CItemMgr::Set_WeaponInfo(CItem* pItem)
{
	CItem_Weapon* tempWeapon = static_cast<CItem_Weapon*>(pItem);

	switch (pItem->Get_ItemSubID())
	{
	case CItem_Weapon::MELEE:
	{
		switch (pItem->Get_ItemCode())
		{
		case 0:
			break;
		default:
			break;
		}
	}
	break;

	case CItem_Weapon::RANGE:
	{
		switch (pItem->Get_ItemCode())
		{
		case 0:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::HOLD, 50.f, 10.f, 25.f, 1.f));
			break;

		default:
			break;
		}
	}
	break;

	case CItem_Weapon::MAGIC:
	{
		switch (pItem->Get_ItemCode())
		{
		case 0:
			break;
		default:
			break;
		}
	}
	break;

	case CItem_Weapon::SUMMON:
	{
		switch (pItem->Get_ItemCode())
		{
		case 0:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::SWING, 500.f, 45.f, 5.f, 10.f));
			break;
			break;
		default:
			break;
		}
	}
	break;

	case CItem_Weapon::TOOL:
	{
		switch (pItem->Get_ItemCode())
		{
		case 0:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::SWING, 100.f, 45.f, 5.f, 10.f));
			break;

		case 1:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::HOLD, 10.f, 45.f, 5.f, 10.f));
			break;

		case 2:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::SWING, 100.f, 45.f, 5.f, 10.f));
			break;

		case 3:
			tempWeapon->Set_WeaponStat(CItem_Weapon::WPSTAT(CItem_Weapon::SWING, 100.f, 45.f, 5.f, 10.f));
			break;

		default:
			break;
		}
	}
	break;
	}
}

void CItemMgr::Set_ArmorInfo(CItem* pItem)
{
}

void CItemMgr::Init_HDCs()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Plg.bmp", L"Plg");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/Reset.bmp", L"Reset");

	m_hPlgDC = CBmpMgr::Get_Instance()->Find_Image(L"Plg");
	m_hResetDC = CBmpMgr::Get_Instance()->Find_Image(L"Reset");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB01_Dirt.bmp", L"ITBB01_Dirt");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB02_Stone.bmp", L"ITBB02_Stone");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB03_Gold.bmp", L"ITBB03_Gold");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB04_NaturalWood.bmp", L"ITBB04_NaturalWood");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB05_ProcessedWood.bmp", L"ITBB05_ProcessedWood");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB06_Honey.bmp", L"ITBB06_Honey");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB07_Ice.bmp", L"ITBB07_Ice");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB08_Cloud.bmp", L"ITBB08_Cloud");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB09_Web.bmp", L"ITBB09_Web");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBB10_WoodPlatform.bmp", L"ITBB10_WoodPlatform");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBW01_WoodWall.bmp", L"ITBW01_WoodWall");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITBR01_Rope.bmp", L"ITBR01_Rope");

	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB00_"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB01_Dirt"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB02_Stone"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB03_Gold"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB04_NaturalWood"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB05_ProcessedWood"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB06_Honey"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB07_Ice"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB08_Cloud"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB09_Grass"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBB10_WoodPlatform"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW01_WoodWall"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW02_"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW03_NaturalWood"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW04_NaturalWoodLeaf"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW05_DoorOpen"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW06_Table"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW07_Chair"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW08_Furnace"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW09_Chest"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBW10_Anvil"));
	m_vecItemDC[IT_BLOCK].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITBR01_Rope"));

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT01_Door.bmp", L"ITFT01_Door");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT02_Table.bmp", L"ITFT02_Table");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT03_Chair.bmp", L"ITFT03_Chair");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT04_Furnace.bmp", L"ITFT04_Furnace");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT05_Chest.bmp", L"ITFT05_Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITFT06_Anvil.bmp", L"ITFT06_Anvil");

	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT00_"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT01_Door"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT02_Table"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT03_Chair"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT04_Furnace"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT05_Chest"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT06_Anvil"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT07_Wood1"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT08_Wood2"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT09_Wood3"));
	m_vecItemDC[IT_FURNITURE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITFT10_Wood4"));

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC01_CopperCoin.bmp", L"ITNC01_CopperCoin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC02_SilverCoin.bmp", L"ITNC02_SilverCoin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC03_GoldCoin.bmp", L"ITNC03_GoldCoin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC04_PlatinumCoin.bmp", L"ITNC04_PlatinumCoin");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC05_GoldBar.bmp", L"ITNC05_GoldBar");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITNC06_Bullet.bmp", L"ITNC06_Bullet");

	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC00_"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC01_CopperCoin"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC02_SilverCoin"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC03_GoldCoin"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC04_PlatinumCoin"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC05_GoldBar"));
	m_vecItemDC[IT_NONCONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITNC06_Bullet"));

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITCS01_HPPortion.bmp", L"ITCS01_HPPortion");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITCS02_HeartCrystal.bmp", L"ITCS02_HeartCrystal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITCS03_Dynamite.bmp", L"ITCS03_Dynamite");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITCS04_GravityPortion.bmp", L"ITCS04_GravityPortion");

	m_vecItemDC[IT_CONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITCS00_"));
	m_vecItemDC[IT_CONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITCS01_HPPortion"));
	m_vecItemDC[IT_CONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITCS02_HeartCrystal"));
	m_vecItemDC[IT_CONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITCS03_Dynamite"));
	m_vecItemDC[IT_CONSUMABLE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITCS04_GravityPortion"));

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW000_Sword.bmp", L"ITW000_Sword");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW001_StarFury.bmp", L"ITW001_StarFury");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW100_MegaShark.bmp", L"ITW100_MegaShark");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW101_StarCannon.bmp", L"ITW101_StarCannon");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW200_MagicHarp.bmp", L"ITW200_MagicHarp");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW300_SlimeStaff.bmp", L"ITW300_SlimeStaff");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW400_Pickaxe.bmp", L"ITW400_Pickaxe");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW401_Drill.bmp", L"ITW401_Drill");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW402_Axe.bmp", L"ITW402_Axe");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITW403_Hammer.bmp", L"ITW403_Hammer");

	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW000_Sword"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW001_StarFury"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW002_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW003_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW004_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW005_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW006_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW007_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW008_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW009_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW100_MegaShark"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW101_StarCannon"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW102_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW103_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW104_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW105_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW106_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW107_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW108_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW109_"));

	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW200_MagicHarp"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW201_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW202_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW203_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW204_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW205_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW206_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW207_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW208_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW209_"));

	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW300_SlimeStaff"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW301_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW302_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW303_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW304_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW305_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW306_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW307_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW308_"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW309_"));

	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW400_Pickaxe"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW401_Drill"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW402_Axe"));
	m_vecItemDC[IT_WEAPON].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITW403_Hammer"));

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA101_Gold1.bmp", L"ITA101_Gold1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA102_Link1.bmp", L"ITA102_Link1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA201_Gold2.bmp", L"ITA201_Gold2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA202_Link2.bmp", L"ITA202_Link2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA301_Gold3.bmp", L"ITA301_Gold3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Item/ITA302_Link3.bmp", L"ITA302_Link3");

	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA100_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA101_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA102_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA103_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA104_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA105_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA106_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA107_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA108_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA009_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA100_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA101_Gold1"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA102_Link1"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA103_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA104_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA105_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA106_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA107_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA108_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA109_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA200_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA201_Gold2"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA202_Link2"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA203_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA204_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA205_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA206_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA207_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA208_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA209_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA300_"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA301_Gold3"));
	m_vecItemDC[IT_ARMOR].push_back(CBmpMgr::Get_Instance()->Find_Image(L"ITA302_Link3"));
}

void CItemMgr::Init_DataBase()
{
	for (UINT i = 0; i < IT_END; ++i)
	{
		size_t len = m_vecItemDC[i].size();

		for (size_t j = 0; j < len; ++j)
		{
			BITMAP		bmpInfo = {};
			HBITMAP		hBitmap = (HBITMAP)GetCurrentObject(CItemMgr::m_vecItemDC[i][j], OBJ_BITMAP);
			GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

			m_mapItemDataBase.emplace(ITEMKEY((ITEMID)i, int(j * 0.1f), j % 10), INFO(0.f, 0.f, float(bmpInfo.bmWidth), float(bmpInfo.bmHeight)));
			if (i == 0 && j == 10)
				m_mapItemDataBase.emplace(ITEMKEY(IT_BLOCK, 0, 10), INFO(0.f, 0.f, float(bmpInfo.bmWidth), float(bmpInfo.bmHeight)));
		}
	}
}