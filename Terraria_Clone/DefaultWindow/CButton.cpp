#include "pch.h"
#include "CButton.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"

CButton::CButton()
    : m_iDrawID(0)
{
}

CButton::~CButton()
{
    Release();
}

void CButton::Initialize()
{
    m_tInfo.fCX = 325.f;
    m_tInfo.fCY = 68.f;
}

int CButton::Update()
{
    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CButton::Late_Update()
{
    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    if (PtInRect(&m_tRect, pt))
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            if (!lstrcmp(L"Start", m_pFrameKey))
                CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_STAGE);

            else if(!lstrcmp(L"Edit", m_pFrameKey))
                CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SC_EDIT);

            else if (!lstrcmp(L"Exit", m_pFrameKey))
                DestroyWindow(g_hWnd);

            return;
        }

        m_iDrawID = 1;
    }
    else
        m_iDrawID = 0;

}

void CButton::Render(HDC hDC)
{
    HDC     hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

    GdiTransparentBlt(hDC,
        m_tRect.left,
        m_tRect.top,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        hMemDC,
        0, 
        (int)m_tInfo.fCY * m_iDrawID,
        (int)m_tInfo.fCX,   // 복사할 이미지 가로
        (int)m_tInfo.fCY,   // 복사할 이미지 세로
        RGB(255, 0, 255));    // 제거할 픽셀 색상 값
}

void CButton::Release()
{
}

void CButton::Set_Data(int iNum)
{
    switch (iNum)
    {
    case BUTTON_START:
        m_pFrameKey = L"START";
        break;

    case BUTTON_EDIT:
        m_pFrameKey = L"EDIT";
        break;

    case BUTTON_EXIT:
        m_pFrameKey = L"EXIT";
        break;
    }
}
