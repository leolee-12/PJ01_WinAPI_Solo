#include "pch.h"
#include "CCraft.h"
#include "CUIMgr.h"
#include "CItemMgr.h"
#include "CInven_Bag.h"
#include "CItem_Block.h"
#include "CItem_Furniture.h"
#include "CItem_Nonconsumable.h"
#include "CItem_Consumable.h"
#include "CItem_Weapon.h"
#include "CItem_Armor.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CTextMgr.h"
#include "CSoundMgr.h"

CCraft::CCraft()
	:	m_iSelectIndex(0),
		m_pInven_Bag(nullptr)
{
	m_vecRawSlot.reserve(3);
	m_vecProductSlot.reserve(7);
	m_vecRender.reserve(30);
	ZeroMemory(m_bCraftLevel, sizeof(m_bCraftLevel));
}

CCraft::~CCraft()
{
	Release();
}

void CCraft::Initialize()
{
	m_pInven_Bag = CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG);

	m_bCraftLevel[CL_HAND] = true;

	for (int i = 0; i < 7; ++i)
	{
		m_vecProductSlot.push_back(CAbstractFactory<CTileSlot>::Create_Tile(30.f, float((WINCY >> 1) + 30.f + 52.f * i)));
	}

for (int i = 0; i < 3; ++i)
{
	m_vecRawSlot.push_back(CAbstractFactory<CTileSlot>::Create_Tile(82.f + 32.f * i, float((WINCY >> 1) + 170.f)));
}
}

int CCraft::Update()
{
	if (!g_bESC)
		return OBJ_NOEVENT;

	return OBJ_NOEVENT;
}

void CCraft::Late_Update()
{
	if (!g_bESC)
		return;

	m_vecRender.clear();

	if (m_bCraftLevel[CL_HAND])
	{
		for (auto& tRecipe : m_mapRecipeLV0)
		{
			if (Check_Recipe(CL_HAND, tRecipe.first))
				m_vecRender.push_back(tRecipe.first);
		}
	}

	if (m_bCraftLevel[CL_FURNACE])
	{
		for (auto& tRecipe : m_mapRecipeLV1)
		{
			if (Check_Recipe(CL_FURNACE, tRecipe.first))
				m_vecRender.push_back(tRecipe.first);
		}
	}

	if (m_bCraftLevel[CL_ANVIL])
	{
		for (auto& tRecipe : m_mapRecipeLV2)
		{
			if (Check_Recipe(CL_ANVIL, tRecipe.first))
				m_vecRender.push_back(tRecipe.first);
		}
	}

	Key_Input();
}

