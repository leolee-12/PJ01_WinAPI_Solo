#pragma once
#include "CCharacter.h"
#include "CItem.h"
#include "CMouse.h"

class CPlayer : public CCharacter
{
public:
	enum PLAYERSTATE	{ IDLE, WALK, JUMP, USE, DEAD, PS_END };
	enum JUMPSTATE		{ CANJUMP, UPWARD, DOWNWARD, JS_END };

public:
	CPlayer();
	virtual ~CPlayer();

public:
	void		Initialize()	override;
	int			Update()		override;
	void		Late_Update()	override;
	void		Render(HDC hDC)	override;
	void		Release()		override;

	void		Move_Frame()	override;
	void		Status_Check()	override;
	void		OnCollision(COLMSG _colMSG) override;
	void		Gravity()		override;

	void		Set_TargetMouse(CMouse* pMouse) { m_pTargetMouse = pMouse; }
	void		Set_HandItem(CItem* _pItem) { m_pHandItem = _pItem; }
	void		Set_HandRect(RECT _tRect) { m_tHandRect = _tRect; }
	void		Set_HandFrameKey(const TCHAR* _pFrameKey) { m_pHandFrameKey = _pFrameKey; }
	void		Set_GravityState() {	m_bGravityPotion = true;
										m_fTime[TM_BUFF] = GetTickCount(); }
	CMouse*		Get_TargetMouse() { return m_pTargetMouse; }
	CItem*		Get_HandItem() { return m_pHandItem; }
	bool		Get_GravityState() { return m_bGravityPotion; }
	
private:
	void		Key_Input();
	void		MoveKey_Input();
	void		OptionKey_Input();
	void		MotionKey_Input();

	void		Offset();
	void		Motion_Change();
	void		Search_TargetDir();

	void		Move();
	void		Update_HandRect();

private:
	PLAYERSTATE		m_ePreMotion;
	PLAYERSTATE		m_eCurMotion;
	JUMPSTATE		m_eJumpState;
	CMouse*			m_pTargetMouse;
	CItem*			m_pHandItem;
	RECT			m_tHandRect;
	const TCHAR*	m_pHandFrameKey;

	bool			m_bUsing;
	float			m_fMaxSpeed;
	float			m_fMaxJumpSpeed;
	bool			m_bGravityPotion;
	int				m_iGravityDir;
	float			m_fHandAngle;
};