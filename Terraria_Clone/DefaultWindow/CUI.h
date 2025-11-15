#pragma once
#include "Define.h"
#include "CObj.h"

class CUI : public CObj
{
public:
	CUI();
	virtual ~CUI();

public:
	virtual void	Initialize()			PURE;
	virtual int		Update()				PURE;
	virtual void	Late_Update()			PURE;
	virtual void	Render(HDC hDC)			PURE;
	virtual void	Release()				PURE;
	virtual void	Set_Data(int _iData)	PURE;
	
protected:
	UIID			m_eUIID;
};