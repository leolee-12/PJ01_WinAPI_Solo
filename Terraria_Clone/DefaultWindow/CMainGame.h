#pragma once
#include "Define.h"
#include "CMouse.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render();
	void		Release();

	void		Key_Input();

private:
	HDC			m_hDC;
	TCHAR		m_szFPS[32];
	DWORD		m_dwTime;
	CMouse*		m_pMouse;
	int			m_iFPS;
};