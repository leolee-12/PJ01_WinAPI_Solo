#include "pch.h"
#include "CTileBlock.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CItemMgr.h"
#include "CAbstractFactory.h"
#include "CItem_Block.h"
#include "CTile.h"

CTileBlock::CTileBlock()
	:	m_eTileState(BLOCK_HIT),
		m_iMyIndex(-1),
		m_iBlockHP(100),
		m_iWallHP(100),
		m_iRopeHP(100)
{
	ZeroMemory(&m_tTileInfo, sizeof(TILEINFO));
}

CTileBlock::~CTileBlock()
{
	Release();
}

void CTileBlock::Initialize()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CTileBlock::Update()
{
	if (m_iMyIndex == -1)
	{
		int x = m_tInfo.fX / TILECX;
		int y = m_tInfo.fY / TILECY;
		m_iMyIndex = y * TILEX + x;
	}

	if (m_eTileState != IDLE)
	{
		Check_Tile();
	}

	Update_Rect();

	return 0;
}

void CTileBlock::Late_Update()
{
}

void CTileBlock::Render_Tile(HDC hDC, int iScrollX, int iScrollY, float fScale)
{
	if (m_tTileInfo.iBlock == 0)
	{
		if (m_tTileInfo.iWall != 0)
		{
			GdiTransparentBlt(hDC,
				m_tRect.left + iScrollX,
				m_tRect.top + iScrollY,
				(int)m_tInfo.fCX,
				(int)m_tInfo.fCY,
				CTileMgr::m_hWallDC[m_tTileInfo.iWall],
				0,
				0,
				(int)m_tInfo.fCX,		// 복사할 이미지 가로
				(int)m_tInfo.fCY,		// 복사할 이미지 세로
				RGB(255, 0, 255));		// 제거할 픽셀 색상 값
		}

		if (m_tTileInfo.iRope != 0)
		{
			GdiTransparentBlt(hDC,
				m_tRect.left + iScrollX,
				m_tRect.top + iScrollY,
				(int)m_tInfo.fCX,
				(int)m_tInfo.fCY,
				CTileMgr::m_hRopeDC[m_tTileInfo.iRope],
				(int)((2.f + m_tInfo.fCX) * m_tFrame.iStart),
				(int)((2.f + m_tInfo.fCY) * m_tFrame.iEnd),
				(int)m_tInfo.fCX,		// 복사할 이미지 가로
				(int)m_tInfo.fCY,		// 복사할 이미지 세로
				RGB(255, 0, 255));		// 제거할 픽셀 색상 값
		}
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			CTileMgr::m_hBlockDC[m_tTileInfo.iBlock],
			(int)((2.f + m_tInfo.fCX) * m_tFrame.iStart),
			(int)((2.f + m_tInfo.fCY) * m_tFrame.iEnd),
			(int)m_tInfo.fCX,		// 복사할 이미지 가로
			(int)m_tInfo.fCY,		// 복사할 이미지 세로
			RGB(255, 0, 255));		// 제거할 픽셀 색상 값
	}
}

void CTileBlock::Release()
{
}

void CTileBlock::Set_Option(TILEID eID, int iNum)
{
	switch (eID)
	{
	case TILE_BLOCK:
		m_tTileInfo.iBlock = iNum;
		break;

	case TILE_WALL:
		m_tTileInfo.iWall = iNum;
		break;

	case TILE_ROPE:
		m_tTileInfo.iRope = iNum;
		break;

	default:
		break;
	}
}

const int CTileBlock::Get_Option(TILEID eID)
{
	switch (eID)
	{
	case TILE_BLOCK:
		return m_tTileInfo.iBlock;

	case TILE_WALL:
		return m_tTileInfo.iWall;

	case TILE_ROPE:
		return m_tTileInfo.iRope;

	default:
		return 0;
	}
}

void CTileBlock::Set_BlockPos(float _fX, float _fY)
{
	Set_Pos(_fX, _fY);
	int x = m_tInfo.fX / TILECX;
	int y = m_tInfo.fY / TILECY;
	m_iMyIndex = y * TILEX + x;
}

void CTileBlock::Check_Tile()
{
	if ((m_tTileInfo.iBlock != 0) && (m_iBlockHP <= 0))
	{
		if (m_tTileInfo.iBlock == 4)
			CItemMgr::Get_Instance()->Add_Item(IT_BLOCK, CAbstractFactory<CItem_Block>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_BLOCK, 0, 5, 1)));
		if (m_tTileInfo.iBlock == 9)
			CItemMgr::Get_Instance()->Add_Item(IT_BLOCK, CAbstractFactory<CItem_Block>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_BLOCK, 0, 1, 1)));
		else
			CItemMgr::Get_Instance()->Add_Item(IT_BLOCK, CAbstractFactory<CItem_Block>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_BLOCK, 0, m_tTileInfo.iBlock, 1)));

		m_tTileInfo.iBlock = 0;
		CTileMgr::Get_Instance()->Set_Active(m_iMyIndex);
	}

	if ((m_tTileInfo.iWall != 0) && (m_iWallHP <= 0))
	{
		CItemMgr::Get_Instance()->Add_Item(IT_BLOCK, CAbstractFactory<CItem_Block>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_BLOCK, 1, m_tTileInfo.iWall, 1)));

		m_tTileInfo.iWall = 0;
		
		CTileMgr::Get_Instance()->Set_Active(m_iMyIndex);
	}

	if ((m_tTileInfo.iRope != 0) && (m_iRopeHP <= 0))
	{
		CItemMgr::Get_Instance()->Add_Item(IT_BLOCK, CAbstractFactory<CItem_Block>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_BLOCK, 2, m_tTileInfo.iRope, 1)));

		m_tTileInfo.iRope = 0;

		CTileMgr::Get_Instance()->Set_Active(m_iMyIndex);
	}

	CTileMgr::Get_Instance()->Check_SideTile(0, m_iMyIndex);
}

