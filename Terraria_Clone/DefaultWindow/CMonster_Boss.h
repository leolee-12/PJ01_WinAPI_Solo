#pragma once
#include "CMonster.h"

class CMonster_Boss : public CMonster
{
public:
	enum BOSS_PATTERN { BP_NONE1, BP_NONE2, BP_SKILL1, BP_SKILL2, BP_SKILL3, BP_END };

public:
	CMonster_Boss();
	virtual ~CMonster_Boss();

public:
	void		Initialize()	override;
	int			Update()		override;
	void		Late_Update()	override;
	void		Render(HDC hDC)	override;
	void		Release()		override;

	void		Status_Check()					override;
	void		OnCollision(COLMSG	_colMSG)	override;
	void		Move_Frame()					override;
	void		Motion_Change()					override;
	void		Pattern_Move();
	void		Pattern_Change();
	void		Pattern_Change(BOSS_PATTERN _ePattern) { m_ePattern = _ePattern; }

	const int	Get_Damage() const { return m_iDamage; }

private:
	BOSS_PATTERN	m_ePattern;
	float			m_fScale;
};