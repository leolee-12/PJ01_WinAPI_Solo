#include "pch.h"
#include "CCharacter.h"

CCharacter::CCharacter()
	:	m_fDistance(0.f),
		m_iMaxHP(100),
		m_iCurHP(100),
		m_dwIvTime(0),
		m_fFallingDistance(0.f)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}

CCharacter::~CCharacter()
{
}

void CCharacter::Initialize()
{
}

int CCharacter::Update()
{
	return 0;
}

void CCharacter::Late_Update()
{
}

void CCharacter::Render(HDC hDC)
{
}

void CCharacter::Release()
{
}

void CCharacter::Gravity()
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
		m_fFallingDistance = 0.f;
	}
}


void CCharacter::OnCollision(COLMSG _colMSG)
{
	Add_PosX(_colMSG.diffX);
	Add_PosY(_colMSG.diffY);
}