void CTileBlock::Change_BlockDir()
{
	bool bCurLeft = false;
	bool bCurTop = false;
	bool bCurRight = false;
	bool bCurBottom = false;

	if (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex - 1))
		bCurLeft = (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex - 1)->Get_Option(CTile::TILE_BLOCK) == m_tTileInfo.iBlock);

	if (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex - TILEX))
		bCurTop = (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex - TILEX)->Get_Option(CTile::TILE_BLOCK) == m_tTileInfo.iBlock);

	if (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex + 1))
		bCurRight = (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex + 1)->Get_Option(CTile::TILE_BLOCK) == m_tTileInfo.iBlock);

	if (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex + TILEX))
		bCurBottom = (CTileMgr::Get_Instance()->Get_Tile(m_iMyIndex + TILEX)->Get_Option(CTile::TILE_BLOCK) == m_tTileInfo.iBlock);

	if ((bCurLeft == m_tTileInfo.bLeft) &&
		(bCurTop == m_tTileInfo.bTop) &&
		(bCurRight == m_tTileInfo.bRight) &&
		(bCurBottom == m_tTileInfo.bBottom))
	{
		m_eTileState = IDLE;
		return;
	}

	m_tTileInfo.bLeft = bCurLeft;
	m_tTileInfo.bTop = bCurTop;
	m_tTileInfo.bRight = bCurRight;
	m_tTileInfo.bBottom = bCurBottom;

	int iTemp = m_tTileInfo.bLeft + m_tTileInfo.bTop + m_tTileInfo.bRight + m_tTileInfo.bBottom;

	switch (iTemp)
	{
	case 0:
	{
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 0;
	}
	break;

	case 1:
	{
		m_tFrame.iEnd = 1;

		if (m_tTileInfo.bLeft) // LEFT
		{
			m_tFrame.iStart = 0;
		}
		else if (m_tTileInfo.bTop) // UP
		{
			m_tFrame.iStart = 1;
		}
		else if (m_tTileInfo.bRight) // RIGHT
		{
			m_tFrame.iStart = 2;
		}
		else if (m_tTileInfo.bBottom) // DOWN
		{
			m_tFrame.iStart = 3;
		}
	}
	break;

	case 2:
	{
		m_tFrame.iEnd = 2;

		if (m_tTileInfo.bLeft)
		{
			if (m_tTileInfo.bTop) // LU
			{
				m_tFrame.iStart = 0;
			}
			else if (m_tTileInfo.bRight) // LR
			{
				m_tFrame.iStart = 1;
			}
			else if (m_tTileInfo.bBottom) // LD
			{
				m_tFrame.iStart = 2;
			}
		}
		else if (m_tTileInfo.bTop)
		{
			if (m_tTileInfo.bRight) // UR
			{
				m_tFrame.iStart = 3;
			}
			else if (m_tTileInfo.bBottom) // UD
			{
				m_tFrame.iStart = 4;
			}
		}
		else if (m_tTileInfo.bRight) // RD
		{
			m_tFrame.iStart = 5;
		}
	}
	break;

	case 3:
	{
		m_tFrame.iEnd = 3;

		if (!m_tTileInfo.bBottom) // LUR
		{
			m_tFrame.iStart = 0;
		}
		else if (!m_tTileInfo.bRight) // LUD
		{
			m_tFrame.iStart = 1;
		}
		else if (!m_tTileInfo.bTop) // LRD
		{
			m_tFrame.iStart = 2;
		}
		else if (!m_tTileInfo.bLeft) // URD
		{
			m_tFrame.iStart = 3;
		}
	}
	break;

	case 4:
	{
		m_tFrame.iStart = 0;
		m_tFrame.iEnd = 4;
	}
	break;
	}
}

void CTileBlock::Inflict_Damage(int _iDamage, int _toolCode)
{
	if ((_toolCode == 0) || (_toolCode == 1))
	{
		m_eTileState = BLOCK_HIT;
		m_iBlockHP -= _iDamage;
	}
	else if (_toolCode == 2)
	{
		m_eTileState = WOOD_HIT;
	}
	else if (_toolCode == 3)
	{
		m_eTileState = WALL_HIT;
		m_iWallHP -= _iDamage;
	}
}