#include "pch.h"
#include "CItem.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CCollisionMgr.h"
#include "CItemMgr.h"
#include "CUIMgr.h"
#include "CInven_Bag.h"
using namespace TileConst;

CItem::CItem()
	:	m_bUse(false),
		m_eItemID(IT_END),
		m_iItemSubID(0),
		m_iItemCode(0),
		m_iTile(0),
		m_iCount(1),
		m_eItemState(FIELD)
{
	ZeroMemory(m_ptHand, sizeof(m_ptHand));
	m_fTime[TM_INVINCIBLE] = GetTickCount();
	m_fYSpeed = -4.f;
}

CItem::CItem(ITEMINFO tItemInfo)
	:	m_bUse(false),
		m_eItemID(tItemInfo.eItemID),
		m_iItemSubID(tItemInfo.iItemSubID),
		m_iItemCode(tItemInfo.iItemCode),
		m_iTile(0),
		m_iCount(tItemInfo.iCount),
		m_eItemState(FIELD)
{
	ZeroMemory(m_ptHand, sizeof(m_ptHand));
	m_fTime[TM_INVINCIBLE] = GetTickCount();
	m_fYSpeed = -4.f;
}

CItem::~CItem()
{
}

void CItem::Initialize()
{
	CItemMgr::Get_Instance()->Set_ItemInfo(this);
}

int CItem::Update()
{
	if (!m_iCount)
		return OBJ_DEAD;

	switch (m_eItemState)
	{
	case FIELD:
	{
		Status_Check();

		Update_Rect();

		if (m_pTarget)
		{
			m_fXSpeed += 0.1f * cosf(m_fAngle);
			m_fYSpeed -= 0.1f * sinf(m_fAngle);
		}
		else
		{
			m_fXSpeed *= 0.9f;
			Gravity();
		}

		Move();
	}
		break;

	case INVEN:
		break;

	case HAND:
		m_bUse = false;
		break;
	}

	return OBJ_NOEVENT;
}

void CItem::OnCollision(COLMSG _colMSG)
{
	if ((m_fTime[TM_INVINCIBLE] + 1000 < GetTickCount()) && (_colMSG.eColType == CD_END))
	{
		if (_colMSG.bFunction)
		{
			if (static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG))->Gain_Item(this))
			{
				m_iCount = 0;
			}
		}
		else
		{
			m_pTarget = _colMSG.pObj;
		}
	}
}

void CItem::Search_TargetDir()
{
	if (m_pTarget)
	{
		float	fWidth(0.f), fHeight(0.f);
		fWidth = m_pTarget->Get_Info()->fX - m_tInfo.fX;
		fHeight = m_pTarget->Get_Info()->fY - m_tInfo.fY;
		m_fAngle = atan2f(-fHeight, fWidth);
	}
}

void CItem::Search_ObjTile(float _fX, float _fY)
{
	// 바닥 타일의 top과 아이템의 bottom을 맞추기 위해 "m_tInfo.fX, m_tRect.bottom" 입력
	// 아이템은 무조건 1칸

	int iTempX = _fX / TILECX;
	int iTempY = _fY / TILECY;
	m_iTile = iTempY * TILEX + iTempX;
}

void CItem::Status_Check()
{
	Search_TargetDir();

	Search_ObjTile(m_tInfo.fX, (float)m_tRect.bottom);

	CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(m_iTile + TILEX);

	if(pTile == nullptr) return;

	if (pTile->Get_Option(CTile::TILE_BLOCK) == BLOCK_NONE)
	{
		m_bAir = true;
	}
}

void CItem::Gravity()
{
	if (m_bAir == true)
	{
		m_fYSpeed += 24.f * m_fTime[TM_GRAVITY];

		if (m_fYSpeed >= 5.f)
			m_fYSpeed = 2.5f;
	}
	else
	{
		m_fYSpeed = 0.f;
	}
}

void CItem::Move()
{
	while (!m_pTarget)
	{
		int iResult = CCollisionMgr::PredictCollision_Tile(this, m_fXSpeed, m_fYSpeed);

		if (iResult == PCT_NONE)
		{
			break;
		}

		else if (iResult == PCT_X)
		{
			if (abs(m_fXSpeed) < 0.01f)
			{
				m_fXSpeed = 0.f;
			}
			else m_fXSpeed *= 0.5f;
		}

		else if (iResult == PCT_Y)
		{
			if (abs(m_fYSpeed) < 0.01f)
			{
				m_fYSpeed = 0.f;
				m_bAir = false;
			}
			else m_fYSpeed *= 0.5f;
		}
	}

	m_tInfo.fX += m_fXSpeed;
	m_tInfo.fY += m_fYSpeed;
}

