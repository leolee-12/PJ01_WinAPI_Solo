#include "pch.h"
#include "CBullet_B.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"

CBullet_B::CBullet_B()
{
}

CBullet_B::~CBullet_B()
{
	Release();
}

void CBullet_B::Initialize()
{
	m_tInfo.fCX = 15.f;
	m_tInfo.fCY = 27.f;
	m_fXSpeed = 0.f;
	m_fYSpeed = -15.f;
	m_iDamage = 5;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Projectile_BulletB.bmp", L"Projectile_BulletB");
}

int CBullet_B::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	Gravity();

	if (m_fYSpeed < 0)
	{
		m_tFrame.iStart = 0;
	}
	else
	{
		m_tFrame.iStart = 1;
	}

	m_tInfo.fX += m_fXSpeed;
	m_tInfo.fY += m_fYSpeed;

	return OBJ_NOEVENT;
}

void CBullet_B::Late_Update()
{
	if (0 > m_tRect.left || m_tRect.right > MAPCX ||
		0 > m_tRect.top || m_tRect.bottom > MAPCY)
	{
		m_bDead = true;
	}
}


void CBullet_B::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Projectile_BulletB");

	//Rectangle(hDC,
	//	m_tRect.left + iScrollX,
	//	m_tRect.top + iScrollY,
	//	m_tRect.right + iScrollX,
	//	m_tRect.bottom + iScrollY);

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

void CBullet_B::Release()
{
}

void CBullet_B::OnCollision(COLMSG _colMSG)
{
	if (_colMSG.pObj->Get_ObjID() == PLAYER)
	{
		m_bDead = true;
		return;
	}
}

void CBullet_B::Gravity()
{
	if (m_bAir == true)
	{
		m_fYSpeed += 24.f * m_fTime[TM_GRAVITY];

		if (m_fYSpeed >= 600.f)
			m_fYSpeed = 600.f;
	}
	else
	{
		m_fYSpeed = 0.f;
	}
}