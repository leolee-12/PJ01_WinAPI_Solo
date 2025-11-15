#pragma once
#include "Define.h"
#include "CUI.h"

class CUIMgr
{
public:
	enum NUMBERKEYID { SLOTNUM, COUNTNUM, DAMAGENUM, DEADNUM, NI_END };

private:
	CUIMgr();
	CUIMgr(const CUIMgr& rhs) = delete;		// 삭제
	CUIMgr& operator=(CUIMgr& rObj) = delete;	// 삭제
	~CUIMgr();

private:
	static CUIMgr* m_pInstance;

public:
	static CUIMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUIMgr;
		}

		return m_pInstance;
	}

	static void			Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Add_UI(UIID eID, CUI* pUI);
	CUI*		Get_UI(UIID eID) { return m_vecUI[eID]; }
	list<CUI*>*	Get_TextList() { return &m_TextList; }
	void		Delete_UI(UIID eID);

private:
	vector<CUI*>	m_vecUI;
	list<CUI*>		m_TextList;
	
public:
	static HDC		m_hNumDC[NI_END];
	static HDC		m_hCraftIconDC;
	static HDC		m_hPlayerDeadTextDC;
	static HDC		m_hMessageBoxDC;
	static HDC		m_hQuestBoxDC;
	static HDC		m_hErrorMessageDC;
};