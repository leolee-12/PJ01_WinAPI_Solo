#include "pch.h"
#include "CMonster_Boss.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
#include "CAbstractFactory.h"
#include "CMonster_N1.h"
#include "CBullet_B.h"
#include "CEffectMgr.h"
#include "CItem_Weapon.h"
#include "CItemMgr.h"

#include "CNPC_Guide.h"
#include "CSoundMgr.h"

CMonster_Boss::CMonster_Boss()
	:	m_ePattern(BP_NONE1),
		m_fScale(1.0f)
{
	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);
}

CMonster_Boss::~CMonster_Boss()
{
	Release();
}

void CMonster_Boss::Initialize()
{
	m_tInfo.fCX = 166.f;
	m_tInfo.fCY = 118.f;
	m_fXSpeed = 0.5f;
	m_fYSpeed = 0.f;
	m_iCurHP = m_iMaxHP = 2000;
	m_dwIvTime = 100;
	m_eObjID = MONSTER;

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	m_pFrameKey = L"M04_KingSlime";

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/M04_KingSlime.bmp", L"M04_KingSlime");

	m_fTime[TM_PATTERN] = GetTickCount();
}

int CMonster_Boss::Update()
{
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_KingSlimeKilled.wav", SOUND_EFFECT2, g_fVolume, 1, 100);
		CItemMgr::Get_Instance()->Add_Item(IT_WEAPON, CAbstractFactory<CItem_Weapon>::Create_Item(m_tInfo.fX, m_tInfo.fY, ITEMINFO(IT_WEAPON, 3, 0, 1)));

		if (CObjMgr::Get_Instance()->Get_ObjList(NPC)->empty())
		{
			CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Guide>::Create_Obj(m_tInfo.fX, m_tInfo.fY));
		}

		return OBJ_DEAD;
	}
	else if (m_iCurHP <= 0)
	{
		m_bDead = true;
		CEffectMgr::Get_Instance()->Generate_Effect(this, EF_BOSSDEAD);
		return OBJ_NOEVENT;
	}

	Status_Check();

	__super::Update_Rect();

	Motion_Change();

	Gravity();

	Pattern_Move();

	return OBJ_NOEVENT;
}

void CMonster_Boss::Late_Update()
{
	Move_Frame();

	if(CUIMgr::Get_Instance()->Get_UI(UIS_BOSSHPBAR))
		CUIMgr::Get_Instance()->Get_UI(UIS_BOSSHPBAR)->Set_Data(m_iCurHP);
}

void CMonster_Boss::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		(int)(m_tInfo.fX - (m_tInfo.fCX * 0.5f * m_fScale) + iScrollX),
		(int)(m_tInfo.fY - (m_tInfo.fCY * 0.5f * m_fScale) + iScrollY),
		(int)(m_tInfo.fCX * m_fScale),
		(int)(m_tInfo.fCY * m_fScale),
		hMemDC,
		0,
		(int)((m_tInfo.fCY + 2.f) * m_tFrame.iStart),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));

	if (m_fTime[TM_INVINCIBLE] + m_dwIvTime > GetTickCount())
	{
		HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, redBrush);

		Rectangle(hDC,
			(m_tInfo.fX - m_tInfo.fCX * 0.5f * m_fScale) + iScrollX,
			(m_tInfo.fY - m_tInfo.fCY * 0.5f * m_fScale) + iScrollY,
			(m_tInfo.fX + m_tInfo.fCX * 0.5f * m_fScale) + iScrollX,
			(m_tInfo.fY + m_tInfo.fCY * 0.5f * m_fScale) + iScrollY);

		SelectObject(hDC, oldBrush);
		DeleteObject(redBrush);
	}

	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"N01_Guide_RIGHT");

	GdiTransparentBlt(hDC,
		m_tInfo.fX - 20.f + iScrollX,
		m_tInfo.fY - 28.f + iScrollY,
		40,
		56,
		hMemDC,
		0,
		58,
		40,		// 복사할 이미지 가로
		56,		// 복사할 이미지 세로
		RGB(255, 0, 255));		// 제거할 픽셀 색상 값
}

void CMonster_Boss::Release()
{
}

void CMonster_Boss::Status_Check()
{
	Search_ObjTile(m_tInfo.fX, m_tRect.bottom);

	int iTemp = 0;
	int iAirLimit = OBJ_TILEX * 0.75f;
	int len = OBJ_TILEX * OBJ_TILEY;

	for(int i = 0; i < OBJ_TILEX; ++i)
	{
		CTile* pTile = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[len - 1 - i] + TILEX);

		if(pTile == nullptr) continue;

		if (pTile->Get_Option(CTile::TILE_BLOCK) == 0) iTemp++;

		if (iTemp >= iAirLimit)
		{
			m_bAir = true;
			break;
		}
	}

	if (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty())
		m_pTarget = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();

	Search_TargetDir();
}

