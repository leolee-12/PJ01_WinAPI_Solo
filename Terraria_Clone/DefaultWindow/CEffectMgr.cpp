#include "pch.h"
#include "CEffectMgr.h"
#include "CBmpMgr.h"
#include "CEffect.h"
#include "CBullet_B.h"
#include "CAbstractFactory.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;
vector<HDC>	CEffectMgr::m_vecEffectDC[EF_END];
map<EFFECTKEY, INFO> CEffectMgr::m_mapEffectDataBase;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
	Release();
}

void CEffectMgr::Initialize()
{
	Init_HDCs();
	Init_DataBase();
}

void CEffectMgr::Update()
{
	for (UINT i = 0; i < EF_END; ++i)
	{
		for (auto iter = m_EffectList[i].begin();
			iter != m_EffectList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (iResult == OBJ_DEAD)
			{
				Safe_Delete<CEffect*>(*iter);
				iter = m_EffectList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CEffectMgr::Late_Update()
{
	for (UINT i = 0; i < EF_END; ++i)
	{
		for (auto& pEffect : m_EffectList[i])
		{
			pEffect->Late_Update();

			if (m_EffectList[i].empty())
				break;
		}
	}
}

void CEffectMgr::Render(HDC hDC)
{
	for (UINT i = 0; i < EF_END; ++i)
	{
		for (auto& pEffect : m_EffectList[i])
		{
			pEffect->Render(hDC);
		}
	}
}

void CEffectMgr::Release()
{
	for (UINT i = 0; i < EF_END; ++i)
	{
		for_each(m_EffectList[i].begin(), m_EffectList[i].end(), Safe_Delete<CEffect*>);
		m_EffectList[i].clear();
	}
}

void CEffectMgr::Add_Effect(EFFECTID eID, CEffect* pEffect)
{
	if (eID >= EF_END || nullptr == pEffect)
		return;

	m_EffectList[eID].push_back(pEffect);
}

void CEffectMgr::Delete_ID(EFFECTID eID)
{
	for (auto& pEffect : m_EffectList[eID])
		Safe_Delete(pEffect);

	m_EffectList[eID].clear();
}

void CEffectMgr::Init_HDCs()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_PlayerDead0.bmp", L"Effect_PlayerDead0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_PlayerDead1.bmp", L"Effect_PlayerDead1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_PlayerDead2.bmp", L"Effect_PlayerDead2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_ZombieDead0.bmp", L"Effect_ZombieDead0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_ZombieDead1.bmp", L"Effect_ZombieDead1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_ZombieDead2.bmp", L"Effect_ZombieDead2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_KingSlimeDead0.bmp", L"Effect_KingSlimeDead0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_KingSlimeDead1.bmp", L"Effect_KingSlimeDead1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_KingSlimeDead2.bmp", L"Effect_KingSlimeDead2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_GuideDead0.bmp", L"Effect_GuideDead0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_GuideDead1.bmp", L"Effect_GuideDead1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_GuideDead2.bmp", L"Effect_GuideDead2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite00.bmp", L"Effect_Dynamite00");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite01.bmp", L"Effect_Dynamite01");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite02.bmp", L"Effect_Dynamite02");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite03.bmp", L"Effect_Dynamite03");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite04.bmp", L"Effect_Dynamite04");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/Effect_Dynamite05.bmp", L"Effect_Dynamite05");

	m_vecEffectDC[EF_PLAYERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_PlayerDead0"));
	m_vecEffectDC[EF_PLAYERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_PlayerDead1"));
	m_vecEffectDC[EF_PLAYERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_PlayerDead2"));
	m_vecEffectDC[EF_MONSTERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_ZombieDead0"));
	m_vecEffectDC[EF_MONSTERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_ZombieDead1"));
	m_vecEffectDC[EF_MONSTERDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_ZombieDead2"));
	m_vecEffectDC[EF_BOSSDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_KingSlimeDead0"));
	m_vecEffectDC[EF_BOSSDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_KingSlimeDead1"));
	m_vecEffectDC[EF_BOSSDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_KingSlimeDead2"));
	m_vecEffectDC[EF_GUIDEDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_GuideDead0"));
	m_vecEffectDC[EF_GUIDEDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_GuideDead1"));
	m_vecEffectDC[EF_GUIDEDEAD].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_GuideDead2"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite00"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite01"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite02"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite03"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite04"));
	m_vecEffectDC[EF_DYNAMITE].push_back(CBmpMgr::Get_Instance()->Find_Image(L"Effect_Dynamite05"));
}

void CEffectMgr::Init_DataBase()
{
	for (size_t i = 0; i < EF_END; ++i)
	{
		size_t len = m_vecEffectDC[i].size();

		for (size_t j = 0; j < len; ++j)
		{
			BITMAP		bmpInfo = {};
			HBITMAP		hBitmap = (HBITMAP)GetCurrentObject(CEffectMgr::m_vecEffectDC[i][j], OBJ_BITMAP);
			GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

			m_mapEffectDataBase.emplace(EFFECTKEY((EFFECTID)i, j), INFO(0.f, 0.f, float(bmpInfo.bmWidth), float(bmpInfo.bmHeight)));
		}
	}
}


void CEffectMgr::Generate_Effect(CObj* pObj, EFFECTID eID)
{
	srand(GetTickCount());
	float fAngleTemp[6];
	float fXTemp[6];

	for (int i = 0; i < 6; i++)
	{
		fAngleTemp[i] = ((rand() % 360)) * PI / 180.f;
		fXTemp[i] = (rand() % 10) - 5.f;
	}

	if (eID != EF_DYNAMITE)
	{
		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX, pObj->Get_Info()->fY, fAngleTemp[0], eID, 0));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[0]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX, pObj->Get_Info()->fY, fAngleTemp[1], eID, 1));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[1]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX, pObj->Get_Info()->fY, fAngleTemp[2], eID, 1));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[2]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX, pObj->Get_Info()->fY, fAngleTemp[3], eID, 2));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[3]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX, pObj->Get_Info()->fY, fAngleTemp[4], eID, 2));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[4]);
	}
	else
	{
		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX - 16.f, pObj->Get_Info()->fY, fAngleTemp[0], eID, 0));
		m_EffectList[eID].back()->Add_YSpeed(9.5f);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 8.f, pObj->Get_Info()->fY - 16.f, fAngleTemp[1], eID, 1));
		m_EffectList[eID].back()->Add_YSpeed(9.5f);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX - 8.f, pObj->Get_Info()->fY + 16.f, fAngleTemp[2], eID, 2));
		m_EffectList[eID].back()->Add_YSpeed(9.5f);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[5], pObj->Get_Info()->fY + 16.f * fXTemp[1], fAngleTemp[3], eID, 3));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[0]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[4], pObj->Get_Info()->fY + 16.f * fXTemp[3], fAngleTemp[4], eID, 4));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[1]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[3], pObj->Get_Info()->fY + 16.f * fXTemp[5], fAngleTemp[5], eID, 5));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[2]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[2], pObj->Get_Info()->fY + 16.f * fXTemp[0], fAngleTemp[3], eID, 3));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[3]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[1], pObj->Get_Info()->fY + 16.f * fXTemp[2], fAngleTemp[4], eID, 4));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[4]);

		Add_Effect(eID, CAbstractFactory<CEffect>::Create_Effect(pObj->Get_Info()->fX + 16.f * fXTemp[0], pObj->Get_Info()->fY + 16.f * fXTemp[4], fAngleTemp[5], eID, 5));
		m_EffectList[eID].back()->Add_XSpeed(fXTemp[5]);
	}
}

void CEffectMgr::Set_EffectInfo(CEffect* pEffect)
{
	pEffect->Set_Info(m_mapEffectDataBase[EFFECTKEY(pEffect->Get_EffectID(), pEffect->Get_EffectCode())]);
}