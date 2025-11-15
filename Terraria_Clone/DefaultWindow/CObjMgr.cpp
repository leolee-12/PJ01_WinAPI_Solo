#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"
#include "CItemMgr.h"
#include "CBmpMgr.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;
HDC		 CObjMgr::m_hNPCDC[CNPC::NPC_END];
HDC		 CObjMgr::m_hFurnitureDC[CFurniture::FT_END];

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Get_Target(OBJID eID, CObj* pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj* pTarget = nullptr;

	float	fDistance(0.f);

	for (auto& Dst : m_ObjList[eID])
	{
		if (Dst->Get_Dead())
			continue;

		float	fWidth = Dst->Get_Info()->fX - pObj->Get_Info()->fX;
		float	fHeight = Dst->Get_Info()->fY - pObj->Get_Info()->fY;

		float  fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			pTarget = Dst;
			fDistance = fDiagonal;
		}

	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj* pObj)
{
	if (eID >= OBJ_END || nullptr == pObj)
		return;

	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (iResult == OBJ_DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CObjMgr::Late_Update()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Late_Update();

			if (m_ObjList[i].empty())
				break;
		}
	}
	CCollisionMgr::Collision_RectEx(m_ObjList[PLAYER], m_ObjList[MONSTER]);
	CCollisionMgr::Collision_RectEx(m_ObjList[PLAYER], m_ObjList[BOSS]);
	CCollisionMgr::Collision_RectEx(m_ObjList[PLAYER], m_ObjList[MONSTER_BULLET]);
	CCollisionMgr::Collision_RectEx(m_ObjList[MONSTER], m_ObjList[PLAYER_BULLET]);
	CCollisionMgr::Collision_RectEx(m_ObjList[BOSS], m_ObjList[PLAYER_BULLET]);
	CCollisionMgr::Collision_RectEx(m_ObjList[MINION], m_ObjList[MONSTER]);

	if (g_bESC)
	{
		if((!m_ObjList[PLAYER].empty()) && (!m_ObjList[FURNITURE].empty()))
			CCollisionMgr::Check_NearFurniture(m_ObjList[PLAYER].front(), m_ObjList[FURNITURE]);
	}
}

void CObjMgr::Render(HDC hDC)
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Render(hDC);
		}
	}
}

void CObjMgr::Release()
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}
}

void CObjMgr::Explode(float fX, float fY, int iDamage)
{
	for (UINT i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			float	fWidth = pObj->Get_Info()->fX - fX;
			float	fHeight = pObj->Get_Info()->fY - fY;
			float	fDiagonalsq = fWidth * fWidth + fHeight * fHeight;

			if (fDiagonalsq <= 20736.f)
			{
				pObj->Set_Dead();
			}
		}
	}

}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void CObjMgr::Init_HDCs()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N00_HintBox.bmp", L"N00_HintBox");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N01_Guide_RIGHT.bmp", L"N01_Guide_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N02_Merchant_RIGHT.bmp", L"N02_Merchant_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/NPC/N03_Nurse_RIGHT.bmp", L"N03_Nurse_RIGHT");

	m_hNPCDC[CNPC::NPC_HINTBOX1]	= CBmpMgr::Get_Instance()->Find_Image(L"N00_HintBox");
	m_hNPCDC[CNPC::NPC_HINTBOX2]	= CBmpMgr::Get_Instance()->Find_Image(L"N00_HintBox");
	m_hNPCDC[CNPC::NPC_HINTBOX3]	= CBmpMgr::Get_Instance()->Find_Image(L"N00_HintBox");
	m_hNPCDC[CNPC::NPC_HINTBOX4]	= CBmpMgr::Get_Instance()->Find_Image(L"N00_HintBox");
	m_hNPCDC[CNPC::NPC_HINTBOX5]	= CBmpMgr::Get_Instance()->Find_Image(L"N00_HintBox");
	m_hNPCDC[CNPC::NPC_GUIDE]		= CBmpMgr::Get_Instance()->Find_Image(L"N01_Guide_RIGHT");
	m_hNPCDC[CNPC::NPC_MERCHANT]	= CBmpMgr::Get_Instance()->Find_Image(L"N02_Merchant_RIGHT");
	m_hNPCDC[CNPC::NPC_NURSE]		= CBmpMgr::Get_Instance()->Find_Image(L"N03_Nurse_RIGHT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT01_Door.bmp", L"FT01_Door");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT02_Table.bmp", L"FT02_Table");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT03_Chair.bmp", L"FT03_Chair");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT04_Furnace.bmp", L"FT04_Furnace");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT05_Chest.bmp", L"FT05_Chest");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT06_Anvil.bmp", L"FT06_Anvil");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT07_Wood1.bmp", L"FT07_Wood1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT08_Wood2.bmp", L"FT08_Wood2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT09_Wood3.bmp", L"FT09_Wood3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Furniture/FT10_Wood4.bmp", L"FT10_Wood4");

	m_hFurnitureDC[CFurniture::FT_DOOR] = CBmpMgr::Get_Instance()->Find_Image(L"FT01_Door");
	m_hFurnitureDC[CFurniture::FT_TABLE] = CBmpMgr::Get_Instance()->Find_Image(L"FT02_Table");
	m_hFurnitureDC[CFurniture::FT_CHAIR] = CBmpMgr::Get_Instance()->Find_Image(L"FT03_Chair");
	m_hFurnitureDC[CFurniture::FT_FURNACE] = CBmpMgr::Get_Instance()->Find_Image(L"FT04_Furnace");
	m_hFurnitureDC[CFurniture::FT_CHEST] = CBmpMgr::Get_Instance()->Find_Image(L"FT05_Chest");
	m_hFurnitureDC[CFurniture::FT_ANVIL] = CBmpMgr::Get_Instance()->Find_Image(L"FT06_Anvil");
	m_hFurnitureDC[CFurniture::FT_WOOD1] = CBmpMgr::Get_Instance()->Find_Image(L"FT07_Wood1");
	m_hFurnitureDC[CFurniture::FT_WOOD2] = CBmpMgr::Get_Instance()->Find_Image(L"FT08_Wood2");
	m_hFurnitureDC[CFurniture::FT_WOOD3] = CBmpMgr::Get_Instance()->Find_Image(L"FT09_Wood3");
	m_hFurnitureDC[CFurniture::FT_WOOD4] = CBmpMgr::Get_Instance()->Find_Image(L"FT10_Wood4");
}