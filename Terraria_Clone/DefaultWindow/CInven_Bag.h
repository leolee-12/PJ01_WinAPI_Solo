#pragma once  
#include "CUI.h"
#include "CTileSlot.h"
#include "CItem.h"

class CInven_Bag : public CUI
{
public:
	CInven_Bag();
	virtual ~CInven_Bag();

public:
	void        Initialize()	override;
	int         Update()		override;
	void        Late_Update()	override;
	void        Render(HDC hDC)	override;
	void        Release()		override;

	void		Set_Data(int iIndex) override;
	int			Get_SelectIndex() { return m_iSelectIndex; }
	bool		Gain_Item(CItem* pItem);
	bool		Remove_Item(ITEMINFO pItemInfo);
	CItem*		Get_Item(int iIndex) { return m_vecItem[iIndex]; }
	vector<CTile*>*		Get_vecSlot() { return &m_vecSlot; }
	vector<CItem*>*		Get_vecItem() { return &m_vecItem; }
	void		Set_PlayerHand();

	void		Key_Input();
	bool		Check_Item(ITEMINFO tItemInfo);
	void		Swap_Item(CItem*& pDst, CItem*& pSrc);
	void		Throw_Item(CItem*& pDst);

private:
	CObj*				m_pPlayer;
	vector<CTile*>		m_vecSlot;
	vector<CItem*>		m_vecItem;
	int					m_iSelectIndex;
};
