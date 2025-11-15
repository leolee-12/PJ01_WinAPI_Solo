#pragma once
#include "CUI.h"
#include "CNPC.h"

class CNPCButton : public CUI
{
public:
	CNPCButton();
	virtual ~CNPCButton();

public:
	void	Initialize()		override;
	int		Update()			override;
	void	Late_Update()		override;
	void	Render(HDC hDC)		override;
	void	Release()			override;
	void	Set_Data(int iNum)	override;

private:
	CNPC::NPCID		m_eID;
	const TCHAR*	m_pText[CNPC::NPC_END];
};

