#include "pch.h"
#include "CNPC_HintBox.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"

CNPC_HintBox::CNPC_HintBox()
{
	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);
}

CNPC_HintBox::~CNPC_HintBox()
{
	Release();
}

void CNPC_HintBox::Initialize()
{
	m_tInfo.fCX = 44.f;
	m_tInfo.fCY = 44.f;
	m_fXSpeed = 0.f;
	m_fYSpeed = 0.f;
	m_dwIvTime = 500;
	m_eObjID = NPC;

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N00_HintBox.bmp", L"N00_HintBox");
	m_pFrameKey = L"N00_HintBox";
}

int CNPC_HintBox::Update()
{
	Status_Check();

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CNPC_HintBox::Late_Update()
{
	Pattern_Change();

	if (m_eCurMotion != IDLE)
		Move_Frame();
}

void CNPC_HintBox::Render(HDC hDC)
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

void CNPC_HintBox::Release()
{
}

void CNPC_HintBox::Status_Check()
{
	if (m_iCurHP <= 0)
	{
		m_bDead = true;
		return;
	}

	Search_ObjTile(m_tInfo.fX, m_tRect.bottom);
}