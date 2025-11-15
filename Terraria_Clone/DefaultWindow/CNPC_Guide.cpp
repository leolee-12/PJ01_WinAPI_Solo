#include "pch.h"
#include "CNPC_Guide.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"
#include "CEffectMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"

CNPC_Guide::CNPC_Guide()
{
	m_eNPCID = NPC_GUIDE;
}

CNPC_Guide::~CNPC_Guide()
{
	Release();
}

void CNPC_Guide::Initialize()
{
	m_tInfo = { 800.f, 100.f, 40.f, 54.f };
	m_fXSpeed = 0.2f;
	m_fYSpeed = 0.f;
	m_dwIvTime = 500;
	m_eObjID = NPC;

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	m_pFrameKey = L"N01_Guide_RIGHT";

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N01_Guide_LEFT.bmp", L"N01_Guide_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N01_Guide_RIGHT.bmp", L"N01_Guide_RIGHT");
}

int CNPC_Guide::Update()
{
	Status_Check();

	if (m_bDead)
	{
		CEffectMgr::Get_Instance()->Generate_Effect(this, EF_GUIDEDEAD);
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_PlayerKilled.wav", SOUND_EFFECT7, g_fVolume, 1, 100);
		return OBJ_DEAD;
	}

	__super::Update_Rect();

	Motion_Change();

	Gravity();

	Move();

	return OBJ_NOEVENT;
}

void CNPC_Guide::Late_Update()
{
	Pattern_Change();

	if (m_eCurMotion != IDLE)
		Move_Frame();
}

void CNPC_Guide::Render(HDC hDC)
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
		(int)m_tInfo.fCX,			// 복사할 이미지 가로
		(int)m_tInfo.fCY,			// 복사할 이미지 세로
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

void CNPC_Guide::Release()
{
}

void CNPC_Guide::Status_Check()
{
	if (m_iCurHP <= 0)
	{
		m_bDead = true;
		return;
	}

	Search_ObjTile(m_tInfo.fX, m_tRect.bottom);

	if ((CTileMgr::Get_Instance()->Get_Tile(m_vecTile[4] + TILEX)->Get_Option(CTile::TILE_BLOCK) == 0) &&
		(CTileMgr::Get_Instance()->Get_Tile(m_vecTile[5] + TILEX)->Get_Option(CTile::TILE_BLOCK) == 0))
	{
		m_bAir = true;
	}

	if (cosf(m_fAngle) >= 0)
	{
		m_pFrameKey = L"N01_Guide_RIGHT";
	}
	else
	{
		m_pFrameKey = L"N01_Guide_LEFT";
	}

	if (m_eCurMotion == WALK)
		m_fXSpeed = 0.4f * cosf(m_fAngle);
}

void CNPC_Guide::Move_Frame()
{
	// NPC 리소스 세로 1열
	// -> 상태마다 start/end값 다름, iMotion에 시작 start값 저장
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}
