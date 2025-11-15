#include "pch.h"
#include "CItem_Weapon.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBullet.h"
#include "CItemMgr.h"
#include "CTileMgr.h"
#include "CSoundMgr.h"
#include "CMinion.h"

CItem_Weapon::CItem_Weapon()
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
	ZeroMemory(&m_tWeaponStat, sizeof(WPSTAT));
}

CItem_Weapon::CItem_Weapon(ITEMINFO tItemInfo)
	: CItem(tItemInfo)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
	ZeroMemory(&m_tWeaponStat, sizeof(WPSTAT));
}

CItem_Weapon::~CItem_Weapon()
{
}

void CItem_Weapon::Render(HDC hDC)
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
		RGB(255, 255, 255));			// 제거할 픽셀 색상 값

}

void CItem_Weapon::Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle)
{
	float fTempAngle = 0.f;

	if (m_tWeaponStat.eWeaponType == SWING)
	{
		_fX += 32.f * cosf(fHandAngle);
		_fY += 24.f * sinf(fHandAngle);
		fTempAngle = -(fHandAngle + PI * 0.25f);
	}
	else if (m_tWeaponStat.eWeaponType == HAND)
	{
		fTempAngle = m_fAngle;
	}

	int		iRenderSize = (int)(sqrtf((m_tInfo.fCX * m_tInfo.fCX + m_tInfo.fCY * m_tInfo.fCY)));
	float	HalfX = m_tInfo.fCX * 0.5f;
	float	HalfY = m_tInfo.fCY * 0.5f;
	float	cosA = cosf(fTempAngle);
	float	sinA = sinf(fTempAngle);
	// 좌 상단
	m_ptHand[0].x = LONG(75 + (-HalfX * cosA - HalfY * sinA));
	m_ptHand[0].y = LONG(75 + (HalfX * sinA + (-HalfY) * cosA));

	// 우 상단
	m_ptHand[1].x = LONG(75 + (HalfX * cosA - HalfY * sinA));
	m_ptHand[1].y = LONG(75 + (-HalfX * sinA + (-HalfY) * cosA));

	// 좌 하단
	m_ptHand[2].x = LONG(75 + (-HalfX * cosA + HalfY * sinA));
	m_ptHand[2].y = LONG(75 + (HalfX * sinA + HalfY * cosA));

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
		(int)(_fX - iRenderSize * 0.5f + iScrollX),
		(int)(_fY - iRenderSize * 0.5f + iScrollY),
		iRenderSize,
		iRenderSize,
		CItemMgr::m_hPlgDC,
		(int)(75 - iRenderSize * 0.5),
		(int)(75 - iRenderSize * 0.5),
		iRenderSize,
		iRenderSize,
		RGB(255, 255, 255));

	BitBlt(CItemMgr::m_hPlgDC,
		(int)(75 - iRenderSize * 0.5),
		(int)(75 - iRenderSize * 0.5),
		iRenderSize,
		iRenderSize,
		CItemMgr::m_hResetDC,
		0, 0, SRCCOPY);
}


void CItem_Weapon::Use_Item(CObj* pPlayer)
{
	switch (m_tWeaponStat.eWeaponType)
	{
	case SWING:
		Use_SwingWeapon(pPlayer);
		break;
	case STAB:
		Use_StabWeapon(pPlayer);
		break;
	case HOLD:
		Use_HoldWeapon(pPlayer);
		break;
	}
}

void CItem_Weapon::Use_SwingWeapon(CObj* pPlayer)
{
	DWORD tempTime = GetTickCount();

	switch (m_iItemSubID)
	{
	case MELEE:
		break;

	case RANGE:
		break;

	case MAGIC:
		break;

	case SUMMON:
	{	// SlimeStaff
		if (m_fTime[TM_USE] + m_tWeaponStat.fCoolTime < tempTime)
		{
			if (CObjMgr::Get_Instance()->Get_ObjList(MINION)->size() < 3)
			{
				CObjMgr::Get_Instance()->Add_Object(MINION, CAbstractFactory<CMinion>::Create_Obj(pPlayer->Get_Info()->fX, pPlayer->Get_Info()->fY, m_fAngle));
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_ITW3.wav", SOUND_EFFECT4, g_fVolume, 1, m_tWeaponStat.fCoolTime);
				m_fTime[TM_USE] = tempTime;
			}
		}
	}
		break;

	case TOOL:
	{
		if (m_fTime[TM_USE] + m_tWeaponStat.fCoolTime < tempTime)
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

			if (m_iItemCode == 0)
				Use_Pickaxe(iIndex, pt);

			else if (m_iItemCode == 2)
				Use_Axe(iIndex, pt);

			else if (m_iItemCode == 3)
				Use_Hammer(iIndex, pt);

			m_fTime[TM_USE] = tempTime;
		}
	}
		break;
	}

}

