#include "pch.h"
#include "CPlayer.h"
#include "CAbstractFactory.h"
#include "CBullet.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CUIMgr.h"
#include "CHPBar.h"
#include "CMonster.h"
#include "CInven_Bag.h"
#include "CSoundMgr.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CTileMgr.h"
#include "CItem.h"
#include "CText.h"
#include "CItemMgr.h"
#include "CEffectMgr.h"
#include "CPlayerDeadText.h"
#include "CNPC.h"
#include "CMouse.h"
#include "CMainGame.h"
#include "CMonster_N1.h"

CPlayer::CPlayer()
	:	m_ePreMotion(PS_END),
		m_eCurMotion(IDLE),
		m_bUsing(false),
		m_fMaxSpeed(3.f),
		m_fMaxJumpSpeed(10.f),
		m_eJumpState(CANJUMP),
		m_pHandItem(nullptr),
		m_bGravityPotion(false),
		m_iGravityDir(1),
		m_fHandAngle(-PI * 0.75f)
{
	m_vecTile.reserve(OBJ_TILEX * OBJ_TILEY);
	ZeroMemory(&m_tHandRect, sizeof(RECT));
}

CPlayer::~CPlayer()
{
	Release();
}



void CPlayer::Initialize()
{
	m_tInfo = { 200.f, float(WINCY >> 1), 40.f, 54.f };
	//m_tInfo = { 200.f, 1300.f, 40.f, 54.f };
	m_fXSpeed = 0.f;
	m_fYSpeed = 0.f;
	m_fAngle = 0.f;
	m_fDistance = 100.f;
	m_dwIvTime = 500;

	for (int i = 0; i < OBJ_TILEX * OBJ_TILEY; ++i)
	{
		m_vecTile.push_back(0);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/PlayerNew_LEFT.bmp", L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/PlayerNew_RIGHT.bmp", L"Player_RIGHT");
	m_pFrameKey = L"Player_RIGHT";
	m_eCurMotion = IDLE;
	m_tFrame.iStart = 0;
	m_tFrame.iEnd = 0;
	m_tFrame.iMotion = 0;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 300;
}

int CPlayer::Update()
{
	if (m_bDead)
	{
		if (m_fTime[TM_RESPAWN] + 5000 < GetTickCount())
		{
			m_bDead = false;
			m_iCurHP = m_iMaxHP * 0.5f;
			m_eCurMotion = IDLE;
			m_tInfo.fX = 400.f;
			m_tInfo.fY = float(WINCY >> 1);
			m_fFallingDistance = 0.f;
		}
		return OBJ_NOEVENT;
	}
	else if (m_iCurHP <= 0)
	{
		m_eCurMotion = DEAD;
		m_bDead = true;
		CEffectMgr::Get_Instance()->Generate_Effect(this, EF_PLAYERDEAD);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_Killed.wav", SOUND_EFFECT4, g_fVolume);
		CUIMgr::Get_Instance()->Add_UI(UIS_PLAYERDEADTEXT, CAbstractFactory<CPlayerDeadText>::Create_UI());
		m_fTime[TM_RESPAWN] = GetTickCount();
		return OBJ_NOEVENT;
	}

	Status_Check();

	__super::Update_Rect();

	Key_Input();

	Motion_Change();

	Gravity();

	Move();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	Offset();

	Move_Frame();

	if (m_iCurHP > 0)
	{
		CUIMgr::Get_Instance()->Get_UI(UIS_HPBAR)->Set_Data(m_iCurHP);
	}
	else
	{
		CUIMgr::Get_Instance()->Get_UI(UIS_HPBAR)->Set_Data(0);
	}
}

