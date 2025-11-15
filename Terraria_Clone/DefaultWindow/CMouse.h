#pragma once
#include "CItem.h"

class CMouse
{
public:
	CMouse();
	virtual ~CMouse();

public:
	void		Initialize();
	int			Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

	void		Set_MouseItem(CItem* pItem) { m_pMouseItem = pItem; }

	const POINT*	Get_Point() { return &m_ptMouse; }
	const INFO*		Get_Info() { return &m_tInfo; }
	const RECT*		Get_Rect() { return &m_tRect; }
	CItem*&			Get_MouseItem() { return m_pMouseItem; }

	void		Update_Rect();

private:
	POINT		m_ptMouse;
	INFO		m_tInfo;
	RECT		m_tRect;
	CItem*		m_pMouseItem;
	HDC			m_hMemDC;
};