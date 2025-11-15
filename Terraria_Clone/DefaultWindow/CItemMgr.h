#pragma once
#include "Define.h"
#include "CItem.h"

class CItemMgr
{
private:
	CItemMgr();
	CItemMgr(const CItemMgr& rhs) = delete;		// 삭제
	CItemMgr& operator=(CItemMgr& rObj) = delete;	// 삭제
	~CItemMgr();

private:
	static CItemMgr*	m_pInstance;
	list<CItem*>		m_FieldItemList[IT_END];

public:
	void	Add_Item(ITEMID eID, CItem* pItem);
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
	void	Init_HDCs();
	void	Init_DataBase();

	void	Set_ItemInfo(CItem* pItem);
	void	Set_BlockInfo(CItem* pItem);
	void	Set_FurnitureInfo(CItem* pItem);
	void	Set_NonconsumableInfo(CItem* pItem);
	void	Set_ConsumableInfo(CItem* pItem);
	void	Set_WeaponInfo(CItem* pItem);
	void	Set_ArmorInfo(CItem* pItem);

public:
	static CItemMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CItemMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	static void Get_ItemSize(ITEMKEY tItemKey, INFO& tInfo)
	{
		tInfo.fX = m_mapItemDataBase[tItemKey].fX;
		tInfo.fY = m_mapItemDataBase[tItemKey].fX;
		tInfo.fCX = m_mapItemDataBase[tItemKey].fCX;
		tInfo.fCY = m_mapItemDataBase[tItemKey].fCY;
	}

public:
	static vector<HDC>	m_vecItemDC[IT_END];
	static HDC			m_hPlgDC;
	static HDC			m_hResetDC;
	static map<ITEMKEY, INFO> m_mapItemDataBase;
};

