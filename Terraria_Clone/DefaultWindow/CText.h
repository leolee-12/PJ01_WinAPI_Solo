#pragma once
#include "CUI.h"
class CText : public CUI
{
public:
	enum TEXTID { PLAYERDEAD_N, PLAYERDEAD_G, PLAYERDEAD_B, BOSSSPAWN, BOSSKILL, TI_END };

public:
	CText();
	virtual ~CText();

	void Initialize()			override;
	int Update()				override;
	void Late_Update()			override;
	void Render(HDC hDC)		override;
	void Release()				override;
	void Set_Data(int _iData)	override;

private:
	DWORD			m_dwTextTime;
	const TCHAR*	m_pTextKey;
	int				m_iStart;
};

