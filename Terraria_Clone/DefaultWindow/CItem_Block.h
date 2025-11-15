#pragma once
#include "CItem.h"

class CItem_Block : public CItem
{
public:
	enum BLOCK_ID { BLOCK, WALL, ROPE, BI_END };	// SubID

public:
	CItem_Block();
	CItem_Block(ITEMINFO tItemInfo);
	virtual ~CItem_Block();

	void	Render(HDC hDC)			override;
	void	Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle) override;
	void	Use_Item(CObj* pPlayer)	override;

	void	Set_BlockOption(int _iBlock) { m_tTileInfo.iBlock = _iBlock; }
	void	Set_WallOption(int _iWall) { m_tTileInfo.iWall = _iWall; }
	void	Set_RopeOption(int _iRope) { m_tTileInfo.iRope = _iRope; }

	void	Use_Block(int iIndex);
	void	Use_Wall(int iIndex);
	void	Use_Rope(int iIndex);

private:
	TILEINFO	m_tTileInfo;
	float		m_fBlockCoolTime;
};

