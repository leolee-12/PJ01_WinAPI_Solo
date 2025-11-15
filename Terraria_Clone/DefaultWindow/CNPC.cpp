#include "pch.h"
#include "CNPC.h"
#include "CBmpMgr.h"
#include "CScrollMgr.h"
#include "CTileMgr.h"
#include "CObjMgr.h"
#include "CBullet.h"
#include "CCollisionMgr.h"
#include "CUIMgr.h"
#include "CInven_Bag.h"
#include "CPlayer.h"
#include "CSoundMgr.h"

CNPC::CNPC()
	:	m_ePreMotion(NS_END),
		m_eCurMotion(IDLE),
		m_eNPCID(NPC_END),
		m_iMessageCount(0)
{
	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);
}

CNPC::~CNPC()
{
	Release();
}

void CNPC::Initialize()
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

int CNPC::Update()
{
	Status_Check();

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();

	Motion_Change();

	Gravity();

	Move();

	return OBJ_NOEVENT;
}

void CNPC::Late_Update()
{
	Pattern_Change();

	if (m_eCurMotion != IDLE)
		Move_Frame();
}

void CNPC::Render(HDC hDC)
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

void CNPC::Release()
{
}

void CNPC::Status_Check()
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

void CNPC::OnCollision(COLMSG _colMSG)
{
	if (_colMSG.pObj->Get_ObjID() == PLAYER)
	{
		return;
	}

	if ((_colMSG.pObj->Get_ObjID() == MONSTER_BULLET) && (m_fTime[TM_INVINCIBLE] + 30 < GetTickCount()))
	{
		m_iCurHP -= static_cast<CBullet*>(_colMSG.pObj)->Get_Damage();
		m_fTime[TM_INVINCIBLE] = GetTickCount();

		if (_colMSG.eColType == COLTYPE::CD_LEFT)
			m_fXSpeed = 5.f;
		else if (_colMSG.eColType == COLTYPE::CD_RIGHT)
			m_fXSpeed = -5.f;

		m_bAir = true;
		m_fYSpeed = -3.f;
		return;
	}
}

void CNPC::Motion_Change()
{
	if (m_ePreMotion != m_eCurMotion)
	{
		switch (m_eCurMotion)
		{
		case IDLE:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 0;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100000;
			break;

		case WALK:
			m_tFrame.iStart = 1;
			m_tFrame.iEnd = 14;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100;
			break;

		case JUMP:
			m_tFrame.iStart = 1;
			m_tFrame.iEnd = 1;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 300;
			break;

		case INTERACT:
			break;

		case DEAD:
			m_tFrame.iStart = 0;
			m_tFrame.iEnd = 3;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 300;
			break;
		}

		m_ePreMotion = m_eCurMotion;
	}
}

void CNPC::Move_Frame()
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

void CNPC::Move()
{
	while (true)
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
				m_eCurMotion = WALK;
			}
			else m_fYSpeed *= 0.5f;
		}
	}

	m_tInfo.fX += m_fXSpeed;
	m_tInfo.fY += m_fYSpeed;
}

void CNPC::NPC_Function(NPCID _eID)
{
	switch (_eID)
	{
	case NPC_MERCHANT:
	{
		g_bShop = !g_bShop;
	}
	break;

	case NPC_NURSE:
	{
		CInven_Bag* tempInven = static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG));

		if (!tempInven->Check_Item(ITEMINFO(2, 0, 1, 1)))
			return;

		CPlayer* tempPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front());

		if (tempPlayer->Get_MaxHP() == tempPlayer->Get_HP())
			return;

		tempPlayer->Set_HP(tempPlayer->Get_MaxHP());
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Heal.wav", SOUND_EFFECT7, g_fVolume, 1, 100);
		tempInven->Remove_Item(ITEMINFO{ 2, 0, 1, 1 });
	}
	break;

	default:
		break;
	}
}

void CNPC::Pattern_Change()
{
	DWORD dwTemp = GetTickCount();

	switch (m_eCurMotion)
	{
	case IDLE:
	{
		if (m_fTime[TM_PATTERN] + 3000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = WALK;
		}
	}
	break;

	case WALK:
	{
		if (m_fTime[TM_PATTERN] + 3000 < dwTemp)
		{
			m_fTime[TM_PATTERN] = dwTemp;
			m_eCurMotion = IDLE;
			m_fXSpeed = 0.f;
		}
	}
	break;
	}

	if ((m_eCurMotion == IDLE) && (m_fTime[TM_USE] + 10000 < dwTemp))
	{
		m_fTime[TM_USE] = dwTemp;
		m_fAngle += PI;
	}
}

void CNPC::Interaction()
{
	CUIMgr::Get_Instance()->Get_UI(UIS_NPCBUTTON)->Set_Data(m_eNPCID);
	CUIMgr::Get_Instance()->Get_UI(UIS_MESSAGEBOX)->Set_Data(m_eNPCID * 10 + m_iMessageCount);
}