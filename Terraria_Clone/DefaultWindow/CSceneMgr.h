#pragma once
#include "CLogo.h"
#include "CMenu.h"
#include "CStage.h"
#include "CEdit.h"

class CSceneMgr
{
public:
	enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_END };

private:
	CSceneMgr();
	CSceneMgr(const CSceneMgr& rhs) = delete;		// 삭제
	CSceneMgr& operator=(CSceneMgr& rObj) = delete;	// 삭제
	~CSceneMgr();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneMgr;
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
	void		Scene_Change(SCENEID eScene);
	void		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();
	void		Init_Mouse(CMouse* pMouse) { m_pMouse = pMouse; }
	SCENEID		Get_SceneID() { return m_eCurrentID; }
	CMouse*		Get_Mouse() { return m_pMouse; }

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;
	CMouse*				m_pMouse;
	SCENEID				m_eCurrentID;
	SCENEID				m_ePreID;
};

