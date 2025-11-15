#pragma once
#include "CObj.h"

class CCharacter : public CObj
{
public:
	CCharacter();
	virtual ~CCharacter();

public:
	void		Initialize()	override;
	int			Update()		override;
	void		Late_Update()	override;
	void		Render(HDC hDC)	override;
	void		Release()		override;

public:
	void		Gravity()		override;
	void		OnCollision(COLMSG	_colMSG) override;

	void		Set_HP(int _iHP) { m_iCurHP = _iHP; }
	void		Set_MaxHP(int _iHP) { m_iMaxHP = _iHP; }

	int			Get_HP() { return m_iCurHP; }
	int			Get_MaxHP() { return m_iMaxHP; }

protected:
	POINT		m_tPosin;
	DWORD		m_dwIvTime;

	float		m_fDistance;
	float		m_fFallingDistance;
	int			m_iMaxHP;
	int			m_iCurHP;
};