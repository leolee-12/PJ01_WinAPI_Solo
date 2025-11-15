#include "pch.h"
#include "CObj.h"

CObj::CObj()
	:	m_bDead(false),
		m_pTarget(nullptr),
		m_fXSpeed(0.f),
		m_fYSpeed(0.f),
		m_fAngle(0.f),
		m_bAir(true),
		m_eObjID(OBJ_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_fTime, sizeof(m_fTime));
	m_fTime[TM_GRAVITY] = 1.f / 60.f;
}

CObj::CObj(ITEMINFO tItemInfo)
	: m_bDead(false),
	m_pTarget(nullptr),
	m_fXSpeed(0.f),
	m_fYSpeed(0.f),
	m_fAngle(0.f),
	m_bAir(true),
	m_eObjID(OBJ_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_fTime, sizeof(m_fTime));
	m_fTime[TM_GRAVITY] = 1.f / 60.f;
}

CObj::CObj(EFFECTID eID, int iEffectCode)
	: m_bDead(false),
	m_pTarget(nullptr),
	m_fXSpeed(0.f),
	m_fYSpeed(0.f),
	m_fAngle(0.f),
	m_bAir(true),
	m_eObjID(OBJ_END)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_fTime, sizeof(m_fTime));
	m_fTime[TM_GRAVITY] = 1.f / 60.f;
}

CObj::~CObj()
{
}



void CObj::Update_Rect()
{
	m_tRect.left	= long(m_tInfo.fX - (m_tInfo.fCX / 2.f));
	m_tRect.top		= long(m_tInfo.fY - (m_tInfo.fCY / 2.f));
	m_tRect.right	= long(m_tInfo.fX + (m_tInfo.fCX / 2.f));
	m_tRect.bottom	= long(m_tInfo.fY + (m_tInfo.fCY / 2.f));
}

void CObj::Search_ObjTile(float _fX, float _fY)
{	
	// 바닥 타일의 top과 플레이어의 bottom을 맞추기 위해 "m_tInfo.fX, m_tRect.bottom" 입력
	int idx = 0;
	bool isOdd = (OBJ_TILEX % 2 != 0);

	if (isOdd)
	{
		for (float i = 0.f; i < OBJ_TILEY; i += 1.0f)
		{
			for (float j = -OBJ_TILEX * 0.5f; j < OBJ_TILEX * 0.5f; j += 1.0f)
			{
				int iTempX = (_fX + (TILECX * (j + 0.5f))) / TILECX;
				int iTempY = (_fY - (TILECY * (OBJ_TILEY - 1.f - i))) / TILECY;
				m_vecTile[idx++] = iTempY * TILEX + iTempX;
			}
		}
	}
	else
	{
		for (float i = 0.f; i < OBJ_TILEY; i += 1.0f)
		{
			for (float j = -OBJ_TILEX * 0.5f; j < OBJ_TILEX * 0.5f; j += 1.0f)
			{
				int iTempX = (_fX + (TILECX * (j + 0.5f))) / TILECX;
				int iTempY = (_fY - (TILECY * (OBJ_TILEY - 1.f - i))) / TILECY;
				m_vecTile[idx++] = iTempY * TILEX + iTempX;
			}
		}
	}
}

void CObj::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = 0;
	}
}