void CPlayer::Render(HDC hDC)
{
	if (m_eCurMotion == DEAD)
		return;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

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
		RGB(255, 0, 255));		// 제거할 픽셀 색상 값

	if ((m_bUsing) && (m_pHandItem))
	{
		bool isRight = cosf(m_fAngle) >= 0;
		
		if (isRight)
		{
			m_fHandAngle += PI / 15.f;
			if (m_fHandAngle >= PI * 0.25f) m_fHandAngle = -PI * 0.75f;
		}
		else
		{
			m_fHandAngle -= PI / 15.f;
			if (m_fHandAngle <= -PI * 1.25f) m_fHandAngle = -PI * 0.25f;
		}

		m_pHandItem->Render_Hand(hDC, m_tInfo.fX, m_tInfo.fY, m_fHandAngle);
	}

	if (m_fTime[TM_INVINCIBLE] + 30 > GetTickCount())
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

void CPlayer::Release()
{
}



void CPlayer::Key_Input()
{
	MoveKey_Input();

	OptionKey_Input();

	MotionKey_Input();
}

void CPlayer::MoveKey_Input()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pFrameKey = L"Player_LEFT";

		if (m_fXSpeed > -m_fMaxSpeed)
			m_fXSpeed -= 0.2f;

		if (!m_bAir)
			m_eCurMotion = WALK;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pFrameKey = L"Player_RIGHT";

		if (m_fXSpeed < m_fMaxSpeed)
			m_fXSpeed += 0.2f;

		if (!m_bAir)
			m_eCurMotion = WALK;
	}
	else
	{
		if (m_bAir)
		{
			m_eCurMotion = JUMP;

			if (abs(m_fXSpeed) > 0.001f) m_fXSpeed = 0.95f * m_fXSpeed;
			else  m_fXSpeed = 0.f;

			if ((m_fYSpeed < 0.f) || (m_fYSpeed <= -m_fMaxJumpSpeed)) m_eJumpState = DOWNWARD;
			else m_eJumpState = UPWARD;
		}
		else
		{
			m_eCurMotion = IDLE;

			if (abs(m_fXSpeed) > 0.001f)
			{
				CTile* pTile1 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[4] + TILEX);
				CTile* pTile2 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[5] + TILEX);

				if (pTile1 == nullptr || pTile2 == nullptr) return;

				if ((pTile1->Get_Option(CTile::TILE_BLOCK) == int(BLOCK::HONEY)) &&
					(pTile2->Get_Option(CTile::TILE_BLOCK) == int(BLOCK::HONEY)))
				{
					m_fXSpeed *= 0.5f;
				}
				else if ((pTile1->Get_Option(CTile::TILE_BLOCK) == int(BLOCK::ICE)) &&
						(pTile2->Get_Option(CTile::TILE_BLOCK) == int(BLOCK::ICE)))
				{
					m_fXSpeed *= 0.995f;
				}
				else m_fXSpeed = 0.9f * m_fXSpeed;
			}
			else  m_fXSpeed = 0.f;
		}
	}
}

void CPlayer::OptionKey_Input()
{
	for (int i = 0; i < 10; ++i)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('0' + i))
		{
			if (i != 0)
			{
				CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG)->Set_Data(i - 1);
			}
			else
			{
				CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG)->Set_Data(9);
			}
		}
	}

	if (CKeyMgr::Get_Instance()->Get_WheelUp() && g_bESC)
	{
		CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(1);
		cout << "Wheel Up" << endl;
		CKeyMgr::Get_Instance()->Set_WheelUp(false);
	}
	else if (CKeyMgr::Get_Instance()->Get_WheelDown() && g_bESC)
	{
		CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT)->Set_Data(-1);
		cout << "Wheel Down" << endl;
		CKeyMgr::Get_Instance()->Set_WheelDown(false);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F4))
	{
		m_tInfo.fX = 300.f;
		m_tInfo.fY = 300.f;
		m_fYSpeed = 0.f;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F6))
	{
		CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CMonster_N1>::Create_Obj(m_tInfo.fX + 100.f, m_tInfo.fY - 100.f));
	}
	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
	{
		g_bESC = !g_bESC;

		if (g_bESC)
		{
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Menu_Open.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Menu_Close.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
		}
	}
}

