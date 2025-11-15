#pragma once
#include "CUI.h"

class CHPBar : public CUI
{
public:
	CHPBar();
	~CHPBar();

public:
	void		Initialize()		override;
	int			Update()			override;
	void		Late_Update()		override;
	void		Render(HDC hDC)		override;
	void		Release()			override;

public:
	void		Set_Data(int _iHP)	override;
	void		Set_MaxHP(int _iHP) { m_iPlayerMaxHP = _iHP; }

private:
	int			m_iPlayerMaxHP;
	int			m_iPlayerCurHP;
	INFO		m_tLeftInfo;
	INFO		m_tMidInfo;
	INFO		m_tRightInfo;
	INFO		m_tFillInfo;
};