#pragma once
#include "CItem.h"

class CItem_Armor : public CItem
{
public:
	CItem_Armor();
	CItem_Armor(ITEMINFO tItemInfo);
	virtual ~CItem_Armor();

	void	Initialize()			override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Use_Item(CObj* pPlayer)	override;
};

