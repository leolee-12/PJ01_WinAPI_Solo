#pragma once
#include "CUI.h"
class CInventory : public CUI
{
public:
	CInventory();
	virtual ~CInventory();

public:
	void		Initialize()	override;
	int			Update()		override;
	void		Late_Update()	override;
	void		Render(HDC hDC)	override;
	void		Release()		override;
};

