#pragma once

#include "Define.h"
#include "CMouse.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	Late_Update()	PURE;
	virtual void	Render(HDC hDC)	PURE;
	virtual void	Release()		PURE;

	void			Set_Mouse(CMouse* pMouse) { m_pMouse = pMouse; }

protected:
	CMouse* m_pMouse;
};