void CPlayer::MotionKey_Input()
{
	// JUMP
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
	{
		if (m_eJumpState == CANJUMP)
		{
			m_bAir = true;
			m_fYSpeed -= 10.f;
			m_eJumpState = UPWARD;
			m_eCurMotion = JUMP;
		}
	}

	// USE
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		if ((g_bESC) || (g_bInteraction))
			return;

		if (m_pHandItem)
		{
			m_bUsing = true;
			m_pHandItem->Set_Pos(m_tInfo.fX, m_tInfo.fY);
			Update_HandRect();
			m_pHandItem->Set_Angle(m_fAngle);
			m_pHandItem->Use_Item(this);
			m_eCurMotion = USE;

			if (cosf(m_fAngle) >= 0)
			{
				m_pFrameKey = L"Player_RIGHT";
			}
			else
			{
				m_pFrameKey = L"Player_LEFT";
			}
		}
	}
	else
	{
		m_bUsing = false;
		if(cosf(m_fAngle) >= 0)
			m_fHandAngle = -PI * 0.75f;
		else
			m_fHandAngle = -PI * 0.25f;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		CObj* tempNPC = CCollisionMgr::Picked_NPC();

		if (tempNPC)
		{
			if ((!g_bInteraction) && (CCollisionMgr::Check_NearNPC(this, tempNPC)))
			{
				cout << "Interaction NPC : " << tempNPC->Get_Info()->fX << ", " << tempNPC->Get_Info()->fY << endl;
				
				g_bInteraction = true;
				
				static_cast<CNPC*>(tempNPC)->Interaction();
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Chat.wav", SOUND_EFFECT6, g_fVolume, 1, 100);
			}
		}
		else if (m_bGravityPotion)
		{
			m_iGravityDir *= -1;
			m_fFallingDistance = 0.f;
		}
	}

	if ((CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)) && (g_bInteraction))
	{
		CUIMgr::Get_Instance()->Get_UI(UIS_MESSAGEBOX)->Set_Data(-1);
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Chat.wav", SOUND_EFFECT6, g_fVolume, 1, 100);

		if (g_bShop)
			g_bShop = false;
	}
}



void CPlayer::Offset()
{
	int	iOffSetMinX = (WINCX >> 1) - 1;
	int	iOffSetMinY = (WINCY >> 1) - 1;
	int	iOffSetMaxX = (WINCX >> 1) + 1;
	int	iOffSetMaxY = (WINCY >> 1) + 1;

	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (iOffSetMinX >= m_tInfo.fX + iScrollX)
	{
		CScrollMgr::Get_Instance()->Add_ScrollX(iOffSetMinX - m_tInfo.fX - iScrollX);
	}

	if (iOffSetMaxX < m_tInfo.fX + iScrollX)
	{
		CScrollMgr::Get_Instance()->Add_ScrollX(iOffSetMaxX - m_tInfo.fX - iScrollX);
	}

	if (iOffSetMinY >= m_tInfo.fY + iScrollY)
	{
		CScrollMgr::Get_Instance()->Add_ScrollY(iOffSetMinY - m_tInfo.fY - iScrollY);
	}

	if (iOffSetMaxY < m_tInfo.fY + iScrollY)
	{
		CScrollMgr::Get_Instance()->Add_ScrollY(iOffSetMaxY - m_tInfo.fY - iScrollY);
	}
}

void CPlayer::Motion_Change()
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
			m_tFrame.iEnd = 15;
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

		case USE:
			m_tFrame.iStart = 16;
			m_tFrame.iEnd = 22;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 50;
			break;

		case DEAD:
			m_tFrame.iStart = 99;
			m_tFrame.iEnd = 99;
			m_tFrame.iMotion = m_tFrame.iStart;
			m_tFrame.dwTime = GetTickCount();
			m_tFrame.dwSpeed = 100000;
			break;
		}

		m_ePreMotion = m_eCurMotion;
	}
}

