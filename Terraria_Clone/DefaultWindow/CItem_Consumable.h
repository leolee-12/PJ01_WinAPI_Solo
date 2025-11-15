#pragma once
#include "CItem.h"

class CItem_Consumable : public CItem
{
public:
	CItem_Consumable();
	CItem_Consumable(ITEMINFO tItemInfo);
	virtual ~CItem_Consumable();

	void Render(HDC hDC)			override;
	void Use_Item(CObj* pPlayer)	override;

private:
	float m_fCSCoolTime;
};