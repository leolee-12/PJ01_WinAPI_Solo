#pragma once
#include "CMonster.h"

class CMonster_N1 : public CMonster
{
public:
	CMonster_N1();
	virtual ~CMonster_N1();

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

	const int	Get_Damage() const { return m_iDamage; }
};