void CPlayer::Move_Frame()
{
	// 플레이어 리소스 세로 1열
	// -> 상태마다 start/end값 다름, iMotion에 시작 start값 저장
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount())
	{
		++m_tFrame.iStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iStart > m_tFrame.iEnd)
			m_tFrame.iStart = m_tFrame.iMotion;
	}
}

void CPlayer::Status_Check()
{
	if (!m_bGravityPotion)
	{
		m_iGravityDir = 1;
	}
	else
	{
		if (m_fTime[TM_BUFF] + 10000 < GetTickCount())
		{
			m_bGravityPotion = false;
		}
	}

	Search_TargetDir();

	Search_ObjTile(m_tInfo.fX, (float)m_tRect.bottom);

	CTile* pTile1 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[4] + TILEX);
	CTile* pTile2 = CTileMgr::Get_Instance()->Get_Tile(m_vecTile[5] + TILEX);

	if (pTile1 == nullptr || pTile2 == nullptr) return;

	if ((pTile1->Get_Option(CTile::TILE_BLOCK) == 0) &&
		(pTile2->Get_Option(CTile::TILE_BLOCK) == 0))
	{
		m_bAir = true;
		m_eJumpState = DOWNWARD;
	}
}

void CPlayer::Search_TargetDir()
{
	if (m_pTargetMouse)
	{
		int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
		float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f);

		// 마우스를 다시 World 좌표로 변환
		fWidth = (m_pTargetMouse->Get_Info()->fX - iScrollX) - m_tInfo.fX;
		fHeight = (m_pTargetMouse->Get_Info()->fY - iScrollY) - m_tInfo.fY;

		m_fAngle = atan2f(-fHeight, fWidth);
	}
}

void CPlayer::Gravity()
{
	if (m_bAir == true)
	{
		m_fYSpeed += 24.f * m_fTime[TM_GRAVITY] * m_iGravityDir;

		if (m_fYSpeed >= 600.f)
			m_fYSpeed = 600.f;
	}
	else
	{
		m_fYSpeed = 0.f;
		m_fFallingDistance = 0.f;
	}
}

void CPlayer::Move()
{
	const int iMaxIter = 100;
	int iIter = 0;
	CTileMgr* pTileMgr = CTileMgr::Get_Instance();

	while (iIter++ < iMaxIter)
	{
		CTile* pTile1 = pTileMgr->Get_Tile(m_vecTile[4] + TILEX);
		CTile* pTile2 = pTileMgr->Get_Tile(m_vecTile[5] + TILEX);

		if (pTile1 == nullptr || pTile2 == nullptr) continue;

		if ((pTile1->Get_Option(CTile::TILE_BLOCK) == 6) &&
			(pTile2->Get_Option(CTile::TILE_BLOCK) == 6))
		{
			m_fXSpeed += -0.5f * m_fXSpeed;
		}

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

				if (m_fFallingDistance > 400)
				{
					pTile1 = pTileMgr->Get_Tile(m_vecTile[4] + TILEX);
					pTile2 = pTileMgr->Get_Tile(m_vecTile[5] + TILEX);

					if (pTile1 == nullptr || pTile2 == nullptr) continue;

					if ((pTile1->Get_Option(CTile::TILE_BLOCK) != 8) &&
						(pTile2->Get_Option(CTile::TILE_BLOCK) != 8))
						m_iCurHP -= int(10.f * (abs(m_fFallingDistance) - 400.f) / 16.f);

					if (m_iCurHP <= 0)
					{
						if (!CUIMgr::Get_Instance()->Get_TextList()->empty())
						{
							for (auto p : *CUIMgr::Get_Instance()->Get_TextList())
							{
								p->Add_PosY(-28.f);
							}
						}

						CUIMgr::Get_Instance()->Add_UI(UIS_TEXT, CAbstractFactory<CText>::Create_UI());
						CUIMgr::Get_Instance()->Get_TextList()->back()->Set_Data(CText::PLAYERDEAD_G);
						CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_PlayerKilled.wav", SOUND_EFFECT1, g_fVolume, 1, 100);
					}
					else
					{
						CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_PlayerHit.wav", SOUND_EFFECT1, g_fVolume, 1, 100);
					}
				}

				m_bAir = false;
				m_eJumpState = CANJUMP;
			}
			else m_fYSpeed *= 0.5f;
		}
	}

