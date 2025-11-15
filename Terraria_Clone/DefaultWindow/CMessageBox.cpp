#include "pch.h"
#include "CMessageBox.h"
#include "CUIMgr.h"
#include "CTextMgr.h"
#include "CObjMgr.h"

bool g_bInteraction = false;

CMessageBox::CMessageBox()
	:	m_eNPCID(CNPC::NPC_END),
		m_iMessageCount(0)
{
	ZeroMemory(&m_tTextRect, sizeof(RECT));
	ZeroMemory(m_iMessageEnd, sizeof(m_iMessageEnd));
	ZeroMemory(m_bAlreadyChatted, sizeof(m_bAlreadyChatted));
}

CMessageBox::~CMessageBox()
{
}

void CMessageBox::Initialize()
{
	m_tInfo = { float(WINCX >> 1), 225.f, 400.f, 200.f };
	__super::Update_Rect();
	m_tTextRect = { m_tRect.left + 6, m_tRect.top + 24, m_tRect.right - 6, m_tRect.bottom };

	m_vecText[CNPC::NPC_HINTBOX1].push_back(L"\n블럭은 곡괭이로, 나무는 도끼로, 벽지는 망치로\n\n부술 수 있습니다. 마음껏 캐보세요 !");
	m_iMessageEnd[CNPC::NPC_HINTBOX1] = 1;

	m_vecText[CNPC::NPC_HINTBOX2].push_back(L"\n몬스터는 여러분을 싫어합니다.\n\n총으로 자신을 지키세요!");
	m_iMessageEnd[CNPC::NPC_HINTBOX2] = 1;

	m_vecText[CNPC::NPC_HINTBOX3].push_back(L"\n꿀 블록은 끈적합니다.\n\n얼음 블록은 미끄럽습니다.\n\n구름 블록은 푹신합니다.");
	m_iMessageEnd[CNPC::NPC_HINTBOX3] = 1;

	m_vecText[CNPC::NPC_HINTBOX4].push_back(L"\n포션은 현재 체력을 채워줍니다.\n\n하트수정은 최대 체력을 올려줍니다.");
	m_vecText[CNPC::NPC_HINTBOX4].push_back(L"\n중력 포션은 우클릭으로 중력을 바꿀 수 있어요.\n\n다이너마이트는... 한 번 써보세요 !");
	m_iMessageEnd[CNPC::NPC_HINTBOX4] = 2;

	m_vecText[CNPC::NPC_HINTBOX5].push_back(L"\nESC를 눌러 인벤토리와 제작창을 확인해보세요.\n\nC키로 현재 선택한 아이템을 제작하세요.");
	m_iMessageEnd[CNPC::NPC_HINTBOX5] = 1;

	m_vecText[CNPC::NPC_GUIDE].push_back(L"\n테라리아 세계에 온 걸 환영해!\n\n어라,,, 몰골이 왜그래?\n\n어디서 추락한 사람 같아.");
	m_vecText[CNPC::NPC_GUIDE].push_back(L"\n저런,,, 여기서 우측으로 가면 힌트 박스가 있어.\n\n도움이 될거야.");
	m_vecText[CNPC::NPC_GUIDE].push_back(L"\n이동은 A/D키, 점프는 SPACE,\n\n아이템퀵슬롯 전환은 1~0,\n\n아이템 사용은 좌클릭이야. 행운을 빌어 !");
	m_iMessageEnd[CNPC::NPC_GUIDE] = 3;

	m_vecText[CNPC::NPC_MERCHANT].push_back(L"\n천국의 가이드가 살해당했다는군.\n\n젠장, 고객이 한 명 줄었잖아 !");
	m_iMessageEnd[CNPC::NPC_MERCHANT] = 1;

	m_vecText[CNPC::NPC_NURSE].push_back(L"\n가이드의 죽음은 정말 슬픈 일이에요.\n\n범인은 폭탄을 썼다더라구요.");
	m_iMessageEnd[CNPC::NPC_NURSE] = 1;
}

int CMessageBox::Update()
{
	if (!g_bInteraction)
		return OBJ_NOEVENT;

	if (m_iMessageEnd[m_eNPCID] - 1 == m_iMessageCount)
	{
		if (!m_bAlreadyChatted[m_eNPCID])
		{
			m_bAlreadyChatted[m_eNPCID] = true;
			CUIMgr::Get_Instance()->Get_UI(UIS_QUESTBOX)->Set_Data(-1);
		}
	}
	else if (m_iMessageEnd[m_eNPCID] == m_iMessageCount)
	{
		g_bInteraction = false;
	}

	return OBJ_NOEVENT;
}

void CMessageBox::Late_Update()
{
	if (!g_bInteraction)
		return;


}

void CMessageBox::Render(HDC hDC)
{
	if (!g_bInteraction)
		return;

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CUIMgr::m_hMessageBoxDC,
		0,
		0,
		(int)m_tInfo.fCX,   // 복사할 이미지 가로
		(int)m_tInfo.fCY,   // 복사할 이미지 세로
		RGB(255, 0, 255));    // 제거할 픽셀 색상 값

	if (m_eNPCID <= CNPC::NPC_HINTBOX5)
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top - 22,
			44,
			44,
			CObjMgr::m_hNPCDC[m_eNPCID],
			0,
			0,
			44,   // 복사할 이미지 가로
			44,   // 복사할 이미지 세로
			RGB(255, 0, 255));    // 제거할 픽셀 색상 값
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top - 27,
			40,
			54,
			CObjMgr::m_hNPCDC[m_eNPCID],
			0,
			0,
			40,   // 복사할 이미지 가로
			54,   // 복사할 이미지 세로
			RGB(255, 0, 255));    // 제거할 픽셀 색상 값
	}

	CTextMgr::Get_Instance()->Set_Font(hDC, CTextMgr::FONT_24);
	SetTextColor(hDC, RGB(255, 255, 255));	// 폰트 색깔
	SetTextAlign(hDC, TA_LEFT);				// 폰트 정렬 기준

	wchar_t wStr[64];
	swprintf(wStr, 64, m_vecText[m_eNPCID][m_iMessageCount]);
	DrawText(hDC, wStr, -1, &m_tTextRect, DT_LEFT | DT_WORDBREAK);

	CTextMgr::Get_Instance()->Unset_Font(hDC);
}

void CMessageBox::Release()
{
}

void CMessageBox::Set_Data(int _iData)
{

	if (_iData == -1)
	{
		m_iMessageCount++;
	}
	else if (_iData >= 0)
	{
		m_eNPCID = CNPC::NPCID(_iData / 10);
		m_iMessageCount = _iData % 10;
	}
	else if (_iData == -2)
	{
		m_vecText[CNPC::NPC_GUIDE].clear();
		m_vecText[CNPC::NPC_GUIDE].push_back(L"\n테라리아 세계에,, 내가 왜 여기있지?\n\n어라,,, 몰골이 왜그래?\n\n누구한테 두들겨 맞은 것 같아.");
		m_iMessageEnd[CNPC::NPC_GUIDE] = 1;
	}
}