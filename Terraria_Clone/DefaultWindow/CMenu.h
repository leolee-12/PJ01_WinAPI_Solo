#pragma once
#include "CScene.h"
class CMenu : public CScene
{
public:
	CMenu();
	virtual ~CMenu();

public:
	void	Initialize()	override;
	void	Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

private:
	HDC		m_hBGDC[5];
	HDC		m_hTitleDC;
	float	m_fBGScrollX[5][2];
	int		m_iBGScrollY[5];
};

