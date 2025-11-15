#pragma once
#include "CTileBlock.h"

class CTileMgr
{
public:
	enum BLOCKTYPE { BT_NONE, DIRT, STONE, GOLD, NWOOD, PWOOD, HONEY, ICE, CLOUD, WEB, PLATFORM, BT_END };
	enum WALLTYPE { WT_NONE, WOODWALL, WT_END };
	enum ROPETYPE { RT_NONE, ROPE, RT_END };

private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize() ;
	int Update() ;
	void Late_Update() ;
	void Render(HDC hDC) ;
	void Release() ;

public:
	void	Picking(POINT pt, int iOption, int iSetRange);
	void	Picking_RB(POINT pt, int iFrameStart);
	void	Picking_Bomb(POINT pt, int iSetRange);
	void    Save_Tile();
	void    Load_Tile();
	int		Search_CursorTile();
	void	Render_CursorTile(HDC hDC, int iIndex, int iScrollX, int iScrollY);
	void	Render_CursorTile(HDC hDC, vector<size_t>& pIndex, int iScrollX, int iScrollY);
	void	Check_SideTile(int iItemSubID, int iIndex);
	void	Init_HDCs();
	void	Set_Active(int iIndex);

	vector<CTile*>&	Get_vecTile() { return m_vecTile; }
	CTile*			Get_Tile(int iIndex);

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
			Get_Instance()->Initialize();
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
	static CTileMgr*	m_pInstance;
	vector<CTile*>		m_vecTile;
	vector<bool>		m_vecActiveIndex;

public:
	static HDC			m_hBlockDC[BT_END];
	static HDC			m_hWallDC[WT_END];
	static HDC			m_hRopeDC[RT_END];
	static HDC			m_hMouseDC;
	static HDC			m_hSlotDC;
	static HDC			m_hPickedSlotDC;
	static HDC			m_hShopSlotDC;
};

