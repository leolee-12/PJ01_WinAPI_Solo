#include "pch.h"
#include "CItem_Furniture.h"
#include "CFurniture.h"
#include "CItemMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CQuestBox.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"

CItem_Furniture::CItem_Furniture()
	:	m_fFTCoolTime(300.f),
		m_iFTTileX(0),
		m_iFTTileY(0)
{
	ZeroMemory(&m_tFTInfo, sizeof(INFO));
}

CItem_Furniture::CItem_Furniture(ITEMINFO tItemInfo)
	:	CItem(tItemInfo),
		m_fFTCoolTime(300.f),
		m_iFTTileX(0),
		m_iFTTileY(0)

{
}

CItem_Furniture::~CItem_Furniture()
{
}

void CItem_Furniture::Initialize()
{
	CItem::Initialize();

	CFurniture::Set_FurnitureInfo(m_iItemCode, m_tFTInfo);

	m_iFTTileX = m_tFTInfo.fCX / TILECX;
	m_iFTTileY = m_tFTInfo.fCY / TILECY;
}

void CItem_Furniture::Render(HDC hDC)
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
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CItem_Furniture::Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle)
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

void CItem_Furniture::Use_Item(CObj* pPlayer)
{
	DWORD tempTime = GetTickCount();

	if (m_fTime[TM_USE] + m_fFTCoolTime < tempTime)
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

		for (int i = 0; i < m_iFTTileY; i++)
		{
			for (int j = 0; j < m_iFTTileX; j++)
			{
				CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(iIndex + j - i * TILEX);

				if (pTile == nullptr) return;

				if (pTile->Get_Option(CTile::TILE_BLOCK) != 0) return;
			}
		}

		CTile* pTile1 = CTileMgr::Get_Instance()->Get_Tile(iIndex + TILEX);
		CTile* pTile2 = CTileMgr::Get_Instance()->Get_Tile(iIndex + 1 + TILEX);

		if (pTile1 == nullptr || pTile2 == nullptr) return;

		if ((pTile1->Get_Option(CTile::TILE_BLOCK) == 0) ||
			(pTile2->Get_Option(CTile::TILE_BLOCK) == 0)) return;

		Place_Furniture(iIndex);

		m_fTime[TM_USE] = tempTime;
	}
}

void CItem_Furniture::Place_Furniture(int iIndex)
{
	float fX = CTileMgr::Get_Instance()->Get_Tile(iIndex)->Get_Rect()->left + m_tFTInfo.fCX * 0.5f;
	float fY = CTileMgr::Get_Instance()->Get_Tile(iIndex)->Get_Rect()->bottom - m_tFTInfo.fCY * 0.5f;
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(fX, fY));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(m_iItemCode);
	CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig_0.wav", SOUND_EFFECT5, g_fVolume);
	m_iCount--;

	if (CQuestBox::m_eCurQuest == CQuestBox::Q_HOUSING)
		CUIMgr::Get_Instance()->Get_UI(UIS_QUESTBOX)->Set_Data(-1);
}