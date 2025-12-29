#include "pch.h"
#include "CItem_Block.h"
#include "CItemMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"
#include "CTileBlock.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
using namespace TileConst;

CItem_Block::CItem_Block()
	:	m_fBlockCoolTime(300.f)
{
	ZeroMemory(&m_tTileInfo, sizeof(TILEINFO));
}

CItem_Block::CItem_Block(ITEMINFO tItemInfo)
	:	CItem(tItemInfo),
		m_fBlockCoolTime(300.f)
{
	ZeroMemory(&m_tTileInfo, sizeof(TILEINFO));
}

CItem_Block::~CItem_Block()
{
}

void CItem_Block::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_vecItemDC[m_eItemID][m_iItemSubID * 10 + m_iItemCode],
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));
}

void CItem_Block::Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle)
{
	float fTempAngle = 0.f;

	_fX += 32.f * cosf(fHandAngle);
	_fY += 24.f * sinf(fHandAngle);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	size_t	idx = m_iItemSubID * 10 + m_iItemCode;
	INFO	tempInfo = CItemMgr::m_mapItemDataBase[ITEMKEY(m_eItemID, m_iItemSubID, m_iItemCode)];

	GdiTransparentBlt(hDC,
		(int)(_fX - m_tInfo.fCX * 0.5f + iScrollX),
		(int)(_fY - m_tInfo.fCY * 0.5f + iScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_vecItemDC[m_eItemID][m_iItemSubID * 10 + m_iItemCode],
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CItem_Block::Use_Item(CObj* pPlayer)
{
	DWORD tempTime = GetTickCount();

	if (m_fTime[TM_USE] + m_fBlockCoolTime < tempTime)
	{
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();
		int x = pt.x / TILECX;
		int y = pt.y / TILECY;
		int iIndex = y * TILEX + x;

		if (0 > iIndex || iIndex >= TILEX * TILEY)
			return;

		if (m_tTileInfo.iBlock != 0)
			Use_Block(iIndex);
		else if (m_tTileInfo.iWall != 0)
			Use_Wall(iIndex);
		else if (m_tTileInfo.iRope != 0)
			Use_Rope(iIndex);

		m_fTime[TM_USE] = tempTime;
	}
}

void CItem_Block::Use_Block(int iIndex)
{
	CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(iIndex);

	if (pTile == nullptr) return;

	if (pTile->Get_Option(CTile::TILE_BLOCK) == BLOCK_NONE)
	{
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_ITBL.wav", SOUND_EFFECT5, g_fVolume, 1, m_fBlockCoolTime);

		pTile->Set_Option(CTile::TILE_BLOCK, m_tTileInfo.iBlock);
		static_cast<CTileBlock*>(pTile)->Set_BlockHP(100);
		CTileMgr::Get_Instance()->Set_Active(iIndex);
		CTileMgr::Get_Instance()->Check_SideTile(0, iIndex);

		m_iCount--;
	}
}

void CItem_Block::Use_Wall(int iIndex)
{
	CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(iIndex);

	if (pTile == nullptr) return;

	if (pTile->Get_Option(CTile::TILE_WALL) == WALL_NONE)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig.wav", SOUND_EFFECT3, g_fVolume);

		pTile->Set_Option(CTile::TILE_WALL, m_tTileInfo.iWall);
		static_cast<CTileBlock*>(pTile)->Set_WallHP(100);
		CTileMgr::Get_Instance()->Set_Active(iIndex);
		CTileMgr::Get_Instance()->Check_SideTile(1, iIndex);

		m_iCount--;
	}
}

void CItem_Block::Use_Rope(int iIndex)
{
	CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(iIndex);

	if (pTile == nullptr) return;

	if (pTile->Get_Option(CTile::TILE_ROPE) == ROPE_NONE)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig.wav", SOUND_EFFECT3, g_fVolume);

		pTile->Set_Option(CTile::TILE_ROPE, m_tTileInfo.iRope);
		CTileMgr::Get_Instance()->Set_Active(iIndex);
		CTileMgr::Get_Instance()->Check_SideTile(2, iIndex);

		m_iCount--;
	}
}
