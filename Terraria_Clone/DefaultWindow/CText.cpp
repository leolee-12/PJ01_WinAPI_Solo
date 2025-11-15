#include "pch.h"
#include "CText.h"
#include "CBmpMgr.h"

CText::CText()
	:	m_dwTextTime(0),
		m_iStart(0)
{
}

CText::~CText()
{
}

void CText::Initialize()
{
	m_tInfo.fX = 50.f;		// Text 좌상단 좌표로 쓸 것 (텍스트 리소스 크기가 달라서)
	m_tInfo.fY = float(WINCY - 50);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_PlayerDead_N.bmp", L"Text_PlayerDead_N");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_PlayerDead_G.bmp", L"Text_PlayerDead_G");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_PlayerDead_B.bmp", L"Text_PlayerDead_B");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_KingSlime.bmp", L"Text_KingSlime");

	m_dwTextTime = GetTickCount();
}

int CText::Update()
{
	if (m_dwTextTime + 5000 < GetTickCount())
	{
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CText::Late_Update()
{
}

void CText::Render(HDC hDC)
{
	HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pTextKey);

	GdiTransparentBlt(hDC,
		m_tInfo.fX,
		m_tInfo.fY,
		(int)m_tInfo.fCX,
		28,					// 텍스트 리소스 한 줄 당 28픽셀로 맞추기
		hMemDC,
		0,
		28 * m_iStart,
		(int)m_tInfo.fCX,
		28,
		RGB(0, 0, 0));
}

void CText::Release()
{
}

void CText::Set_Data(int _iData)
{
	srand(GetTickCount());

	switch (_iData)
	{
	case PLAYERDEAD_N:
	{
		m_tInfo.fCX = 374.f;
		m_tInfo.fCY = 140.f;
		m_pTextKey = L"Text_PlayerDead_N";
		m_iStart = rand() % 5;
	}
	break;

	case PLAYERDEAD_G:
	{
		m_tInfo.fCX = 409.f;
		m_tInfo.fCY = 84.f;
		m_pTextKey = L"Text_PlayerDead_G";
		m_iStart = rand() % 3;
	}
	break;

	case PLAYERDEAD_B:
	{
		m_tInfo.fCX = 515.f;
		m_tInfo.fCY = 140.f;
		m_pTextKey = L"Text_PlayerDead_B";
		m_iStart = rand() % 5;
		
	}
	break;

	case BOSSSPAWN:
	{
		m_tInfo.fCX = 270.f;
		m_tInfo.fCY = 56.f;
		m_pTextKey = L"Text_KingSlime";
		m_iStart = 0;
	}
	break;

	case BOSSKILL:
	{
		m_tInfo.fCX = 270.f;
		m_tInfo.fCY = 56.f;
		m_pTextKey = L"Text_KingSlime";
		m_iStart = 1;
	}
	break;
	}
}
