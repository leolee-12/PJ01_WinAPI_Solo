#pragma once
#include "CCharacter.h"

class CNPC : public CCharacter
{
public:
	enum NPCSTATE	{ IDLE, WALK, JUMP, INTERACT, DEAD, NS_END };
	enum NPCID		{ NPC_HINTBOX1, NPC_HINTBOX2, NPC_HINTBOX3, NPC_HINTBOX4, NPC_HINTBOX5, NPC_GUIDE, NPC_MERCHANT, NPC_NURSE, NPC_END };

public:
	CNPC();
	virtual ~CNPC();

public:
	void			Initialize()	override;
	int				Update()		override;
	void			Late_Update()	override;
	void			Render(HDC hDC)	override;
	void			Release()		override;

	void			Set_NPCID(NPCID eID) { m_eNPCID = eID; }

	void			Status_Check()					override;
	void			OnCollision(COLMSG	_colMSG)	override;
	void			Move_Frame()					override;
	virtual void	Interaction();

	virtual void	Motion_Change();
	void			Pattern_Change();
	void			Search_TargetDir();
	void			Move();
	static	void	NPC_Function(NPCID _eID);

protected:
	NPCSTATE		m_eCurMotion;
	NPCSTATE		m_ePreMotion;
	NPCID			m_eNPCID;
	int				m_iMessageCount;
};

