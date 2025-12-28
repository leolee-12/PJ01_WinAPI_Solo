#include "pch.h"
#include "CCollisionMgr.h"
#include "CTileMgr.h"
#include "CPlayer.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CUIMgr.h"
#include "CInven_Bag.h"

void CCollisionMgr::Collision_Rect(list<CObj*> Dst, list<CObj*> Src)
{
	RECT	rc{};

	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
			{
				Dst->OnCollision(COLMSG(Src, COLTYPE::CD_END, 0, 0));
				Src->OnCollision(COLMSG(Dst, COLTYPE::CD_END, 0, 0));
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx(list<CObj*> Dst, list<CObj*> Src)
{
	float	fWidth(0.f), fHeight(0.f);

	for (auto& Dst : Dst)
	{
		if(Dst->Get_Dead())
			continue;

		for (auto& Src : Src)
		{
			if(Src->Get_Dead())
				continue;

			if (Check_Rect(Dst, Src, &fWidth, &fHeight))
			{
				// 상 하 충돌
				if (fWidth + 5.f >= fHeight)
				{
					// 상 충돌
					if (Dst->Get_Info()->fY < Src->Get_Info()->fY)
					{
						Dst->OnCollision(COLMSG(Src, COLTYPE::CD_BOTTOM, 0, -fHeight));
						Src->OnCollision(COLMSG(Dst, COLTYPE::CD_TOP, 0, fHeight));
					}

					// 하 충돌
					else
					{
						Dst->OnCollision(COLMSG(Src, COLTYPE::CD_TOP, 0, fHeight));
						Src->OnCollision(COLMSG(Dst, COLTYPE::CD_BOTTOM, 0, -fHeight));
					}
				}

				else // 좌우 충돌
				{
					// 좌 충돌
					if (Dst->Get_Info()->fX < Src->Get_Info()->fX)
					{
						Dst->OnCollision(COLMSG(Src, COLTYPE::CD_RIGHT, -fWidth, 0));
						Src->OnCollision(COLMSG(Dst, COLTYPE::CD_LEFT, fWidth, 0));
					}

					// 우 충돌
					else
					{
						Dst->OnCollision(COLMSG(Src, COLTYPE::CD_LEFT, fWidth, 0));
						Src->OnCollision(COLMSG(Dst, COLTYPE::CD_RIGHT, -fWidth, 0));
					}
				}
			}
		}
	}
}

bool CCollisionMgr::Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight)
{
	float fHorizontal = abs(pDst->Get_Info()->fX - pSrc->Get_Info()->fX);
	float fVertical  = abs(pDst->Get_Info()->fY - pSrc->Get_Info()->fY);

	float	fRadiusX = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;
	float	fRadiusY = (pDst->Get_Info()->fCY + pSrc->Get_Info()->fCY) * 0.5f;

	if ((fRadiusX > fHorizontal) && (fRadiusY > fVertical))
	{
		*pWidth = fRadiusX - fHorizontal;
		*pHeight = fRadiusY - fVertical;
		return true;
	}
	return false;
}

void CCollisionMgr::Collision_Circle(list<CObj*> Dst, list<CObj*> Src)
{
	for (auto& Dst : Dst)
	{
		for (auto& Src : Src)
		{
			if (Check_Circle(Dst, Src))
			{
				Dst->OnCollision(COLMSG(Src, COLTYPE::CD_END, 0, 0));
				Src->OnCollision(COLMSG(Dst, COLTYPE::CD_END, 0, 0));
			}
		}
	}	
}

bool CCollisionMgr::Check_Circle(CObj* pDst, CObj* pSrc)
{
	float	fWidth = pDst->Get_Info()->fX - pSrc->Get_Info()->fX;
	float	fHeight = pDst->Get_Info()->fY - pSrc->Get_Info()->fY;
	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);
	float	fRadius = (pDst->Get_Info()->fCX + pSrc->Get_Info()->fCX) * 0.5f;

	return fRadius >= fDiagonal;
}

void CCollisionMgr::Check_NearFurniture(CObj* pPlayer, list<CObj*> Src)
{
	for (auto& pFurniture : Src)
	{
		float	fWidth = pPlayer->Get_Info()->fX - pFurniture->Get_Info()->fX;
		float	fHeight = pPlayer->Get_Info()->fY - pFurniture->Get_Info()->fY;
		float	fDiagonalsq = fWidth * fWidth + fHeight * fHeight;

		if (fDiagonalsq < 4096.f)
		{
			pFurniture->OnCollision(COLMSG(pPlayer, CD_END, 0.f, 0.f, true));
		}
		else
		{
			pFurniture->OnCollision(COLMSG(pPlayer, CD_END, 0.f, 0.f, false));
		}
	}
}

