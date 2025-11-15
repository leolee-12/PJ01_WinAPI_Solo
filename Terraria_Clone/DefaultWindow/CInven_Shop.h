#pragma once
#include "CUI.h"
#include "CTileSlot.h"
#include "CItem.h"

class CInven_Shop : public CUI
{
public:
	CInven_Shop();
	virtual ~CInven_Shop();

public:
	void        Initialize()	override;
	int         Update()		override;
	void        Late_Update()	override;
	void        Render(HDC hDC)	override;
	void        Release()		override;

	void		Set_Data(int iIndex) override;
	bool		Gain_Item(CItem* pItem);
	bool		Remove_Item(ITEMINFO pItemInfo);
	vector<CTile*>* Get_vecSlot() { return &m_vecSlot; }

private:
	vector<CTile*>		m_vecSlot;
	vector<ITEMINFO>	m_vecItemInfo;
};