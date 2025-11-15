#include "pch.h"
#include "CTextMgr.h"

CTextMgr*	CTextMgr::m_pInstance = nullptr;

CTextMgr::CTextMgr()
	: m_hOldFont(nullptr)
{
	ZeroMemory(m_hFont, sizeof(m_hFont));
}

CTextMgr::~CTextMgr()
{
	Release();
}

void CTextMgr::Initialize()
{
	m_hFont[FONT_16] = CreateFont(	16, 0, 0, 0, FW_BOLD,
									FALSE, FALSE, FALSE, ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
									DEFAULT_PITCH | FF_SWISS, TEXT("메이플스토리 Bold"));

	m_hFont[FONT_24] = CreateFont(	24, 0, 0, 0, FW_BOLD,
									FALSE, FALSE, FALSE, ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
									DEFAULT_PITCH | FF_SWISS, TEXT("메이플스토리 Bold"));

	m_hFont[FONT_40] = CreateFont(	40, 0, 0, 0, FW_BOLD,
									FALSE, FALSE, FALSE, ANSI_CHARSET,
									OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
									DEFAULT_PITCH | FF_SWISS, TEXT("메이플스토리 Bold"));
}

void CTextMgr::Release()
{
	for (size_t i = 0; i < FONT_END; ++i)
	{
		DeleteObject(m_hFont[i]);
	}
}

void CTextMgr::Set_Font(HDC hDC, FONTID eID)
{
	SetBkMode(hDC, TRANSPARENT);

	m_hOldFont = (HFONT)SelectObject(hDC, m_hFont[eID]);
}

void CTextMgr::Unset_Font(HDC hDC)
{
	SelectObject(hDC, m_hOldFont);
}