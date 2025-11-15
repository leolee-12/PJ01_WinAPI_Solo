#pragma once
#include "CUI.h"

class CButton : public CUI
{
public:
	enum BUTTONID { BUTTON_START, BUTTON_EDIT, BUTTON_EXIT };

public:
	CButton();
	virtual ~CButton();

public:
	void	Initialize()		override;
	int		Update()			override;
	void	Late_Update()		override;
	void	Render(HDC hDC)		override;
	void	Release()			override;
	void	Set_Data(int iNum)	override;

private:
	int				m_iDrawID;
};

