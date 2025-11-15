#pragma once
#include "CUI.h"
#include "CNPC.h"

class CMessageBox : public CUI
{
public:
	CMessageBox();
	virtual ~CMessageBox();

	void	Initialize()			override;
	int		Update()				override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Set_Data(int _iData)	override;

private:
	vector<const TCHAR*>	m_vecText[CNPC::NPC_END];
	CNPC::NPCID				m_eNPCID;
	RECT					m_tTextRect;
	int						m_iMessageCount;
	int						m_iMessageEnd[CNPC::NPC_END];
	bool					m_bAlreadyChatted[CNPC::NPC_END];
	
};

