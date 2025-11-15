#pragma once
#include "CCharacter.h"

class CMonster : public CCharacter
{
public:
	enum MONSTERSTATE { IDLE, WALK, JUMP, SKILL1, SKILL2, SKILL3, DEAD, MS_END };

public:
	CMonster();
	virtual ~CMonster();

public:
	void			Initialize()	override;
	int				Update()		override;
	void			Late_Update()	override;
	void			Render(HDC hDC)	override;
	void			Release()		override;
	
	void			Status_Check()					override;
	void			OnCollision(COLMSG	_colMSG)	override;
	void			Move_Frame()					override;

	virtual void	Motion_Change();
	void			Search_TargetDir();
	void			Move();

	const int		Get_Damage() const { return m_iDamage; }

protected:
	MONSTERSTATE	m_ePreMotion;
	MONSTERSTATE	m_eCurMotion;
	int				m_iDamage;
	bool			m_bCanJump;
};