void CItem_Weapon::Use_StabWeapon(CObj* pPlayer)
{
}

void CItem_Weapon::Use_HoldWeapon(CObj* pPlayer)
{
	DWORD tempTime = GetTickCount();

	switch (m_iItemSubID)
	{
	case MELEE:
		break;

	case RANGE:
	{
		if (m_fTime[TM_USE] + m_tWeaponStat.fCoolTime < tempTime)
		{
			CObjMgr::Get_Instance()->Add_Object(PLAYER_BULLET, CAbstractFactory<CBullet>::Create_Obj(pPlayer->Get_Info()->fX - 10.f, pPlayer->Get_Info()->fY, m_fAngle));
			CObjMgr::Get_Instance()->Get_ObjList(PLAYER_BULLET)->back()->Set_ObjID(PLAYER_BULLET);

			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_ITW1-1.wav", SOUND_EFFECT4, g_fVolume, 1, m_tWeaponStat.fCoolTime);
			m_fTime[TM_USE] = tempTime;
		}
	}
		break;

	case MAGIC:
		break;

	case SUMMON:
		break;

	case TOOL:
	{	// drill (code : 1)
		if (m_fTime[TM_USE] + m_tWeaponStat.fCoolTime < tempTime)
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

			Use_Pickaxe(iIndex, pt);

			m_fTime[TM_USE] = tempTime;
		}
	}
	break;

	}
}

void CItem_Weapon::Use_Pickaxe(int iIndex, POINT pt)
{
	CTile* tempTile = CTileMgr::Get_Instance()->Get_Tile(iIndex);

	if (tempTile->Get_Option(CTile::TILE_BLOCK) > 0)
	{
		if(tempTile->Get_Option(CTile::TILE_BLOCK) == 2 || tempTile->Get_Option(CTile::TILE_BLOCK) == 3)
			CSoundMgr::Get_Instance()->PlaySound(L"Sound_Tink_0.wav", SOUND_EFFECT3, g_fVolume);
		else
			CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig_2.wav", SOUND_EFFECT3, g_fVolume);

		static_cast<CTileBlock*>(tempTile)->Inflict_Damage(20.f, m_iItemCode);
	}
	else
	{
		for (auto& pFurniture : *CObjMgr::Get_Instance()->Get_ObjList(FURNITURE))
		{
			if (PtInRect(pFurniture->Get_Rect(), pt))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig_2.wav", SOUND_EFFECT3, g_fVolume);

				pFurniture->OnCollision(COLMSG(nullptr, COLTYPE(m_iItemCode), 0, 0));
			}
		}
	}
}

void CItem_Weapon::Use_Axe(int iIndex, POINT pt)
{
	for (auto& pFurniture : *CObjMgr::Get_Instance()->Get_ObjList(FURNITURE))
	{
		if (PtInRect(pFurniture->Get_Rect(), pt))
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig_1.wav", SOUND_EFFECT3, g_fVolume);

			pFurniture->OnCollision(COLMSG(nullptr, COLTYPE(m_iItemCode), 0, 0));
		}
	}
}

void CItem_Weapon::Use_Hammer(int iIndex, POINT pt)
{
	CTile* tempTile = CTileMgr::Get_Instance()->Get_Tile(iIndex);

	if (tempTile->Get_Option(CTile::TILE_WALL) > 0)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Sound_Dig_0.wav", SOUND_EFFECT3, g_fVolume);

		static_cast<CTileBlock*>(tempTile)->Inflict_Damage(20.f, m_iItemCode);
	}
}