#ifdef _DEBUG
	if (iIter >= iMaxIter) cout << "Player Move Loop OverFlow!" << endl;
#endif

	m_tInfo.fX += m_fXSpeed;
	m_tInfo.fY += m_fYSpeed;
	m_fFallingDistance += m_fYSpeed;
}

void CPlayer::Update_HandRect()
{
	if (!m_pHandItem)
		return;

	m_tHandRect = {	long(m_tInfo.fX - (m_pHandItem->Get_Info()->fCX * 0.5f)),
					long(m_tInfo.fY - (m_pHandItem->Get_Info()->fCY * 0.5f)),
					long(m_tInfo.fX + (m_pHandItem->Get_Info()->fCX * 0.5f)),
					long(m_tInfo.fY + (m_pHandItem->Get_Info()->fCY * 0.5f)) };
}



void CPlayer::OnCollision(COLMSG _colMSG)
{
	if ((_colMSG.pObj->Get_ObjID() == MONSTER) && (m_fTime[TM_INVINCIBLE] + m_dwIvTime < (float)GetTickCount()))
	{
		m_iCurHP -= static_cast<CMonster*>(_colMSG.pObj)->Get_Damage();

		if (m_iCurHP > 0)
		{
			m_fTime[TM_INVINCIBLE] = (float)GetTickCount();

			if (_colMSG.eColType == COLTYPE::CD_LEFT)
				m_fXSpeed = 5.f;
			else if (_colMSG.eColType == COLTYPE::CD_RIGHT)
				m_fXSpeed = -5.f;

			m_bAir = true;
			m_fYSpeed = -3.f;
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_PlayerHit.wav", SOUND_EFFECT1, g_fVolume, 1, 100);
		}
		else
		{
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_PlayerKilled.wav", SOUND_EFFECT1, g_fVolume, 1, 100);
			if (!CUIMgr::Get_Instance()->Get_TextList()->empty())
			{
				for (auto p : *CUIMgr::Get_Instance()->Get_TextList())
				{
					p->Add_PosY(-28.f);
				}
			}
			CUIMgr::Get_Instance()->Add_UI(UIS_TEXT, CAbstractFactory<CText>::Create_UI());
			CUIMgr::Get_Instance()->Get_TextList()->back()->Set_Data(CText::PLAYERDEAD_N);
		}

		return;
	}

	if ((_colMSG.pObj->Get_ObjID() == MONSTER_BULLET) && (m_fTime[TM_INVINCIBLE] + m_dwIvTime < (float)GetTickCount()))
	{
		m_iCurHP -= static_cast<CBullet*>(_colMSG.pObj)->Get_Damage();

		if (m_iCurHP > 0)
		{
			m_fTime[TM_INVINCIBLE] = (float)GetTickCount();

			if (_colMSG.eColType == COLTYPE::CD_LEFT)
				m_fXSpeed = 5.f;
			else if (_colMSG.eColType == COLTYPE::CD_RIGHT)
				m_fXSpeed = -5.f;

			m_bAir = true;
			m_fYSpeed = -3.f;
		}
		else
		{
			if (!CUIMgr::Get_Instance()->Get_TextList()->empty())
			{
				for (auto p : *CUIMgr::Get_Instance()->Get_TextList())
				{
					p->Add_PosY(-28.f);
				}
			}
			CUIMgr::Get_Instance()->Add_UI(UIS_TEXT, CAbstractFactory<CText>::Create_UI());
			CUIMgr::Get_Instance()->Get_TextList()->back()->Set_Data(CText::PLAYERDEAD_B);
		}

		return;
	}
}
