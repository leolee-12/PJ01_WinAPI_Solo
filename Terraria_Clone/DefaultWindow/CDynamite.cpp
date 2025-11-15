#include "pch.h"
#include "CDynamite.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CDynamite::CDynamite()
{
}

CDynamite::~CDynamite()
{
	Release();
}

void CDynamite::Initialize()
{
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;
	m_fXSpeed = 8.f;
	m_fYSpeed = 8.f;
	m_iDamage = 200;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Projectile_Dynamite.bmp", L"Projectile_Dynamite");

	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 7;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 200;

	m_fTime[TM_INVINCIBLE] = GetTickCount();
}

int CDynamite::Update()
{
	if (m_bDead)
	{
		POINT pt = {};
		pt.x = long(m_tInfo.fX);
		pt.y = long(m_tInfo.fY);
		CTileMgr::Get_Instance()->Picking_Bomb(pt, 5);
		CObjMgr::Get_Instance()->Explode(m_tInfo.fX, m_tInfo.fY, 200);
		CEffectMgr::Get_Instance()->Generate_Effect(this, EF_DYNAMITE);
		return OBJ_DEAD;
	}
	else if (m_fTime[TM_INVINCIBLE] + 3000 < GetTickCount())
	{
		m_bDead = true;
		CSoundMgr::Get_Instance()->PlaySound(L"Dynamite.wav", SOUND_EFFECT5, g_fVolume);
		return OBJ_NOEVENT;
	}

	m_fXSpeed *= 0.95f;
	m_fYSpeed *= 0.95f;

	__super::Update_Rect();

	m_tInfo.fX += m_fXSpeed * cosf(m_fAngle);
	m_tInfo.fY -= m_fYSpeed * sinf(m_fAngle);

	return OBJ_NOEVENT;
}

void CDynamite::Late_Update()
{
	Move_Frame();
}


void CDynamite::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Projectile_Dynamite");

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
		RGB(255, 255, 255));			// 제거할 픽셀 색상 값
}

void CDynamite::Release()
{
}

void CDynamite::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}