#pragma once
#include "CScene.h"

class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	void	Initialize()	override;
	void	Update()		override;
	void	Late_Update()	override;
	void	Render(HDC hDC)	override;
	void	Release()		override;

	void	Render_Background(HDC hDC, int iScrollX, int iScrollY);
	void	Trigger_Check();

private:
	bool	m_bBossSpawn;
	bool	m_bBossKilled;
	bool	m_bCleared;
	bool	m_bEnding;
	HDC		m_hBGDC[4];
};

