#pragma once
#include "Define.h"
#include "CObj.h"

class CEffect : public CObj
{
public:
	CEffect();
	CEffect(EFFECTID eID, int iEffectCode);
	virtual ~CEffect();

public:
	EFFECTID	Get_EffectID() { return m_eEffectID; }
	int			Get_EffectCode() { return m_iEffectCode; }

public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
	void	OnCollision(COLMSG _colMSG);

protected:
	void	Gravity();
	void	Move();

protected:
	EFFECTID		m_eEffectID;

	int				m_iEffectCode;
	float			m_fLifeTime;

};

