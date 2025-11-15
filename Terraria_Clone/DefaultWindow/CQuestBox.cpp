#include "pch.h"
#include "CQuestBox.h"
#include "CUIMgr.h"
#include "CObjMgr.h"
#include "CTextMgr.h"
#include "CNPC_Merchant.h"
#include "CAbstractFactory.h"
#include "CNPC_Nurse.h"

CQuestBox::QUESTID CQuestBox::m_eCurQuest = Q_START;


CQuestBox::CQuestBox()
	:	m_iCount(0),
		m_dwTime(GetTickCount())
{
	ZeroMemory(&m_tTextRect, sizeof(RECT));
	ZeroMemory(m_bQuestCleared, sizeof(m_bQuestCleared));
	ZeroMemory(m_iQuestEndCount, sizeof(m_iQuestEndCount));
	m_vecQuestText.reserve(Q_END);
}

CQuestBox::~CQuestBox()
{
}

void CQuestBox::Initialize()
{
	m_tInfo = { float(WINCX - 150), 150.f, 250.f, 125.f };
	__super::Update_Rect();
	m_tTextRect = { m_tRect.left + 6, m_tRect.top + 6, m_tRect.right - 6, m_tRect.bottom };

	m_vecQuestText.push_back(L"\n\n상황 파악하기\n % d / % d");
	m_iQuestEndCount[Q_START] = 1;

	m_vecQuestText.push_back(L"\n\n힌트박스 확인하기\n % d / % d");
	m_iQuestEndCount[Q_TUTORIAL] = 4;

	m_vecQuestText.push_back(L"\n\n지상으로 내려가기\n % d / % d");
	m_iQuestEndCount[Q_LAND] = 1;

	m_vecQuestText.push_back(L"\n\n집 지어보기\n % d / % d");
	m_iQuestEndCount[Q_HOUSING] = 5;

	m_vecQuestText.push_back(L"\n\n새로운 NPC와 대화하기\n % d / % d");
	m_iQuestEndCount[Q_MERCHANT] = 2;

	m_vecQuestText.push_back(L"\n\n괴물 처치하기\n % d / % d");
	m_iQuestEndCount[Q_BOSS] = 1;

	m_vecQuestText.push_back(L"\n\n이 세계에서 탈출하기\n % d / % d");
	m_iQuestEndCount[Q_EXIT] = 1;
}

int CQuestBox::Update()
{
	if ((!m_bQuestCleared[m_eCurQuest]) && (m_iCount == m_iQuestEndCount[m_eCurQuest]))
	{
		m_bQuestCleared[m_eCurQuest] = true;
		m_dwTime = GetTickCount();
	}

	if (m_bQuestCleared[m_eCurQuest] && !g_bInteraction)
	{
		if (m_dwTime + 1000 < GetTickCount())
		{
			if (m_eCurQuest == Q_HOUSING)
			{
				CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Merchant>::Create_Obj(400.f, 1200.f));
				CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Nurse>::Create_Obj(600.f, 1200.f));
			}

			Set_Data(1);
		}
	}

	if ((m_eCurQuest == Q_LAND) && (CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front()->Get_Info()->fY > 1200.f))
		Set_Data(-1);

	

	return OBJ_NOEVENT;
}

void CQuestBox::Late_Update()
{
}

void CQuestBox::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CUIMgr::m_hQuestBoxDC,
		0,
		0,
		(int)m_tInfo.fCX,   // 복사할 이미지 가로
		(int)m_tInfo.fCY,   // 복사할 이미지 세로
		RGB(255, 0, 255));    // 제거할 픽셀 색상 값

	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);
	SetTextColor(hDC, RGB(255, 255, 0));	// 폰트 색깔
	SetTextAlign(hDC, TA_LEFT);			// 폰트 정렬 기준

	wchar_t wStr[64];
	swprintf(wStr, 64, L"퀘스트");
	TextOut(hDC, m_tTextRect.left + 6, m_tTextRect.top, wStr, lstrlen(wStr));

	if (m_eCurQuest < Q_END)
	{
		SetTextColor(hDC, RGB(255, 255, 255));	// 폰트 색깔
		swprintf(wStr, 64, m_vecQuestText[m_eCurQuest], m_iCount, m_iQuestEndCount[m_eCurQuest]);
		DrawText(hDC, wStr, -1, &m_tTextRect, DT_LEFT | DT_WORDBREAK);

		if (m_bQuestCleared[m_eCurQuest])
		{
			SetTextColor(hDC, RGB(0, 255, 0));	// 폰트 색깔
			swprintf(wStr, 64, L"완료 !");
			TextOut(hDC, m_tTextRect.right - 50, m_tTextRect.bottom - 50, wStr, lstrlen(wStr));
		}
	}
	else
	{
		SetTextColor(hDC, RGB(0, 255, 0));	// 폰트 색깔
		swprintf(wStr, 64, L"모든 퀘스트 완료 !");
		DrawText(hDC, wStr, -1, &m_tTextRect, DT_LEFT | DT_WORDBREAK);
	}


	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CQuestBox::Release()
{
}

void CQuestBox::Set_Data(int _iData)
{
	if (_iData == -1)
	{
		if (m_iCount != m_iQuestEndCount[m_eCurQuest])
			m_iCount++;
	}
	else if (_iData == 1)
	{
		m_eCurQuest = (QUESTID)(m_eCurQuest + 1);
		m_iCount = 0;
	}
}
