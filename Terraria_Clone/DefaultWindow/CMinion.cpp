#include "pch.h"
#include "CMinion.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CEffectMgr.h"
using namespace TileConst;

CMinion::CMinion()
	:	m_ePreMotion(IDLE),
		m_eCurMotion(WALK),
		m_iDamage(5),
		m_fTargetDistance(6400.f)
{
}

CMinion::~CMinion()
{
	Release();
}

void CMinion::Initialize()
{
	m_tInfo.fCX = 44.f;
	m_tInfo.fCY = 28.f;
	m_fXSpeed = 0.2f;
	m_fYSpeed = 0.f;
	m_dwIvTime = 100;
	m_eObjID = MINION;

	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	m_pFrameKey = L"P01_BabySlime_RIGHT";
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 1;
	m_tFrame.iMotion = m_tFrame.iStart;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/P01_BabySlime_LEFT.bmp", L"P01_BabySlime_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/P01_BabySlime_RIGHT.bmp", L"P01_BabySlime_RIGHT");

	m_fTime[TM_INVINCIBLE] = GetTickCount();
}

int CMinion::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}
	else if (m_fTime[TM_INVINCIBLE] + 60000 < GetTickCount())
	{
		m_bDead = true;
		return OBJ_NOEVENT;
	}

	Status_Check();

	__super::Update_Rect();

	Motion_Change();

	Gravity();

	Move();

	return OBJ_NOEVENT;
}

void CMinion::Late_Update()
{
	Move_Frame();
}

void CMinion::Render(HDC hDC)
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
}

void CMinion::Release()
{
}

void CMinion::Status_Check()
{
	Search_ObjTile(m_tInfo.fX, m_tRect.bottom);

	CTile* pTile1 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[0] + TILEX);
	CTile* pTile2 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[1] + TILEX);

	if(pTile1 == nullptr || pTile2 == nullptr) return;

	if ((pTile1->Get_Option(CTile::TILE_BLOCK) == BLOCK_NONE) &&
		(pTile2->Get_Option(CTile::TILE_BLOCK) == BLOCK_NONE))
	{
		m_bAir = true;
	}

	m_pTarget = CCollisionMgr::Get_NearMonster(this, *CObjMgr::Get_Instance()->Get_ObjList(MONSTER));
	
	if (m_pTarget)
	{
		m_eCurMotion = ATTACK;
	}
	else
	{
		if (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty())
		{
			m_eCurMotion = WALK;
			m_pTarget = CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front();
		}
	}

	if (m_pTarget)
	{
		float fX = m_pTarget->Get_Info()->fX - m_tInfo.fX;
		float fY = m_pTarget->Get_Info()->fY - m_tInfo.fY;
		m_fTargetDistance = fX * fX + fY * fY;
	}
	else
	{
		m_eCurMotion = WALK;
		m_fTargetDistance = 6400.f;
	}
	if ((m_eCurMotion != ATTACK) && (m_fTargetDistance > 160000.f))
	{
		m_tInfo.fX = m_pTarget->Get_Info()->fX;
		m_tInfo.fY = m_pTarget->Get_Info()->fY;
	}

	Search_TargetDir();

	if (cosf(m_fAngle) >= 0)
	{
		m_pFrameKey = L"P01_BabySlime_RIGHT";
	}
	else
	{
		m_pFrameKey = L"P01_BabySlime_LEFT";
	}

	if (m_fTime[TM_PATTERN] + 200 < GetTickCount())
	{
		if (m_eCurMotion == ATTACK)
		{
			m_fXSpeed = 12.f * cosf(m_fAngle);
			m_fYSpeed = -12.f * sinf(m_fAngle);
		}
		else
		{
			m_fXSpeed = 4.f * cosf(m_fAngle);
			m_fYSpeed = -4.f * sinf(m_fAngle);
		}
		m_fTime[TM_PATTERN] = GetTickCount();
	}
}

void CMinion::OnCollision(COLMSG _colMSG)
{
	if (_colMSG.pObj->Get_ObjID() == MONSTER)
	{
		return;
	}
}

void CMinion::Motion_Change()
{
	if (m_ePreMotion != m_eCurMotion)
	{
		switch (m_eCurMotion)
		{
		case IDLE:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 1;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case WALK:
			m_tFrame.iStart = 2;
			m_tFrame.iEnd = 5;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case ATTACK:
			m_tFrame.iStart = 2;
			m_tFrame.iEnd = 5;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 200;
			break;

		case DEAD:
			break;
		}

		m_ePreMotion = m_eCurMotion;
	}
}

void CMinion::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}

void CMinion::Search_TargetDir()
{
	if (m_pTarget)
	{
		float fOffset = 0.f;

		if (m_eCurMotion == WALK)
		{
			if (cosf(m_pTarget->Get_Angle()) >= 0)
				fOffset = -32.f;
			else
				fOffset = 32.f;
		}
		else if (m_eCurMotion == ATTACK)
		{
			fOffset = 0.f;
		}

		float	fWidth(0.f), fHeight(0.f);
		fWidth = m_pTarget->Get_Info()->fX - m_tInfo.fX + fOffset;
		fHeight = m_pTarget->Get_Info()->fY - m_tInfo.fY - 0.5f * fOffset;
		m_fAngle = atan2f(-fHeight, fWidth);
	}
}

void CMinion::Move()
{
	while (m_eCurMotion != ATTACK)
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