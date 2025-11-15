#pragma once
#include "Define.h"

class CObj
{
public:
	CObj();
	CObj(ITEMINFO tItemInfo);
	CObj(EFFECTID eID, int iEffectCode);
	virtual ~CObj();

public:
	void			Set_Size(float fCX, float fCY) {	m_tInfo.fCX = fCX;
														m_tInfo.fCY = fCY; }
	void			Set_Pos(float fX, float fY) {	m_tInfo.fX = fX;
													m_tInfo.fY = fY; }
	void			Set_Speed(float fX = 0, float fY = 0) {	m_fXSpeed = fX;
															m_fYSpeed = fY; }
	void			Add_PosX(float fX) { m_tInfo.fX += fX; }
	void			Add_PosY(float fY) { m_tInfo.fY += fY; }
	void			Add_XSpeed(float fX) { m_fXSpeed += fX; }
	void			Add_YSpeed(float fY) { m_fYSpeed += fY; }
	virtual void	Set_Angle(float fAngle) { m_fAngle = fAngle; }
	void			Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	void			Set_Dead() { m_bDead = true; }
	void			Set_Info(INFO tInfo) { m_tInfo = tInfo; }
	void			Set_Frame(FRAME& _tFrame) { m_tFrame = _tFrame; }
	void			Set_FrameStart(int _iFrameStart) { m_tFrame.iStart = _iFrameStart; }
	void			Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void			Set_ObjID(OBJID eID) { m_eObjID = eID; }
	void			Set_vecTile(vector<size_t> vecTile) { m_vecTile = vecTile; }
	bool			Get_Angle() { return m_fAngle; }
	bool			Get_Dead() { return m_bDead; }
	const INFO*		Get_Info() { return &m_tInfo; }
	const RECT*		Get_Rect() { return &m_tRect; }
	const FRAME*	Get_Frame() { return &m_tFrame; }
	const TCHAR*	Get_FrameKey() { return m_pFrameKey; }
	const OBJID		Get_ObjID() { return m_eObjID; }
	vector<size_t>&	Get_vecTile() { return m_vecTile; }

	void			Search_ObjTile(float _fX, float _fY);
	void			Update_Rect();

public:
	virtual void	Initialize()	PURE;
	virtual int		Update()		PURE;
	virtual void	Late_Update()	PURE;
	virtual void	Render(HDC hDC)	PURE;
	virtual void	Release()		PURE;
	virtual void	Status_Check() {};
	virtual void	OnCollision(COLMSG _colMSG) {};

protected:
	virtual void	Move_Frame();
	virtual void	Gravity() {};

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	FRAME			m_tFrame;
	const TCHAR*	m_pFrameKey;
	CObj*			m_pTarget;
	OBJID			m_eObjID;

	float			m_fTime[TM_END];
	float			m_fXSpeed;
	float			m_fYSpeed;
	float			m_fAngle;
	bool			m_bDead;
	bool			m_bAir;

	vector<size_t>	m_vecTile;
};

