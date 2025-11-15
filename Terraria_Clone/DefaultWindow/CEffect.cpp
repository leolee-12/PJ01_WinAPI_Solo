#include "pch.h"
#include "CEffect.h"
#include "CEffectMgr.h"
#include "CScrollMgr.h"

CEffect::CEffect()
	:	m_eEffectID(EF_END),
		m_iEffectCode(0),
		m_fLifeTime(0.f)
{
}

CEffect::CEffect(EFFECTID eID, int iEffectCode)
	: m_eEffectID(eID),
	m_iEffectCode(iEffectCode),
	m_fLifeTime(0.f)
{
}

CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
	m_fLifeTime = 3000;

	CEffectMgr::Get_Instance()->Set_EffectInfo(this);
	m_fYSpeed = -4.f;
	m_fTime[TM_INVINCIBLE] = GetTickCount();
}

int	CEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;
	else if (m_fTime[TM_INVINCIBLE] + m_fLifeTime < GetTickCount())
		m_bDead = true;

	__super::Update_Rect();

	Gravity();

	m_tInfo.fX += m_fXSpeed;
	m_tInfo.fY += m_fYSpeed;

	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
}

void CEffect::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CEffectMgr::m_vecEffectDC[m_eEffectID][m_iEffectCode],
		0,
		(int)((m_tInfo.fCY + 2.f) * m_tFrame.iStart),
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));			// 제거할 픽셀 색상 값
}

void CEffect::Release()
{

}

void CEffect::OnCollision(COLMSG _colMSG)
{

}

void CEffect::Gravity()
{
	if ((m_eEffectID == EF_DYNAMITE) && (m_iEffectCode <= 2))
		return;

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