#pragma once
#include "Define.h"
#include "CTile.h"

class CTileBlock : public CTile
{
public:
	enum TILESTATE { IDLE, BLOCK_HIT, WOOD_HIT, WALL_HIT, TS_END};

public:
	CTileBlock();
	virtual ~CTileBlock();

public:
	void			Initialize()	override;
	int				Update()		override;
	void			Late_Update()	override;
	void			Render_Tile(HDC hDC, int iScrollX = 0, int iScrollY = 0, float fScale = 1.f)	override;
	void			Release()		override;
	void			Set_Option(TILEID eID, int iNum) override;
	const int		Get_Option(TILEID eID) override;

public:
	void			Set_BlockPos(float fX, float fY);
	void			Set_TileInfo(TILEINFO& _tTileInfo) { m_tTileInfo = _tTileInfo; }
	void			Set_BlockOption(int _iBlock) { m_tTileInfo.iBlock = _iBlock; }
	void			Set_WallOption(int _iWall) { m_tTileInfo.iWall = _iWall; }
	void			Set_RopeOption(int _iRope) { m_tTileInfo.iRope = _iRope; }
	void			Set_BlockHP(int iHP) { m_iBlockHP = iHP; }
	void			Set_WallHP(int iHP) { m_iWallHP = iHP; }
	void			Set_RopeHP(int iHP) { m_iRopeHP = iHP; }

public:
	const TILEINFO*	Get_TileInfo() { return &m_tTileInfo; }

	void			Check_Tile();
	void			Change_BlockDir();
	void			Inflict_Damage(int _iDamage, int _toolCode);

protected:
	TILESTATE		m_eTileState;
	TILEINFO		m_tTileInfo;

	int				m_iMyIndex;
	int				m_iBlockHP;
	int				m_iWallHP;
	int				m_iRopeHP;
};