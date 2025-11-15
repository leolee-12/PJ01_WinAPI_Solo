#pragma once
#include "CObj.h"
#include "CFurniture.h"
#include "CNPC.h"

class CObjMgr
{
private:
	CObjMgr();
	CObjMgr(const CObjMgr& rhs) = delete;		// 삭제
	CObjMgr& operator=(CObjMgr& rObj) = delete;	// 삭제
	~CObjMgr();

public:
	CObj*	Get_Target(OBJID eID, CObj* pObj);		

public:
	void	Add_Object(OBJID eID, CObj* pObj);
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
	void	Explode(float fX, float fY, int iDamage);

	void			Delete_ID(OBJID eID);
	list<CObj*>*	Get_ObjList(OBJID eID) { return &m_ObjList[eID]; }
	void			Init_HDCs();

public:
	static HDC	m_hNPCDC[CNPC::NPC_END];
	static HDC	m_hFurnitureDC[CFurniture::FT_END];

private:
	list<CObj*>		m_ObjList[OBJ_END];

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;

			m_pInstance->Init_HDCs();
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}


private:
	static CObjMgr* m_pInstance;
};

