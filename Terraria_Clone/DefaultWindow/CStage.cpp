#include "pch.h"
#include "CStage.h"
#include "CAbstractFactory.h"
#include "CMonster.h"
#include "CMouse.h"
#include "CHPBar.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CScrollMgr.h"
#include "CKeyMgr.h"
#include "CBmpMgr.h"
#include "CSceneMgr.h"
#include "CPlayer.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
#include "CInven_Bag.h"
#include "CSoundMgr.h"
#include "CItemMgr.h"
#include "CMonster_N1.h"
#include "CMonster_Boss.h"
#include "CBossHPBar.h"
#include "CItem_Block.h"
#include "CItem_Furniture.h"
#include "CItem_Nonconsumable.h"
#include "CItem_Consumable.h"
#include "CItem_Weapon.h"
#include "CItem_Armor.h"
#include "CText.h"
#include "CCraft.h"
#include "CEffectMgr.h"
#include "CNPC_Guide.h"
#include "CNPC_HintBox.h"
#include "CTextMgr.h"
#include "CMessageBox.h"
#include "CQuestBox.h"
#include "CNPC_Merchant.h"
#include "CNPC_Nurse.h"
#include "CNPCButton.h"
#include "CInven_Shop.h"

CStage::CStage()
	:	m_bBossSpawn(false),
		m_bBossKilled(false),
		m_bCleared(false),
		m_bEnding(false)
{
	ZeroMemory(m_hBGDC, sizeof(m_hBGDC));
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CTileMgr::Get_Instance()->Load_Tile();

	CItemMgr::Get_Instance();
	CEffectMgr::Get_Instance()->Initialize();
	CObjMgr::Get_Instance()->Add_Object(PLAYER, CAbstractFactory<CPlayer>::Create_Obj());
	static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front())->Set_TargetMouse(m_pMouse);

	CUIMgr::Get_Instance()->Initialize();
	CUIMgr::Get_Instance()->Add_UI(UIS_HPBAR, CAbstractFactory<CHPBar>::Create_UI());
	CUIMgr::Get_Instance()->Add_UI(UIS_INVENBAG, CAbstractFactory<CInven_Bag>::Create_UI());
	CUIMgr::Get_Instance()->Add_UI(UIS_INVENSHOP, CAbstractFactory<CInven_Shop>::Create_UI());
	CUIMgr::Get_Instance()->Add_UI(UIS_CRAFT, CAbstractFactory<CCraft>::Create_UI());
	//static_cast<CCraft*>(CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT))->Save_Recipe_Simple();
	static_cast<CCraft*>(CUIMgr::Get_Instance()->Get_UI(UIS_CRAFT))->Load_Recipe();
	CUIMgr::Get_Instance()->Add_UI(UIS_MESSAGEBOX, CAbstractFactory<CMessageBox>::Create_UI());
	CUIMgr::Get_Instance()->Add_UI(UIS_QUESTBOX, CAbstractFactory<CQuestBox>::Create_UI());
	CUIMgr::Get_Instance()->Add_UI(UIS_NPCBUTTON, CAbstractFactory<CNPCButton>::Create_UI());

	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(663.f, 447.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(4);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(784.f, 455.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(6);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(424.f, 392.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(7);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(1210.f, 540.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(8);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(314.f, 1352.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(9);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(458.f, 1364.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(10);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(1722.f, 1352.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(9);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(1962.f, 1360.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(10);
	CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CFurniture>::Create_Obj(1914.f, 528.f));
	static_cast<CFurniture*>(CObjMgr::Get_Instance()->Get_ObjList(FURNITURE)->back())->Set_FurnitureInfo(10);

	CItemMgr::Get_Instance()->Add_Item(IT_CONSUMABLE, CAbstractFactory<CItem_Consumable>::Create_Item(1800.f, 0.f, ITEMINFO(IT_CONSUMABLE, 0, 1, 50)));
	CItemMgr::Get_Instance()->Add_Item(IT_CONSUMABLE, CAbstractFactory<CItem_Consumable>::Create_Item(1800.f, 0.f, ITEMINFO(IT_CONSUMABLE, 0, 2, 50)));
	CItemMgr::Get_Instance()->Add_Item(IT_CONSUMABLE, CAbstractFactory<CItem_Consumable>::Create_Item(1800.f, 0.f, ITEMINFO(IT_CONSUMABLE, 0, 3, 50)));
	CItemMgr::Get_Instance()->Add_Item(IT_CONSUMABLE, CAbstractFactory<CItem_Consumable>::Create_Item(1800.f, 0.f, ITEMINFO(IT_CONSUMABLE, 0, 4, 50)));
	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Guide>::Create_Obj(750.f, 400.f));

	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_HintBox>::Create_Obj(800.f, 300.f));
	static_cast<CNPC*>(CObjMgr::Get_Instance()->Get_ObjList(NPC)->back())->Set_NPCID(CNPC::NPC_HINTBOX1);

	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_HintBox>::Create_Obj(1100.f, 500.f));
	static_cast<CNPC*>(CObjMgr::Get_Instance()->Get_ObjList(NPC)->back())->Set_NPCID(CNPC::NPC_HINTBOX2);

	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_HintBox>::Create_Obj(1300.f, 550.f));
	static_cast<CNPC*>(CObjMgr::Get_Instance()->Get_ObjList(NPC)->back())->Set_NPCID(CNPC::NPC_HINTBOX3);

	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_HintBox>::Create_Obj(1800.f, 550.f));
	static_cast<CNPC*>(CObjMgr::Get_Instance()->Get_ObjList(NPC)->back())->Set_NPCID(CNPC::NPC_HINTBOX4);

	CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_HintBox>::Create_Obj(200.f, 1350.f));
	static_cast<CNPC*>(CObjMgr::Get_Instance()->Get_ObjList(NPC)->back())->Set_NPCID(CNPC::NPC_HINTBOX5);

	//CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Merchant>::Create_Obj(400.f, 1200.f));
	//CObjMgr::Get_Instance()->Add_Object(NPC, CAbstractFactory<CNPC_Nurse>::Create_Obj(600.f, 1200.f));

	for (int i = 0; i < 3; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(MONSTER, CAbstractFactory<CMonster_N1>::Create_Obj((1400.f + 300.f * i), 400.f));
	}

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Stage0.bmp", L"BG_Stage0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu2.bmp", L"BG_Stage1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu4.bmp", L"BG_Stage2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Background/BG_Menu3.bmp", L"BG_Stage3");

	m_hBGDC[0] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Stage0");
	m_hBGDC[1] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Stage1");
	m_hBGDC[2] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Stage2");
	m_hBGDC[3] = CBmpMgr::Get_Instance()->Find_Image(L"BG_Stage3");

	CSoundMgr::Get_Instance()->Initialize();

	CSoundMgr::Get_Instance()->PlayBGM(L"Sound_Stage.wav", g_fVolume);
}

