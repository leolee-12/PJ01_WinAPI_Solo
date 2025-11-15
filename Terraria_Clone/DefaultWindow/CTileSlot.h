#pragma once
#include "CTile.h"
class CTileSlot : public CTile
{
public:
	CTileSlot();
	virtual ~CTileSlot();

public:
	void			Initialize()	override;
	int				Update()		override;
	void			Late_Update()	override;
	void			Render_Tile(HDC hDC, int iScrollX = 0, int iScrollY = 0, float fScale = 1.f)	override;
	void			Render_Picked(HDC hDC, int iScrollX = 0, int iScrollY = 0, float fScale = 1.f);
	void			Render_Shop(HDC hDC, int iScrollX = 0, int iScrollY = 0, float fScale = 1.f);
	void			Release()		override;
	const int		Get_Option(TILEID eID) override { return 0; }

	void			Check_Tile();
};