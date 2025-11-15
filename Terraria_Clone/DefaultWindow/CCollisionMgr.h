#pragma once
#include "CObj.h"
#include "CItem.h"

class CCollisionMgr
{
public:
	static void	Collision_Rect(list<CObj*> Dst, list<CObj*> Src);

	static void	Collision_RectEx(list<CObj*> Dst, list<CObj*> Src);
	static bool Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight);

	static void	Collision_Circle(list<CObj*> Dst, list<CObj*> Src);
	static bool Check_Circle(CObj* pDst, CObj* pSrc);

	static void		Check_NearFurniture(CObj* pPlayer, list<CObj*> FurnitureList);
	static bool		Check_NearNPC(CObj* pPlayer, CObj* pNPC);
	static CObj*	Get_NearMonster(CObj* pDst, list<CObj*> MonsterList);
	static void		Check_Item(CObj* pPlayer, const list<CItem*>& FieldItemList);
	static CObj*	Picked_NPC();
	static int		Picked_InvenSlot();

	static int PredictCollision_Tile(CObj* pObj, float _fXSpeed, float _fYSpeed);
	static int PredictCollision_Tile(CItem* pItem, float _fXSpeed, float _fYSpeed);
};