void CCraft::Render(HDC hDC)
{
	if ((!g_bESC) || (m_vecRender.empty()))
		return;

	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_16);
	SetTextColor(hDC, RGB(255, 255, 0));
	SetTextAlign(hDC, TA_CENTER);

	for (auto& pTile : m_vecRawSlot)
	{
		pTile->Render_Tile(hDC, 0.f, 0.f, 0.6f);
	}

	for (int i = 0; i < 7; i++)
	{
		if ((m_iSelectIndex + i - 3 < 0) || (m_iSelectIndex + i - 3 >= m_vecRender.size()))
			continue;

		ITEMINFO	tempItemInfo = m_vecRender[m_iSelectIndex + i - 3];

		if (i == 3)
		{
			static_cast<CTileSlot*>(m_vecProductSlot[i])->Render_Picked(hDC, 0.f, 0.f);

			CItem::Render_Slot(hDC, tempItemInfo, m_vecProductSlot[i]->Get_Info()->fX, m_vecProductSlot[i]->Get_Info()->fY);

			size_t iTemp0 = m_mapRecipeLV0[tempItemInfo].size();
			size_t iTemp1 = m_mapRecipeLV1[tempItemInfo].size();
			size_t iTemp2 = m_mapRecipeLV2[tempItemInfo].size();
			
			for (int i = 0; i < iTemp0; ++i)
			{
				CItem::Render_Slot(hDC, m_mapRecipeLV0[tempItemInfo][i], 82.f + 32.f * i, 554.f);
			}
			for (int i = 0; i < iTemp1; ++i)
			{
				CItem::Render_Slot(hDC, m_mapRecipeLV1[tempItemInfo][i], 82.f + 32.f * i, 554.f);
			}
			for (int i = 0; i < iTemp2; ++i)
			{
				CItem::Render_Slot(hDC, m_mapRecipeLV2[tempItemInfo][i], 82.f + 32.f * i, 554.f);
			}
		}
		else
		{
			m_vecProductSlot[i]->Render_Tile(hDC, 0.f, 0.f, 1.0f - 0.1f * abs(3 - i));
			CItem::Render_Slot(hDC, tempItemInfo, m_vecProductSlot[i]->Get_Info()->fX, m_vecProductSlot[i]->Get_Info()->fY);
		}
	}

	// Craft Icon
	GdiTransparentBlt(hDC,
		66,
		574,
		30,
		30,
		CUIMgr::m_hCraftIconDC,
		0,
		0,
		30,
		30,
		RGB(255, 255, 255));

	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CCraft::Release()
{
	for (int i = 0; i < 7; ++i)
	{
		Safe_Delete<CTile*>(m_vecProductSlot[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		Safe_Delete<CTile*>(m_vecRawSlot[i]);
	}
	m_vecProductSlot.clear();
	m_vecRawSlot.clear();
}

void CCraft::Set_Data(int _iData)
{
	size_t len = m_vecRender.size();

	if (_iData == 1)
	{
		m_iSelectIndex++;
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Menu_Tick.wav", SOUND_EFFECT6, g_fVolume, 1, 100);

		if (m_iSelectIndex >= len)
			m_iSelectIndex = len - 1;
	}
	else if (_iData == -1)
	{
		m_iSelectIndex--;
		CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Menu_Tick.wav", SOUND_EFFECT6, g_fVolume, 1, 100);

		if (m_iSelectIndex < 0)
			m_iSelectIndex = 0;
	}
	else if (_iData == 4)
	{
		m_bCraftLevel[CL_FURNACE] = true;
	}
	else if (_iData == 6)
	{
		m_bCraftLevel[CL_ANVIL] = true;
	}
	else if (_iData == -4)
	{
		m_bCraftLevel[CL_FURNACE] = false;
	}
	else if (_iData == -6)
	{
		m_bCraftLevel[CL_ANVIL] = false;
	}
}

void CCraft::Load_Recipe()
{
	HANDLE	hFile = CreateFile(L"../Data/Recipe.dat",	// 파일이름이 포함된 경로
		GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,					// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈하고자 할 때 허가할 것인가)
		NULL,					// 보안 모드 설정
		OPEN_EXISTING,			// 생성 방식 CREATE_ALWAYS(쓰기 전용), OPEN_EXISTING(파일이 있는 경우만 열기)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성(읽기 전용, 숨김 파일 등등), 아무런 속성이 없는 일반 파일ㄹ
		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Recipe File Open Failed", _T("FAIL"), MB_OK);
		return;
	}

	DWORD		dwByte(0);

	while (true)
	{
		int			iLevel(0);
		ITEMINFO	tProductInfo;
		int			RawCount(0);
		vector<ITEMINFO> vecRaw;

		ReadFile(hFile, &iLevel, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &tProductInfo, sizeof(ITEMINFO), &dwByte, nullptr);
		ReadFile(hFile, &RawCount, sizeof(int), &dwByte, nullptr);

		for (int i = 0; i < RawCount; ++i)
		{
			ITEMINFO	tRawInfo;
			ReadFile(hFile, &tRawInfo, sizeof(ITEMINFO), &dwByte, nullptr);
			vecRaw.push_back(tRawInfo);
		}

		if (0 == dwByte)
			break;

		switch (iLevel)
		{
		case 0:
			m_mapRecipeLV0.emplace(tProductInfo, vecRaw);
			break;

		case 1:
			m_mapRecipeLV1.emplace(tProductInfo, vecRaw);
			break;

		case 2:
			m_mapRecipeLV2.emplace(tProductInfo, vecRaw);
			break;
		}
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"Recipe Load Success", _T("SUCCESS"), MB_OK);
}

//Recipe.dat 파일 생성용
//void CCraft::Save_Recipe_Simple()
//{
//	RECIPE_DATA recipes[] = {
//		// LV0
//		{0, {0,0,10,2}, 1, {{0,0,5,1}}},
//		{0, {1,0,4,1}, 2, {{0,0,5,5}, {0,0,2,5}}},
//		{0, {2,0,2,1}, 1, {{2,0,1,100}}},
//		{0, {2,0,3,1}, 1, {{2,0,2,100}}},
//		{0, {2,0,4,1}, 1, {{2,0,3,100}}},
//		{0, {2,0,1,100}, 1, {{2,0,2,1}}},
//		{0, {2,0,2,100}, 1, {{2,0,3,1}}},
//		{0, {2,0,3,100}, 1, {{2,0,4,1}}},
//		// LV1
//		{1, {1,0,1,1}, 1, {{0,0,5,3}}},
//		{1, {1,0,2,1}, 1, {{0,0,5,3}}},
//		{1, {1,0,3,1}, 1, {{0,0,5,3}}},
//		{1, {1,0,5,1}, 1, {{0,0,5,3}}},
//		{1, {1,0,6,1}, 2, {{0,0,5,3}, {0,0,2,5}}},
//		{1, {0,1,1,4}, 1, {{0,0,5,1}}},
//		{ 1, { 4,4,1,1 }, 3, { {0,0,5,3}, {0,0,2,3}, {4,4,0,1} } },
//		{1, {2,0,5,1}, 1, {{0,0,3,3}}},
//		// LV2
//		{2, {5,1,1,1}, 2, {{2,0,5,3}, {0,0,5,1}}},
//		{2, {5,2,1,1}, 2, {{2,0,5,3}, {0,0,5,1}}},
//		{2, {5,3,1,1}, 2, {{2,0,5,3}, {0,0,5,1}}}
//	};
//
//	HANDLE hFile = CreateFile(L"../Data/Recipe.dat",
//		GENERIC_WRITE,
//		NULL,
//		NULL,
//		CREATE_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		MessageBox(g_hWnd, L"Recipe File Create Failed", _T("FAIL"), MB_OK);
//		return;
//	}
//
//	DWORD dwByte(0);
//	int recipeCount = sizeof(recipes) / sizeof(RECIPE_DATA);
//
//	for (int i = 0; i < recipeCount; ++i)
//	{
//		WriteFile(hFile, &recipes[i].level, sizeof(int), &dwByte, nullptr);
//		WriteFile(hFile, &recipes[i].product, sizeof(ITEMINFO), &dwByte, nullptr);
//		WriteFile(hFile, &recipes[i].rawCount, sizeof(int), &dwByte, nullptr);
//
//		for (int j = 0; j < recipes[i].rawCount; ++j)
//		{
//			WriteFile(hFile, &recipes[i].raws[j], sizeof(ITEMINFO), &dwByte, nullptr);
//		}
//	}
//
//	CloseHandle(hFile);
//	MessageBox(g_hWnd, L"Recipe Save Success", _T("SUCCESS"), MB_OK);
//}

bool CCraft::Check_Recipe(CRAFTLEVEL eLevel, ITEMINFO tProductInfo)
{
	CInven_Bag* tempInven = static_cast<CInven_Bag*>(m_pInven_Bag);

	switch (eLevel)
	{
	case CL_HAND:
	{
		for (auto& tRawInfo : m_mapRecipeLV0[tProductInfo])
		{
			if (!tempInven->Check_Item(tRawInfo))
				return false;
		}
	}
	break;

	case CL_FURNACE:
	{
		for (auto& pRawInfo : m_mapRecipeLV1[tProductInfo])
		{
			if (!tempInven->Check_Item(pRawInfo))
				return false;
		}
	}
	break;

	case CL_ANVIL:
	{
		for (auto& pRawInfo : m_mapRecipeLV2[tProductInfo])
		{
			if (!tempInven->Check_Item(pRawInfo))
				return false;
		}
	}
	break;
	}

	return true;
}

void CCraft::Key_Input()
{
	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (m_iSelectIndex < 0 || m_iSelectIndex >= m_vecRender.size())
			return;

		ITEMINFO tempInfo = m_vecRender[m_iSelectIndex];
		Craft_Item(tempInfo);
	}
}

void CCraft::Craft_Item(ITEMINFO tProductInfo)
{
	size_t iTemp0 = m_mapRecipeLV0[tProductInfo].size();
	size_t iTemp1 = m_mapRecipeLV1[tProductInfo].size();
	size_t iTemp2 = m_mapRecipeLV2[tProductInfo].size();
	size_t len;
	CRAFTLEVEL tempLV;
	vector<ITEMINFO>* tempVec;

	if (iTemp0)
	{
		tempLV = CL_HAND;
		len = iTemp0;
		tempVec = &m_mapRecipeLV0[tProductInfo];
	}
	else if (iTemp1)
	{
		tempLV = CL_FURNACE;
		len = iTemp1;
		tempVec = &m_mapRecipeLV1[tProductInfo];
	}
	else
	{
		tempLV = CL_ANVIL;
		len = iTemp2;
		tempVec = &m_mapRecipeLV2[tProductInfo];
	}

	if (!Check_Recipe(tempLV, tProductInfo))
		return;

	CInven_Bag* tempInven = static_cast<CInven_Bag*>(CUIMgr::Get_Instance()->Get_UI(UIS_INVENBAG));

	CItem* pProduct = nullptr;

	switch (tProductInfo.eItemID)
	{
	case IT_BLOCK:
		pProduct = new CItem_Block(tProductInfo);
		break;

	case IT_FURNITURE:
		pProduct = new CItem_Furniture(tProductInfo);
		break;

	case IT_NONCONSUMABLE:
		pProduct = new CItem_Nonconsumable(tProductInfo);
		break;

	case IT_CONSUMABLE:
		pProduct = new CItem_Consumable(tProductInfo);
		break;

	case IT_WEAPON:
		pProduct = new CItem_Weapon(tProductInfo);
		break;

	case IT_ARMOR:
		pProduct = new CItem_Armor(tProductInfo);
		break;

	default:
		return;
	}


	if (pProduct)
	{
		pProduct->Initialize();

		// Gain_Item은 아이템을 복사해서 인벤토리에 넘겨주므로, 이후 원본의 삭제 필요 
		tempInven->Gain_Item(pProduct);

		Safe_Delete(pProduct);
	}

	for (size_t i = 0; i < len; i++)
	{
		tempInven->Remove_Item((*tempVec)[i]);
	}
}
