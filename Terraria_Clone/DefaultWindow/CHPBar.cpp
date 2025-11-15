#include "pch.h"
#include "CHPBar.h"
#include "CCharacter.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CTextMgr.h"

CHPBar::CHPBar()
	:	m_iPlayerMaxHP(0),
		m_iPlayerCurHP(0)
{
	ZeroMemory(&m_tLeftInfo, sizeof(INFO));
	ZeroMemory(&m_tMidInfo, sizeof(INFO));
	ZeroMemory(&m_tRightInfo, sizeof(INFO));
	ZeroMemory(&m_tFillInfo, sizeof(INFO));
}

CHPBar::~CHPBar()
{
}

void CHPBar::Initialize()
{
	m_eUIID = UIS_HPBAR;
	m_tInfo = { 780.f, 50.f, 0.f, 0.f };
	m_tLeftInfo = { 0.f, 0.f, 28.f, 30.f };
	m_tMidInfo = { 0.f, 0.f, 24.f, 30.f };
	m_tRightInfo = { 0.f, 0.f, 36.f, 38.f };
	m_tFillInfo = { 0.f, 0.f, 22.f, 22.f };

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Left.bmp", L"Heart_Left");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Middle.bmp", L"Heart_Middle");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Right.bmp", L"Heart_Right");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Heart_Fill.bmp", L"Heart_Fill");

	if (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty())
	{
		m_iPlayerMaxHP = static_cast<CCharacter*>(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front())->Get_MaxHP();
		m_iPlayerCurHP = static_cast<CCharacter*>(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front())->Get_HP();
	}
}

int CHPBar::Update()
{

	return 0;
}

void CHPBar::Late_Update()
{
}

void CHPBar::Render(HDC hDC)
{
	int HeartNum = m_iPlayerMaxHP / 20;
	int FillNum = m_iPlayerCurHP / 20;
	int FillRemainder = m_iPlayerCurHP % 20;
	float xTemp = m_tInfo.fX;
	float yTemp = m_tInfo.fY;
	HDC hMemDC = nullptr;
	
	for (int i = 0; i < HeartNum; ++i)
	{
		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heart_Left");

		if (i == 0)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heart_Left");

			GdiTransparentBlt(hDC,
				(int)(xTemp - (m_tLeftInfo.fCX * 0.5f)),
				(int)(yTemp - (m_tLeftInfo.fCY * 0.5f)),
				(int)m_tLeftInfo.fCX,
				(int)m_tLeftInfo.fCY,
				hMemDC,
				0,
				0,
				(int)m_tLeftInfo.fCX,		// 복사할 이미지 가로
				(int)m_tLeftInfo.fCY,		// 복사할 이미지 세로
				RGB(255, 0, 255));			// 제거할 픽셀 색상 값

			xTemp += (int)m_tLeftInfo.fCX - 2.f;
		}
		else if(i == HeartNum - 1)
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heart_Right");
			
			GdiTransparentBlt(hDC,
				(int)(xTemp - (m_tRightInfo.fCX * 0.5f) - 2.f),
				(int)(yTemp - (m_tRightInfo.fCY * 0.5f)),
				(int)m_tRightInfo.fCX,
				(int)m_tRightInfo.fCY,
				hMemDC,
				0,
				0,
				(int)m_tRightInfo.fCX,		// 복사할 이미지 가로
				(int)m_tRightInfo.fCY,		// 복사할 이미지 세로
				RGB(255, 0, 255));			// 제거할 픽셀 색상 값
		}
		else
		{
			hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heart_Middle");

			GdiTransparentBlt(hDC,
				(int)(xTemp - (m_tMidInfo.fCX * 0.5f)),
				(int)(yTemp - (m_tMidInfo.fCY * 0.5f)),
				(int)m_tMidInfo.fCX,
				(int)m_tMidInfo.fCY,
				hMemDC,
				0,
				0,
				(int)m_tMidInfo.fCX,		// 복사할 이미지 가로
				(int)m_tMidInfo.fCY,		// 복사할 이미지 세로
				RGB(255, 0, 255));			// 제거할 픽셀 색상 값

			xTemp += (int)m_tMidInfo.fCX;
		}
	}

	xTemp = m_tInfo.fX + 1.f;
	yTemp = m_tInfo.fY;
	hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Heart_Fill");
	
	for (int i = 0; i < FillNum; ++i)
	{
		GdiTransparentBlt(hDC,
			(int)(xTemp - (m_tFillInfo.fCX * 0.5f)),
			(int)(yTemp - (m_tFillInfo.fCY * 0.5f)),
			(int)m_tFillInfo.fCX,
			(int)m_tFillInfo.fCY,
			hMemDC,
			0,
			0,
			(int)m_tFillInfo.fCX,		// 복사할 이미지 가로
			(int)m_tFillInfo.fCY,		// 복사할 이미지 세로
			RGB(255, 0, 255));			// 제거할 픽셀 색상 값

		xTemp += (int)(m_tFillInfo.fCX + 2.f);
	}

	GdiTransparentBlt(hDC,
		(int)(xTemp - (m_tFillInfo.fCX * (FillRemainder * 0.05f) * 0.5f)),
		(int)(yTemp - (m_tFillInfo.fCY * (FillRemainder * 0.05f) * 0.5f)),
		(int)(m_tFillInfo.fCX * (FillRemainder * 0.05f)),
		(int)(m_tFillInfo.fCY * (FillRemainder * 0.05f)),
		hMemDC,
		0,
		0,
		(int)m_tFillInfo.fCX,		// 복사할 이미지 가로
		(int)m_tFillInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값


	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);
	const wchar_t* label = L"체력 : ";
	wchar_t wStr[32];
	swprintf(wStr, 32, L"%d / %d", m_iPlayerCurHP, m_iPlayerMaxHP);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetTextAlign(hDC, TA_CENTER);
	TextOut(hDC, int(m_tInfo.fX + 15.f), int(m_tInfo.fY - 42.f), label, lstrlen(label));
	TextOut(hDC, int(m_tInfo.fX + 95.f), int(m_tInfo.fY - 42.f), wStr, lstrlen(wStr));
	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CHPBar::Release()
{
}

void CHPBar::Set_Data(int _iHP)
{
	if(_iHP >= 0)
	{
		m_iPlayerCurHP = _iHP;
	}
	else if(_iHP == -1)
	{
		m_iPlayerMaxHP = static_cast<CCharacter*>(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front())->Get_MaxHP();
	}
}