bool CCollisionMgr::Check_NearNPC(CObj* pPlayer, CObj* pNPC)
{
	float	fWidth = pPlayer->Get_Info()->fX - pNPC->Get_Info()->fX;
	float	fHeight = pPlayer->Get_Info()->fY - pNPC->Get_Info()->fY;
	float	fDiagonalsq = fWidth * fWidth + fHeight * fHeight;

	if (fDiagonalsq < 9216.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CObj* CCollisionMgr::Get_NearMonster(CObj* pDst, list<CObj*> MonsterList)
{
	CObj* pTemp = nullptr;
	float fMin = 102400.f;

	for (auto& pMonster : MonsterList)
	{
		float	fWidth = pDst->Get_Info()->fX - pMonster->Get_Info()->fX;
		float	fHeight = pDst->Get_Info()->fY - pMonster->Get_Info()->fY;
		float	fDiagonalsq = fWidth * fWidth + fHeight * fHeight;

		if (fMin >= fDiagonalsq)
		{
			fMin = fDiagonalsq;
			pTemp = pMonster;
		}
	}

	return pTemp;
}

void CCollisionMgr::Check_Item(CObj* pPlayer, const list<CItem*>& FieldItemList)
{
	for (auto& pItem : FieldItemList)
	{
		float	fWidth = pPlayer->Get_Info()->fX - pItem->Get_Info()->fX;
		float	fHeight = pPlayer->Get_Info()->fY - pItem->Get_Info()->fY;
		float	fDiagonalsq = fWidth * fWidth + fHeight * fHeight;

		if (fDiagonalsq < 256.f)
		{
			pItem->OnCollision(COLMSG(pPlayer, CD_END, 0.f, 0.f, true));
		}
		else if (fDiagonalsq < 4096.f)
		{
			pItem->OnCollision(COLMSG(pPlayer, CD_END, 0.f, 0.f, false));
		}
		else
		{
			pItem->OnCollision(COLMSG(nullptr, CD_END, 0.f, 0.f));
		}
	}
}

CObj* CCollisionMgr::Picked_NPC()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	for (auto& pNPC : *(CObjMgr::Get_Instance()->Get_ObjList(NPC)))
	{
		if (PtInRect(pNPC->Get_Rect(), pt))
			return pNPC;
	}

	return nullptr;
}

int CCollisionMgr::Picked_InvenSlot()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	//pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	//pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	const vector<CTile*>* tempVec = static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG))->Get_vecSlot();
	size_t len = tempVec->size();

	for (int i = 0; i < len; ++i)
	{
		if (PtInRect((*tempVec)[i]->Get_Rect(), pt))
			return i;
	}

	return -1;
}

int CCollisionMgr::PredictCollision_Tile(CObj* pObj, float _fXSpeed, float _fYSpeed)
{
	CTileMgr* pTileMgr = CTileMgr::Get_Instance();

	// X축
	float	fPredictX = 0.f;
	float	fPredictY = 0.f;
	vector<size_t> vecTemp;
	
	if (_fXSpeed != 0.f)
	{
		fPredictX = pObj->Get_Info()->fX + _fXSpeed;
		fPredictY = pObj->Get_Info()->fY;
		vecTemp = pObj->Get_vecTile();
		pObj->Search_ObjTile(fPredictX, fPredictY + pObj->Get_Info()->fCY * 0.5f);

		for (size_t& i : pObj->Get_vecTile())
		{
			CTile* pTile = pTileMgr->Get_Tile(i);

			if(pTile == nullptr) continue;

			if (pTile->Get_Option(CTile::TILE_BLOCK) != 0)
			{
				pObj->Set_vecTile(vecTemp);
				return PCT_X;
			}
		}
	}

	// Y축
	if (_fYSpeed != 0.f)
	{
		fPredictX = pObj->Get_Info()->fX;
		fPredictY = pObj->Get_Info()->fY + _fYSpeed;
		vecTemp = pObj->Get_vecTile();
		pObj->Search_ObjTile(fPredictX, fPredictY + pObj->Get_Info()->fCY * 0.5f);

		for (auto& i : pObj->Get_vecTile())
		{
			CTile* pTile = pTileMgr->Get_Tile(i);

			if (pTile == nullptr) continue;

			if (pTile->Get_Option(CTile::TILE_BLOCK) != 0)
			{
				pObj->Set_vecTile(vecTemp);
				return PCT_Y;
			}
		}
	}

	return PCT_NONE;
}

int CCollisionMgr::PredictCollision_Tile(CItem* pItem, float _fXSpeed, float _fYSpeed)
{
	CTileMgr* pTileMgr = CTileMgr::Get_Instance();

	// X축
	float	fPredictX = 0.f;
	float	fPredictY = 0.f;
	UINT iTemp;

	if (_fXSpeed != 0.f)
	{
		fPredictX = pItem->Get_Info()->fX + _fXSpeed;
		fPredictY = pItem->Get_Info()->fY;
		iTemp = pItem->Get_iTile();
		pItem->Search_ObjTile(fPredictX, fPredictY + pItem->Get_Info()->fCY * 0.5f);

		CTile* pTile = pTileMgr->Get_Tile(pItem->Get_iTile());

		if(pTile == nullptr) return PCT_NONE;

		if (pTile->Get_Option(CTile::TILE_BLOCK) != 0)
		{
			pItem->Set_iTile(iTemp);
			return PCT_X;
		}
	}

	// Y축
	if (_fYSpeed != 0.f)
	{
		fPredictX = pItem->Get_Info()->fX;
		fPredictY = pItem->Get_Info()->fY + _fYSpeed;
		iTemp = pItem->Get_iTile();
		pItem->Search_ObjTile(fPredictX, fPredictY + pItem->Get_Info()->fCY * 0.5f);

		CTile* pTile = pTileMgr->Get_Tile(pItem->Get_iTile());

		if (pTile == nullptr) return PCT_NONE;

		if (pTile->Get_Option(CTile::TILE_BLOCK) != 0)
		{
			pItem->Set_iTile(iTemp);
			return PCT_Y;
		}
	}

	return PCT_NONE;
}
