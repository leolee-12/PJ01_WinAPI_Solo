#include "pch.h"
#include "CTileMgr.h"
#include "CAbstractFactory.h"
#include "CScrollMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CPlayer.h"

CTileMgr* CTileMgr::m_pInstance = nullptr;
HDC CTileMgr::m_hBlockDC[BT_END] = {};
HDC CTileMgr::m_hWallDC[WT_END] = {};
HDC CTileMgr::m_hRopeDC[RT_END] = {};
HDC CTileMgr::m_hMouseDC = nullptr;
HDC CTileMgr::m_hSlotDC = nullptr;
HDC CTileMgr::m_hPickedSlotDC = nullptr;
HDC CTileMgr::m_hShopSlotDC = nullptr;

CTileMgr::CTileMgr()
{
	m_vecTile.reserve(TILEX * TILEY);
	m_vecActiveIndex.reserve(TILEX * TILEY);
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fX = float((TILECX >> 1) + (TILECX * j));
			float fY = float((TILECY >> 1) + (TILECY * i));

			CTile* pTile = CAbstractFactory<CTileBlock>::Create_Tile(fX, fY);
			m_vecTile.push_back(pTile);
			m_vecActiveIndex.push_back(false);
		}
	}

	Init_HDCs();
}

int CTileMgr::Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Update();

	return 0;
}

void CTileMgr::Late_Update()
{
	for (auto& pTile : m_vecTile)
		pTile->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iCullX = abs(iScrollX / TILECX);
	int iCullY = abs(iScrollY / TILECY);

	int iMaxX = WINCX / TILECX + iCullX + 2;
	int iMaxY = WINCY / TILECY + iCullY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			int	iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			if(m_vecActiveIndex[iIndex])
				m_vecTile[iIndex]->Render_Tile(hDC, iScrollX, iScrollY);
		}
	}

 	Render_CursorTile(hDC, Search_CursorTile(), iScrollX, iScrollY);

	if (!CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->empty())
		Render_CursorTile(hDC, (CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front())->Get_vecTile(), iScrollX, iScrollY);
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CTile*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Picking(POINT pt, int iBlockOption, int iSetRange)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;
	int iIndex = y * TILEX + x;

	for (int i = 0; i < (2 * iSetRange + 1); ++i)
	{
		for (int j = 0; j < (2 * iSetRange + 1); ++j)
		{
			int tempIndex = iIndex - TILEX * (i - iSetRange) + (j - iSetRange);

			if (0 > tempIndex || tempIndex >= m_vecTile.size())
				continue;

			m_vecTile[tempIndex]->Set_Option(CTile::TILE_BLOCK, iBlockOption);
			m_vecTile[tempIndex]->Check_Tile();
			Set_Active(tempIndex);
		}
	}
}

void CTileMgr::Picking_RB(POINT pt, int iFrameStart)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int iIndex = y * TILEX + x;

	m_vecTile[iIndex]->Set_FrameStart(iFrameStart);
}

void CTileMgr::Picking_Bomb(POINT pt, int iSetRange)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;
	int iIndex = y * TILEX + x;

	for (int i = 0; i < (2 * iSetRange + 1); ++i)
	{
		for (int j = 0; j < (2 * iSetRange + 1); ++j)
		{
			int tempIndex = iIndex - TILEX * (i - iSetRange) + (j - iSetRange);

			if (0 > tempIndex || tempIndex >= m_vecTile.size())
				continue;
			
			m_vecTile[tempIndex]->Set_Option(CTile::TILE_BLOCK, 0);
			m_vecTile[tempIndex]->Set_Option(CTile::TILE_WALL, 0);
			m_vecTile[tempIndex]->Check_Tile();
			Set_Active(tempIndex);
		}
	}
}

