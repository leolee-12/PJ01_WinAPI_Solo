#pragma once
#include "Define.h"

class CEffect;

class CEffectMgr
{
private:
	CEffectMgr();
	CEffectMgr(const CEffectMgr& rhs) = delete;		// 삭제
	CEffectMgr& operator=(CEffectMgr& rObj) = delete;	// 삭제
	~CEffectMgr();

public:
	static CEffectMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CEffectMgr;

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	static vector<HDC> m_vecEffectDC[EF_END];
	static map<EFFECTKEY, INFO> m_mapEffectDataBase;

public:
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

	void	Add_Effect(EFFECTID eID, CEffect* pEffect);
	void	Delete_ID(EFFECTID eID);
	void	Init_HDCs();
	void	Init_DataBase();
	void	Generate_Effect(CObj* pObj, EFFECTID eID);
	void	Set_EffectInfo(CEffect* pEffect);

	list<CEffect*>*	Get_EffectList(EFFECTID eID) { return &m_EffectList[eID]; }

private:
	static CEffectMgr*	m_pInstance;
	list<CEffect*>		m_EffectList[EF_END];
};