void CItem::Render_Slot(HDC hDC, float _fX, float _fY)
{
	GdiTransparentBlt(hDC,
		(int)(_fX - m_tInfo.fCX * 0.5f),
		(int)(_fY - m_tInfo.fCY * 0.5f),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_vecItemDC[m_eItemID][m_iItemSubID * 10 + m_iItemCode],
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));
	
	if (m_eItemID < IT_WEAPON)
	{
		wchar_t wStr[32];
		swprintf(wStr, 32, L"%d", m_iCount);
		TextOut(hDC, _fX, _fY + 8.f, wStr, lstrlen(wStr));
	}
}

// 정보만으로 렌더링하기
void CItem::Render_Slot(HDC hDC, ITEMINFO tItemInfo, float _fX, float _fY, float fScale)
{
	size_t		idx = tItemInfo.iItemSubID * 10 + tItemInfo.iItemCode;
	INFO		tempInfo = CItemMgr::m_mapItemDataBase[ITEMKEY(tItemInfo.eItemID, tItemInfo.iItemSubID, tItemInfo.iItemCode)];

	if (fScale == 1.0f)
	{
		GdiTransparentBlt(hDC,
			(int)(_fX - tempInfo.fCX * 0.5f),
			(int)(_fY - tempInfo.fCY * 0.5f),
			(int)tempInfo.fCX,
			(int)tempInfo.fCY,
			CItemMgr::m_vecItemDC[tItemInfo.eItemID][idx],
			0,
			0,
			(int)tempInfo.fCX,
			(int)tempInfo.fCY,
			RGB(255, 255, 255));
	}
	else
	{
		float scaledCX = tempInfo.fCX * fScale;
		float scaledCY = tempInfo.fCY * fScale;

		float scaledLeft = _fX - scaledCX * 0.5f;
		float scaledTop = _fY - scaledCY * 0.5f;

		GdiTransparentBlt(hDC,
			(int)(scaledLeft),
			(int)(scaledTop),
			(int)(scaledCX),
			(int)(scaledCY),
			CTileMgr::m_hPickedSlotDC,
			0,
			0,
			(int)(tempInfo.fCX),
			(int)(tempInfo.fCY),
			RGB(255, 0, 255));
	}

	wchar_t wStr[32];
	swprintf(wStr, 32, L"%d", tItemInfo.iCount);
	TextOut(hDC, _fX, _fY + 8.f, wStr, lstrlen(wStr));
}

void CItem::Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle)
{
	float	fDistance = sqrtf((m_tInfo.fCX / 2.f) * (m_tInfo.fCX / 2.f) + (m_tInfo.fCY / 2.f) * (m_tInfo.fCY / 2.f));

	// 좌 상단
	m_ptHand[0].x = LONG((m_tInfo.fCX / 2.f) + fDistance * cosf(fHandAngle + PI * 0.75f));
	m_ptHand[0].y = LONG((m_tInfo.fCY / 2.f) - fDistance * sinf(fHandAngle + PI * 0.75f));

	// 우 상단
	m_ptHand[1].x = LONG((m_tInfo.fCX / 2.f) + fDistance * cosf(fHandAngle + PI * 0.25f));
	m_ptHand[1].y = LONG((m_tInfo.fCY / 2.f) - fDistance * sinf(fHandAngle + PI * 0.25f));

	// 좌 하단
	m_ptHand[2].x = LONG((m_tInfo.fCX / 2.f) + fDistance * cosf(fHandAngle + PI * 1.25f));
	m_ptHand[2].y = LONG((m_tInfo.fCY / 2.f) - fDistance * sinf(fHandAngle + PI * 1.25f));

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	size_t	idx = m_iItemSubID * 10 + m_iItemCode;
	INFO	tempInfo = CItemMgr::m_mapItemDataBase[ITEMKEY(m_eItemID, m_iItemSubID, m_iItemCode)];

	PlgBlt(CItemMgr::m_hPlgDC,
		m_ptHand,
		CItemMgr::m_vecItemDC[m_eItemID][idx],
		0, 0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		NULL, NULL, NULL);

	GdiTransparentBlt(hDC,
		(int)(_fX - m_tInfo.fCX * 0.5f + iScrollX),
		(int)(_fY - m_tInfo.fCY * 0.5f + iScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_hPlgDC,
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));

	BitBlt(CItemMgr::m_hPlgDC,
		0, 0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_hResetDC,
		0, 0, SRCCOPY);
}