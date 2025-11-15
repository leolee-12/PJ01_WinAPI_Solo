#pragma once
#include "CUI.h"

class CPlayerDeadText : public CUI
{
public:
	CPlayerDeadText();
	virtual ~CPlayerDeadText();

	void	Initialize()			override;
	int		Update()				override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Set_Data(int _iData)	override;

private:
	DWORD			m_dwTextTime;
	string			m_sCount;
	bool			m_bActive;
};