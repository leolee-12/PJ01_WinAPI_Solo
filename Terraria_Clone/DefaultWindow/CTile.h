#pragma once
#include "Define.h"
#include "CObj.h"

class CTile : public CObj
{
public:
	enum TILEID	{ TILE_BLOCK, TILE_WALL, TILE_ROPE, TILE_SLOT, TILE_END };
public:
	CTile();
	virtual ~CTile();

public:
	virtual void		Initialize()	PURE;
	virtual int			Update()		PURE;
	virtual void		Late_Update()	PURE;
	virtual void		Render(HDC hDC) {}
	virtual void		Release()		PURE;
	virtual void		Check_Tile()	PURE;
	virtual void		Render_Tile(HDC hDC, int iScrollX = 0, int iScrollY = 0, float fScale = 1.f) PURE;
	virtual void		Set_Option(TILEID eID, int iNum) {}
	virtual const int	Get_Option(TILEID eID) PURE;
};