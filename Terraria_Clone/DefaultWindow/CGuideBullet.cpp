#include "pch.h"
#include "CGuideBullet.h"
#include "CObjMgr.h"

CGuideBullet::CGuideBullet()
{
}

CGuideBullet::~CGuideBullet()
{
    Release();
}

void CGuideBullet::Initialize()
{
    m_tInfo.fCX = 30.f;
    m_tInfo.fCY = 30.f;
    m_fSpeed = 5.f;
}

int CGuideBullet::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    m_pTarget = CObjMgr::Get_Instance()->Get_Target(MONSTER, this);
    
    if (m_pTarget)
    {
        float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f);

        fWidth = m_pTarget->Get_Info()->fX - m_tInfo.fX;
        fHeight = m_pTarget->Get_Info()->fY - m_tInfo.fY;

        fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

        float fRadian = acosf(fWidth / fDiagonal);

        m_fAngle = fRadian * 180.f / PI;

        if (m_tInfo.fY < m_pTarget->Get_Info()->fY)
            m_fAngle *= -1.f;
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CGuideBullet::Late_Update()
{
    m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
    m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));
}

void CGuideBullet::Render(HDC hDC)
{
    Ellipse(hDC,
        m_tRect.left,
        m_tRect.top,
        m_tRect.right,
        m_tRect.bottom);
}

void CGuideBullet::Release()
{
}
