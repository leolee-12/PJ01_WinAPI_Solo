#pragma once
#include "CObj.h"
#include "CTileBlock.h"
#include "CTileSlot.h"
#include "CItem.h"
#include "CUI.h"
#include "CEffect.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	static CObj* Create_Obj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_Obj(float fX, float fY, float fAngle = 0.f)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(fX, fY);
		pObj->Set_Angle(fAngle);
		return pObj;
	}

	static CTile* Create_Tile(float fX = 0.f, float fY = 0.f)
	{
		CTile* pTile = new T;
		pTile->Initialize();
		pTile->Set_Pos(fX, fY);
		pTile->Update_Rect();
		return pTile;
	}

	static CUI* Create_UI()
	{
		CUI* pUI = new T;
		pUI->Initialize();
		return pUI;
	}

	static CItem* Create_Item(float fX, float fY, ITEMINFO tItemInfo)
	{
		CItem* pItem = new T(tItemInfo);
		pItem->Initialize();
		pItem->Set_Pos(fX, fY);
		return pItem;
	}

	static CEffect* Create_Effect(float fX, float fY, float fAngle, EFFECTID eID, int iEffectCode)
	{
		CEffect* pEffect = new T(eID, iEffectCode);
		pEffect->Initialize();
		pEffect->Set_Pos(fX, fY);
		pEffect->Set_Angle(fAngle);
		return pEffect;
	}
};