void CTileMgr::Save_Tile()
{
	HANDLE	hFile = CreateFile(	L"../Data/Tile.dat",	// 파일이름이 포함된 경로
								GENERIC_WRITE,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
								NULL,					// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
								NULL,					// 보안 모드 설정
								CREATE_ALWAYS,			// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
								FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일ㄹ
								NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"File Open Failed", _T("FAIL"), MB_OK);
		return;
	}

	DWORD		dwByte(0);

	for (auto& pTile : m_vecTile)
	{
		WriteFile(hFile, pTile->Get_Info(), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, static_cast<CTileBlock*>(pTile)->Get_TileInfo(), sizeof(TILEINFO), &dwByte, nullptr);
		WriteFile(hFile, pTile->Get_Frame(), sizeof(FRAME), &dwByte, nullptr);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"Save Success", _T("SUCCESS"), MB_OK);
}

void CTileMgr::Load_Tile()
{
	HANDLE	hFile = CreateFile(	L"../Data/Tile.dat",	// 파일이름이 포함된 경로
								GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
								NULL,					// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
								NULL,					// 보안 모드 설정
								OPEN_EXISTING,			// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
								FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일ㄹ
								NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"File Open Failed", _T("FAIL"), MB_OK);
		return;
	}

	DWORD		dwByte(0);
	INFO		tInfo{};
	TILEINFO	tTileInfo{};
	FRAME		tFrame{};
	/*Release();*/
	size_t idx(0);

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &tTileInfo, sizeof(TILEINFO), &dwByte, nullptr);
		ReadFile(hFile, &tFrame, sizeof(FRAME), &dwByte, nullptr);
		
		if (0 == dwByte)
			break;

		static_cast<CTileBlock*>(m_vecTile[idx])->Set_TileInfo(tTileInfo);
		m_vecTile[idx]->Set_Frame(tFrame);
		m_vecTile[idx]->Check_Tile();
		m_vecActiveIndex[idx] = ((tTileInfo.iBlock) || (tTileInfo.iWall) || (tTileInfo.iRope));	// 하나라도 0이 아니면 true
		idx++;
	}
	
	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Load Success", _T("SUCCESS"), MB_OK);
}

int CTileMgr::Search_CursorTile()
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;
	int iIndex = y * TILEX + x;

	return iIndex;
}

void CTileMgr::Render_CursorTile(HDC hDC, int iIndex, int iScrollX, int iScrollY)
{
	if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
		return;

	GdiTransparentBlt(	hDC,
						(int)(m_vecTile[iIndex]->Get_Rect()->left) + iScrollX,
						(int)(m_vecTile[iIndex]->Get_Rect()->top) + iScrollY,
						(int)TILECX, (int)TILECY,
						m_hMouseDC, 0, 0,
						(int)TILECX, (int)TILECY, RGB(255, 0, 255));
}

void CTileMgr::Render_CursorTile(HDC hDC, vector<size_t>& pIndex, int iScrollX, int iScrollY)
{
	for (size_t i = 0; i < pIndex.size(); ++i)
	{
		if (0 > pIndex[i] || pIndex[i] >= m_vecTile.size())
			return;

		GdiTransparentBlt(hDC,
			(int)(m_vecTile[pIndex[i]]->Get_Rect()->left) + iScrollX,
			(int)(m_vecTile[pIndex[i]]->Get_Rect()->top) + iScrollY,
			(int)TILECX, (int)TILECY,
			m_hMouseDC, 0, 0,
			(int)TILECX, (int)TILECY, RGB(255, 0, 255));
	}
}

void CTileMgr::Check_SideTile(int iItemSubID, int iIndex)
{
	size_t len = m_vecTile.size();

	switch (iItemSubID)
	{
	case 0:
	{
		int iTemp = iIndex;			// MySelf
		if (0 <= iTemp && iTemp < len)
			static_cast<CTileBlock*>(m_vecTile[iTemp])->Change_BlockDir();

		iTemp = iIndex - 1;			// Left
		if (0 <= iTemp && iTemp < len)
			static_cast<CTileBlock*>(m_vecTile[iTemp])->Change_BlockDir();

		iTemp = iIndex - TILEX;		// Top
		if (0 <= iTemp && iTemp < len)
			static_cast<CTileBlock*>(m_vecTile[iTemp])->Change_BlockDir();

		iTemp = iIndex + 1;			// Right
		if (0 <= iTemp && iTemp < len)
			static_cast<CTileBlock*>(m_vecTile[iTemp])->Change_BlockDir();

		iTemp = iIndex + TILEX;		// Bottom
		if (0 <= iTemp && iTemp < len)
			static_cast<CTileBlock*>(m_vecTile[iTemp])->Change_BlockDir();
	}
	break;
	}
}

