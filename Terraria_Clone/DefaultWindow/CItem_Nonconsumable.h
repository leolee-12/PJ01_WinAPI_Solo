#pragma once
#include "CItem.h"
class CItem_Nonconsumable : public CItem
{
public:
	CItem_Nonconsumable();
	CItem_Nonconsumable(ITEMINFO tItemInfo);
	virtual ~CItem_Nonconsumable();

	void Render(HDC hDC)			override;
	void Release()					override;
	void Use_Item(CObj* pPlayer)	override;
};