void CMonster_Boss::OnCollision(COLMSG _colMSG)
{
	if (_colMSG.pObj->Get_ObjID() == PLAYER)
	{
		return;
	}

	if ((_colMSG.pObj->Get_ObjID() == PLAYER_BULLET) && (m_fTime[TM_INVINCIBLE] + 30 < GetTickCount()))
	{
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_MonsterHit.wav", SOUND_EFFECT2, g_fVolume, 1, 100);
		m_iCurHP -= static_cast<CBullet*>(_colMSG.pObj)->Get_Damage();
		m_fTime[TM_INVINCIBLE] = GetTickCount();

		if (_colMSG.eColType == COLTYPE::CD_LEFT)
		{
			//m_fXSpeed = 0.1f;
		}
		else if (_colMSG.eColType == COLTYPE::CD_RIGHT)
		{
			//m_fXSpeed = -0.1f;
		}
		m_bAir = true;
		m_fYSpeed = -0.1f;
		return;
	}
}

void CMonster_Boss::Motion_Change()
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
			m_tFrame.dwSpeed = 100;
			break;

		case JUMP:
			m_tFrame.iStart = 3;
			m_tFrame.iEnd = 5;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
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

void CMonster_Boss::Pattern_Move()
{
	DWORD dwTemp = GetTickCount();
	srand(dwTemp);

	switch (m_ePattern)
	{
	case BP_NONE1:
	{
		if (m_eCurMotion != JUMP)
		{
			m_bAir = true;
			m_eCurMotion = JUMP;
			m_fXSpeed += 5.f * cosf(m_fAngle) / abs(cosf(m_fAngle));
			m_fYSpeed -= 10.f;
		}

		Move();

		if (m_fTime[TM_PATTERN] + 4000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = SKILL1;
			Pattern_Change(BP_SKILL1);
		}
	}
	break;

	case BP_SKILL1:		// Teleport
	{
		m_fXSpeed = 0.f;
		m_fYSpeed = 0.f;

		if ((m_eCurMotion == SKILL1) && (m_fScale > 0.5f))
		{
			m_fScale *= 0.98f;

			if (m_fScale <= 0.5f)
				m_eCurMotion = IDLE;
		}
		else
		{
			if (m_eCurMotion == IDLE)
			{
				m_tInfo.fX = m_pTarget->Get_Info()->fX;
				m_tInfo.fY = m_pTarget->Get_Info()->fY + (m_pTarget->Get_Info()->fCY - m_tInfo.fCY) * 0.5f;
				m_eCurMotion = WALK;
			}
			else if(m_fScale < 1.f)
			{
				m_fScale *= 1.1f;
			}
			else if (m_fScale > 1.f)
			{
				m_fScale = 1.0f;
				Move();
			}
		}

		if (m_fTime[TM_PATTERN] + 1000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = SKILL2;
			Pattern_Change(BP_SKILL2);
		}
	}
	break;

	case BP_SKILL2:
	{
		if (m_eCurMotion != JUMP)
		{
			m_bAir = true;
			m_eCurMotion = JUMP;
			m_fYSpeed -= 10.f;
		}

		if (m_fTime[TM_USE] + 700 < dwTemp)
		{
			CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CMonster_N1>::Create_Obj(m_tInfo.fX, m_tInfo.fY));
			CObjMgr::Get_Instance()->Get_ObjList(MONSTER)->back()->Add_XSpeed(3.f * ((rand() % 3) - 1.f));
			CObjMgr::Get_Instance()->Get_ObjList(MONSTER)->back()->Add_YSpeed(-15.f);
			m_fTime[TM_USE] = dwTemp;
		}

		Move();

		if (m_fTime[TM_PATTERN] + 2000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			Pattern_Change(BP_NONE2);
		}
	}
	break;

	case BP_NONE2:
	{
		if (m_eCurMotion != JUMP)
		{
			m_bAir = true;
			m_eCurMotion = JUMP;
			m_fXSpeed += 5.f * cosf(m_fAngle) / abs(cosf(m_fAngle));
			m_fYSpeed -= 10.f;
		}

		Move();

		if (m_fTime[TM_PATTERN] + 4000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = WALK;
			Pattern_Change(BP_SKILL3);
		}
	}
	break;

	case BP_SKILL3:
	{
		m_fXSpeed = 0.f;
		m_fYSpeed = 0.f;


		if (m_fTime[TM_USE] + 100 < dwTemp)
		{
			float fTemp = ((rand() % 61) + 60) * PI / 180.f;
			float fXTemp = (rand() % 10) - 5.f;
			CObjMgr::Get_Instance()->Add_Object(MONSTER_BULLET, CAbstractFactory<CBullet_B>::Create_Obj(m_tInfo.fX, m_tInfo.fY));
			CObjMgr::Get_Instance()->Get_ObjList(MONSTER_BULLET)->back()->Add_XSpeed(fXTemp);

			m_fTime[TM_USE] = dwTemp;
		}

		Move();

		if (m_fTime[TM_PATTERN] + 1000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = WALK;
			Pattern_Change(BP_NONE1);
		}
	}
	break;
	}
}

void CMonster_Boss::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}

void CMonster_Boss::Pattern_Change()
{

}