#include "pch.h"
#include "CUI.h"
#include "CUIMgr.h"
#include "CBmpMgr.h"


CUIMgr* CUIMgr::m_pInstance = nullptr;
HDC		CUIMgr::m_hNumDC[NI_END];
HDC		CUIMgr::m_hCraftIconDC = nullptr;
HDC		CUIMgr::m_hPlayerDeadTextDC = nullptr;
HDC		CUIMgr::m_hMessageBoxDC = nullptr;
HDC		CUIMgr::m_hQuestBoxDC = nullptr;
HDC		CUIMgr::m_hErrorMessageDC = nullptr;
bool			g_bESC = false;

CUIMgr::CUIMgr()
{
	m_vecUI.reserve(UI_END);
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Initialize()
{
	if (m_vecUI.empty())
	{
		for (size_t i = 0; i < UI_END; ++i)
			m_vecUI.push_back(nullptr);
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_SlotNumber16.bmp", L"Text_SlotNumber16");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_CountNumber16.bmp", L"Text_CountNumber16");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_DamageNumber.bmp", L"Text_DamageNumber");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Icon_Craft.bmp", L"Icon_Craft");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Text_DeadNumber87.bmp", L"Text_DeadNumber87");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PlayerDeadText.bmp", L"UI_PlayerDeadText");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MessageBox400.bmp", L"UI_MessageBox");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_QuestBox.bmp", L"UI_QuestBox");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_ErrorMessage.bmp", L"UI_ErrorMessage");

	m_hNumDC[SLOTNUM] = CBmpMgr::Get_Instance()->Find_Image(L"Text_SlotNumber16");
	m_hNumDC[COUNTNUM] = CBmpMgr::Get_Instance()->Find_Image(L"Text_CountNumber16");
	m_hNumDC[DAMAGENUM] = CBmpMgr::Get_Instance()->Find_Image(L"Text_DamageNumber");
	m_hNumDC[DEADNUM] = CBmpMgr::Get_Instance()->Find_Image(L"Text_DeadNumber87");
	m_hCraftIconDC = CBmpMgr::Get_Instance()->Find_Image(L"Icon_Craft");
	m_hPlayerDeadTextDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_PlayerDeadText");
	m_hMessageBoxDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_MessageBox");
	m_hQuestBoxDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_QuestBox");
	m_hErrorMessageDC = CBmpMgr::Get_Instance()->Find_Image(L"UI_ErrorMessage");
}

void CUIMgr::Update()
{
	for (auto iter = m_vecUI.begin(); iter != m_vecUI.end(); iter++)
	{
		if(*iter == nullptr)
		{
			continue;
		}

		int iResult = (*iter)->Update();

		if (iResult == OBJ_DEAD)
		{
			Safe_Delete<CUI*>(*iter);
		}
	}

	for (auto iter = m_TextList.begin();
		iter != m_TextList.end(); )
	{
		if (*iter == nullptr)
		{
			++iter;
			continue;
		}

		int iResult = (*iter)->Update();

		if (iResult == OBJ_DEAD)
		{
			Safe_Delete<CUI*>(*iter);
			iter = m_TextList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CUIMgr::Late_Update()
{
	for (auto& pUI : m_vecUI)
	{
		if (pUI == nullptr)
			continue;

		pUI->Late_Update();

		if (m_vecUI.empty())
			break;
	}
}

void CUIMgr::Render(HDC hDC)
{
	for (auto& pUI : m_vecUI)
	{
		if (pUI == nullptr)
			continue;

		pUI->Render(hDC);
	}

	for (auto& pUI : m_TextList)
	{
		if (pUI == nullptr)
			continue;

		pUI->Render(hDC);
	}
}

void CUIMgr::Release()
{
	for_each(m_vecUI.begin(), m_vecUI.end(), Safe_Delete<CUI*>);
	m_vecUI.clear();

	for_each(m_TextList.begin(), m_TextList.end(), Safe_Delete<CUI*>);
	m_TextList.clear();
}

void CUIMgr::Add_UI(UIID eID, CUI* pUI)
{
	if ((0 > eID) || (eID >= UI_END) || pUI == nullptr)
		return;

	if (eID == UIS_TEXT)
		m_TextList.push_back(pUI);
	else
		m_vecUI[eID] = pUI;
}

void CUIMgr::Delete_UI(UIID eID)
{
	if ((0 > eID) || (eID >= UI_END))
		return;

	Safe_Delete<CUI*>(m_vecUI[eID]);
}
