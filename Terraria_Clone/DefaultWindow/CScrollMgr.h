#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	void		Add_ScrollX(float _fX) { m_fScrollX += _fX; }
	void		Add_ScrollY(float _fY) { m_fScrollY += _fY; }

	float		Get_ScrollX() { return m_fScrollX; }
	float		Get_ScrollY() { return m_fScrollY; }

	void		Scroll_Lock();

private:
	float		m_fScrollX;
	float		m_fScrollY;


public:
	static CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CScrollMgr* m_pInstance;
};