CTile* CTileMgr::Get_Tile(int iIndex)
{
	if (0 > iIndex || iIndex >= m_vecTile.size())
		return m_vecTile[0];

	else
		return m_vecTile[iIndex];
}

void CTileMgr::Init_HDCs()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block01_Dirt.bmp", L"Block01_Dirt");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block02_Stone.bmp", L"Block02_Stone");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block03_Gold.bmp", L"Block03_Gold");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block04_NaturalWood.bmp", L"Block04_NaturalWood");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block05_ProcessedWood.bmp", L"Block05_ProcessedWood");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block06_Honey.bmp", L"Block06_Honey");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block07_Ice.bmp", L"Block07_Ice");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block08_Cloud.bmp", L"Block08_Cloud");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block09_Grass.bmp", L"Block09_Grass");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Block10_WoodPlatform.bmp", L"Block10_WoodPlatform");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Wall01_WoodWall2.bmp", L"Wall01_WoodWall");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Rope01_Rope.bmp", L"Rope01_Rope");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Mouse/CheckMouse.bmp", L"CheckMouse");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Inventory_Top.bmp", L"Inventory_Top");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Inventory_Picked.bmp", L"Inventory_Picked");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_Inventory_Shop.bmp", L"Inventory_Shop");

	m_hMouseDC = CBmpMgr::Get_Instance()->Find_Image(L"CheckMouse");
	m_hSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"Inventory_Top");
	m_hPickedSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"Inventory_Picked");
	m_hShopSlotDC = CBmpMgr::Get_Instance()->Find_Image(L"Inventory_Shop");

	m_hBlockDC[DIRT] = CBmpMgr::Get_Instance()->Find_Image(L"Block01_Dirt");
	m_hBlockDC[STONE] = CBmpMgr::Get_Instance()->Find_Image(L"Block02_Stone");
	m_hBlockDC[GOLD] = CBmpMgr::Get_Instance()->Find_Image(L"Block03_Gold");
	m_hBlockDC[NWOOD] = CBmpMgr::Get_Instance()->Find_Image(L"Block04_NaturalWood");
	m_hBlockDC[PWOOD] = CBmpMgr::Get_Instance()->Find_Image(L"Block05_ProcessedWood");
	m_hBlockDC[HONEY] = CBmpMgr::Get_Instance()->Find_Image(L"Block06_Honey");
	m_hBlockDC[ICE] = CBmpMgr::Get_Instance()->Find_Image(L"Block07_Ice");
	m_hBlockDC[CLOUD] = CBmpMgr::Get_Instance()->Find_Image(L"Block08_Cloud");
	m_hBlockDC[WEB] = CBmpMgr::Get_Instance()->Find_Image(L"Block09_Grass");
	m_hBlockDC[PLATFORM] = CBmpMgr::Get_Instance()->Find_Image(L"Block10_WoodPlatform");

	m_hWallDC[WOODWALL] = CBmpMgr::Get_Instance()->Find_Image(L"Wall01_WoodWall");

	m_hRopeDC[ROPE] = CBmpMgr::Get_Instance()->Find_Image(L"Rope01_Rope");
}

void CTileMgr::Set_Active(int iIndex)
{
	CTileBlock* tempBlock = static_cast<CTileBlock*>(m_vecTile[iIndex]);

	m_vecActiveIndex[iIndex] =	((tempBlock->Get_Option(CTile::TILE_BLOCK)) ||
								(tempBlock->Get_Option(CTile::TILE_WALL)) ||
								(tempBlock->Get_Option(CTile::TILE_ROPE)));
}