void CStage::Update()
{
	Trigger_Check();

	CObjMgr::Get_Instance()->Update();
	CTileMgr::Get_Instance()->Update();
	CItemMgr::Get_Instance()->Update();
	CEffectMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
}

void CStage::Late_Update()
{
	CObjMgr::Get_Instance()->Late_Update();	
	CTileMgr::Get_Instance()->Late_Update();
	CItemMgr::Get_Instance()->Late_Update();
	CEffectMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CStage::Render(HDC hDC)
{
	if (!m_bEnding)
	{
		int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

		Render_Background(hDC, iScrollX, iScrollY);

		CTileMgr::Get_Instance()->Render(hDC);
		CObjMgr::Get_Instance()->Render(hDC);
		CItemMgr::Get_Instance()->Render(hDC);
		CEffectMgr::Get_Instance()->Render(hDC);
		CUIMgr::Get_Instance()->Render(hDC);
	}
	else
	{
		GdiTransparentBlt(hDC,
			(WINCX >> 1),
			(WINCY >> 1),
			400,
			200,
			CUIMgr::m_hErrorMessageDC,
			0,
			0,
			400,
			200,
			RGB(255, 255, 255));
	}
}

void CStage::Release()
{
	CSoundMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release();
	CItemMgr::Get_Instance()->Release();
	CUIMgr::Get_Instance()->Release();
}

void CStage::Render_Background(HDC hDC, int iScrollX, int iScrollY)
{
	BitBlt(hDC, iScrollX, iScrollY, 7200, 4000, m_hBGDC[0], 0, 0, SRCCOPY);

	GdiTransparentBlt(hDC,
		iScrollX,
		MAPCY - 630 + iScrollY,
		WINCX,
		435,
		m_hBGDC[1],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		WINCX + iScrollX,
		MAPCY - 630 + iScrollY,
		WINCX,
		435,
		m_hBGDC[1],
		0,
		0,
		WINCX,
		435,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	GdiTransparentBlt(hDC,
		iScrollX,
		MAPCY - 520 + iScrollY,
		WINCX,
		509,
		m_hBGDC[2],
		0,
		0,
		WINCX,
		509,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		WINCX + iScrollX,
		MAPCY - 520 + iScrollY,
		WINCX,
		509,
		m_hBGDC[2],
		0,
		0,
		WINCX,
		509,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값

	GdiTransparentBlt(hDC,
		iScrollX,
		50 + iScrollY,
		2 * WINCX,
		434,
		m_hBGDC[3],
		0,
		0,
		2 * WINCX,
		434,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
	GdiTransparentBlt(hDC,
		2 * WINCX + iScrollX,
		50 + iScrollY,
		2 * WINCX,
		434,
		m_hBGDC[3],
		0,
		0,
		2 * WINCX,
		434,
		RGB(255, 0, 255));			// 제거할 픽셀 색상 값
}

void CStage::Trigger_Check()
{
	if ((!m_bBossSpawn) && (CQuestBox::m_eCurQuest == CQuestBox::Q_BOSS) &&
		(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front()->Get_Info()->fX >= 1300.f) &&
		(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front()->Get_Info()->fY >= 1000.f))
	{
		m_bBossSpawn = true;

		CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
		CSoundMgr::Get_Instance()->PlaySound(L"Sound_BossSpawn.wav", SOUND_EFFECT4, g_fVolume);
		CSoundMgr::Get_Instance()->PlayBGM(L"Sound_BossBGM.wav", g_fVolume);

		CObjMgr::Get_Instance()->Add_Object(BOSS, CAbstractFactory<CMonster_Boss>::Create_Obj(1900.f, 1000.f));
		CObjMgr::Get_Instance()->Delete_ID(NPC);
		CUIMgr::Get_Instance()->Add_UI(UIS_BOSSHPBAR, CAbstractFactory<CBossHPBar>::Create_UI());

		if (!CUIMgr::Get_Instance()->Get_TextList()->empty())
		{
			for (auto p : *CUIMgr::Get_Instance()->Get_TextList())
			{
				p->Add_PosY(-28.f);
			}
		}
		CUIMgr::Get_Instance()->Add_UI(UIS_TEXT, CAbstractFactory<CText>::Create_UI());
		CUIMgr::Get_Instance()->Get_TextList()->back()->Set_Data(CText::BOSSSPAWN);
	}

	if (m_bBossSpawn)
	{
		if (CObjMgr::Get_Instance()->Get_ObjList(BOSS)->empty())
		{
			m_bBossKilled = true;

			if (m_bBossKilled && !m_bCleared)
			{
				CUIMgr::Get_Instance()->Get_UI(UIS_QUESTBOX)->Set_Data(-1);
				CUIMgr::Get_Instance()->Get_UI(UIS_QUESTBOX)->Set_Data(0);
				CUIMgr::Get_Instance()->Get_UI(UIS_MESSAGEBOX)->Set_Data(-2);
				CUIMgr::Get_Instance()->Delete_UI(UIS_BOSSHPBAR);
				CSoundMgr::Get_Instance()->StopSound(SOUND_BGM);
				CSoundMgr::Get_Instance()->PlayBGM(L"Sound_Stage.wav", g_fVolume);

				if (!CUIMgr::Get_Instance()->Get_TextList()->empty())
				{
					for (auto p : *CUIMgr::Get_Instance()->Get_TextList())
					{
						p->Add_PosY(-28.f);
					}
				}
				CUIMgr::Get_Instance()->Add_UI(UIS_TEXT, CAbstractFactory<CText>::Create_UI());
				CUIMgr::Get_Instance()->Get_TextList()->back()->Set_Data(CText::BOSSKILL);
				m_bCleared = true;
			}
		}
	}

	if ((!m_bEnding) && (CQuestBox::m_eCurQuest == CQuestBox::Q_EXIT) &&
		(CObjMgr::Get_Instance()->Get_ObjList(PLAYER)->front()->Get_Info()->fY >= 2000.f))
	{
		m_bEnding = true;
	}

	if ((m_bEnding) && (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN)))
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_MENU);
	}
}
