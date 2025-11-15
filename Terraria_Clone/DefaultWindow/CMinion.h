#pragma once
#include "CCharacter.h"

class CMinion : public CCharacter
{
public:
	enum MINIONSTATE { IDLE, WALK, ATTACK, DEAD};
public:
	CMinion();
	virtual ~CMinion();

public:
	void		Initialize()	override;
	int			Update()		override;
	void		Late_Update()	override;
	void		Render(HDC hDC)	override;
	void		Release()		override;

	void		Status_Check()					override;
	void		OnCollision(COLMSG	_colMSG)	override;
	void		Move_Frame()					override;
	void		Motion_Change();
	void		Search_TargetDir();
	void		Move();

	const int	Get_Damage() const { return m_iDamage; }

protected:
	MINIONSTATE		m_ePreMotion;
	MINIONSTATE		m_eCurMotion;
	int				m_iDamage;
	float			m_fTargetDistance;
};