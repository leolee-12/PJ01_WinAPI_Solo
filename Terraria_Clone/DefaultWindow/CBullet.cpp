#include "pch.h"
#include "CBullet.h"
#include "CScrollMgr.h"

CBullet::CBullet()
	:   m_iDamage(20)
{
}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
    m_tInfo.fCX = 10.f;
    m_tInfo.fCY = 10.f;
    m_fXSpeed = 15.f;
    m_fYSpeed = 15.f;
}

int CBullet::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    __super::Update_Rect();

	return OBJ_NOEVENT;
}

void CBullet::Late_Update()
{
    if (0 > m_tRect.left || m_tRect.right > MAPCX ||
        0 > m_tRect.top || m_tRect.bottom > MAPCY)
    {
        m_bDead = true;
    }

    m_tInfo.fX += m_fXSpeed * cosf(m_fAngle);
    m_tInfo.fY -= m_fYSpeed * sinf(m_fAngle);
}


void CBullet::Render(HDC hDC)
{
    int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
    int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

    Ellipse(hDC,
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        m_tRect.right + iScrollX,
        m_tRect.bottom + iScrollY);
}

void CBullet::Release()
{
}

void CBullet::OnCollision(COLMSG _colMSG)
{
	if (_colMSG.pObj->Get_ObjID() == MONSTER)
	{
        m_bDead = true;
		return;
	}
}