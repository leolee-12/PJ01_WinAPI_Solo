#pragma once
#include "CUI.h"
class CQuestBox : public CUI
{
public:
	enum QUESTID { Q_START, Q_TUTORIAL, Q_LAND, Q_HOUSING, Q_MERCHANT, Q_BOSS, Q_EXIT, Q_END };

public:
	CQuestBox();
	virtual ~CQuestBox();

	void	Initialize()			override;
	int		Update()				override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Set_Data(int _iData)	override;
	QUESTID	Get_Quest() { return m_eCurQuest; }


public:
	static QUESTID	m_eCurQuest;

private:
	vector<const TCHAR*>	m_vecQuestText;
	RECT					m_tTextRect;
	DWORD					m_dwTime;
	bool					m_bQuestCleared[Q_END + 1];
	int						m_iQuestEndCount[Q_END + 1];
	int						m_iCount;
};