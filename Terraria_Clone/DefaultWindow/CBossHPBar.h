#pragma once
#include "CUI.h"
class CBossHPBar : public CUI
{
public:
	CBossHPBar();
	virtual ~CBossHPBar();

	void	Initialize()			override;
	int		Update()				override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Set_Data(int _iHP)		override { m_iBossCurHP = _iHP; }

private:
	int			m_iBossMaxHP;
	int			m_iBossCurHP;
	int			m_iGap;
};

