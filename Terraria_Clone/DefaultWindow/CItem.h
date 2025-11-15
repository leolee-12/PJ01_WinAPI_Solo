#pragma once
#include "Define.h"
#include "CObj.h"

class CItem : public CObj
{
public:
	enum ITEMSTATE { FIELD, INVEN, HAND, IS_END };

public:
	CItem();
	CItem(ITEMINFO tItemInfo);
	virtual ~CItem();

public:
	void			Initialize()	override;
	int				Update()		override;
	void			Late_Update()	override {}
	void			Release()		override {}
	virtual void	Use_Item(CObj* pPlayer)	PURE;
	virtual void	OnCollision(COLMSG _colMSG);

	void			Set_ItemState(ITEMSTATE eState) { m_eItemState = eState; }
	void			Set_ItemID(ITEMID _eItemID) { m_eItemID = _eItemID; }
	void			Set_ItemSubID(UINT _iItemSubID) { m_iItemSubID = _iItemSubID; }
	void			Set_ItemCode(int _iItemCode) { m_iItemCode = _iItemCode; }
	void			Set_iTile(UINT iIndex) { m_iTile = iIndex; }
	void			Set_Info(INFO tInfo) { m_tInfo = tInfo; }

	ITEMID			Get_ItemID() { return m_eItemID; }
	UINT			Get_ItemSubID() { return m_iItemSubID; }
	UINT			Get_ItemCode() { return m_iItemCode; }
	UINT			Get_iTile() { return m_iTile; }
	const CObj*		Get_Target() { return m_pTarget; }
	int				Get_Count() { return m_iCount; }
	void			Add_Count(int iCount) { m_iCount += iCount; }
	virtual void	Render_Slot(HDC hDC, float _fX, float _fY);
	static	void	Render_Slot(HDC hDC, ITEMINFO tItemInfo, float _fX, float _fY, float fScale = 1.0f);	// 정보만으로 렌더링하기
	virtual void	Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle);

	void			Search_TargetDir();
	void			Search_ObjTile(float _fX, float _fY);
	void			Status_Check();
	void			Gravity();
	void			Move();

protected:
	POINT			m_ptHand[3];
	ITEMSTATE		m_eItemState;
	ITEMID			m_eItemID;
	UINT			m_iItemSubID;
	UINT			m_iItemCode;
	UINT			m_iTile;

	bool			m_bUse;
	int				m_iCount;
};