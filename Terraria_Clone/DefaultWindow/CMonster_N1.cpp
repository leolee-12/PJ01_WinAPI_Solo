#include "pch.h"
#include "CMonster_N1.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CEffectMgr.h"
#include "CMinion.h"
#include "CItemMgr.h"
#include "CItem_Nonconsumable.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"

CMonster_N1::CMonster_N1()
{
	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);
}

CMonster_N1::~CMonster_N1()
{
	Release();
}

void CMonster_N1::Initialize()
{
	m_tInfo.fCX = 38.f;
	m_tInfo.fCY = 46.f;
	m_fXSpeed = 0.2f;
	m_fYSpeed = 0.f;
	m_dwIvTime = 100;
	m_eObjID = MONSTER;

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	m_pFrameKey = L"M01_Zombie_RIGHT";

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/M01_Zombie_LEFT.bmp", L"M01_Zombie_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/M01_Zombie_RIGHT.bmp", L"M01_Zombie_RIGHT");
}

int CMonster_N1::Update()
{
	if (m_bDead)
	{
		CItemMgr::Get_Instance()->Add_Item(IT_NONCONSUMABLE, CAbstractFactory<CItem_Nonconsumable>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_NONCONSUMABLE, 0, rand() % 4 + 1, 1)));
		return OBJ_DEAD;
	}
	else if (m_iCurHP <= 0)
	{
		m_bDead = true;
		CEffectMgr::Get_Instance()->Generate_Effect(this, EF_MONSTERDEAD);
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_ZombieKilled.wav", SOUND_EFFECT2, g_fVolume, 1, 100);
		return OBJ_NOEVENT;
	}

	Status_Check();

	__super::Update_Rect();

	Motion_Change();

	Gravity();

	Move();

	return OBJ_NOEVENT;
}

void CMonster_N1::Late_Update()
{

	if (m_eCurMotion != JUMP)
		Move_Frame();
}

void CMonster_N1::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		(int)((m_tInfo.fCY + 2.f) * m_tFrame.iStart),
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	if (m_fTime[TM_INVINCIBLE] + m_dwIvTime > GetTickCount())
	{
		HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, redBrush);

		Rectangle(hDC,
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY,
			m_tRect.right + iScrollX,
			m_tRect.bottom + iScrollY);

		SelectObject(hDC, oldBrush);
		DeleteObject(redBrush);
	}
}

void CMonster_N1::Release()
{
}

void CMonster_N1::Status_Check()
{
	Search_ObjTile(m_tInfo.fX, m_tRect.bottom);

	if ((CTileMgr::Get_Instance()->Get_Tile(m_vecTile[2] + TILEX)->Get_Option(CTile::TILE_BLOCK) == 0) &&
		(CTileMgr::Get_Instance()->Get_Tile(m_vecTile[3] + TILEX)->Get_Option(CTile::TILE_BLOCK) == 0))
	{
		m_bAir = true;
	}

	if (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty())
		m_pTarget = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();

	Search_TargetDir();

	if (cosf(m_fAngle) >= 0)
	{
		m_pFrameKey = L"M01_Zombie_RIGHT";
	}
	else
	{
		m_pFrameKey = L"M01_Zombie_LEFT";
	}

	if ((m_fTime[TM_INVINCIBLE] + 200 < GetTickCount()) && (m_eCurMotion == WALK))
		m_fXSpeed = 0.2f * cosf(m_fAngle) / abs(cosf(m_fAngle));
}

void CMonster_N1::OnCollision(COLMSG _colMSG)
{
	if ((_colMSG.pObj->Get_ObjID() == MINION) && (m_fTime[TM_BUFF] + 30 < GetTickCount()))
	{
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_MonsterHit.wav", SOUND_EFFECT2, g_fVolume, 1, 100);
		m_iCurHP -= static_cast<CMinion*>(_colMSG.pObj)->Get_Damage();
		m_fTime[TM_BUFF] = GetTickCount();
		m_bAir = true;
		m_fYSpeed = -0.5f;
		return;
	}

	if ((_colMSG.pObj->Get_ObjID() == PLAYER_BULLET) && (m_fTime[TM_INVINCIBLE] + 30 < GetTickCount()))
	{
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_MonsterHit.wav", SOUND_EFFECT2, g_fVolume, 1, 100);
		m_iCurHP -= static_cast<CBullet*>(_colMSG.pObj)->Get_Damage();
		m_fTime[TM_INVINCIBLE] = GetTickCount();

		if (_colMSG.eColType == COLTYPE::CD_LEFT)
			m_fXSpeed = 5.f;
		else if (_colMSG.eColType == COLTYPE::CD_RIGHT)
			m_fXSpeed = -5.f;

		m_bAir = true;
		m_fYSpeed = -3.f;
		return;
	}
}

void CMonster_N1::Motion_Change()
{
	if (m_ePreMotion != m_eCurMotion)
	{
		switch (m_eCurMotion)
		{
		case IDLE:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100000;
			break;

		case WALK:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 2;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case JUMP:
			//m_tFrame.iStart = 0;
			//m_tFrame.iEnd = 2;
			//m_tFrame.iMotion = m_tFrame.iStart;
			//m_tFrame.dwTime = GetTickCount();
			//m_tFrame.dwSpeed = 100;
			break;

		case DEAD:
			//m_tFrame.iStart = 0;
			//m_tFrame.iEnd = 3;
			//m_tFrame.iMotion = m_tFrame.iStart;
			//m_tFrame.dwTime = GetTickCount();
			//m_tFrame.dwSpeed = 300;
			break;
		}

		m_ePreMotion = m_eCurMotion;
	}
}

void CMonster_N1::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}