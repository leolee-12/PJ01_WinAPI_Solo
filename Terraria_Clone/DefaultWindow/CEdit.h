#pragma once
#include "CScene.h"

class CEdit :
    public CScene
{
public:
    CEdit();
    virtual ~CEdit();

public:
    void Initialize() override;
    void Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;



private:
    void    Key_Input();
    int     m_iCurBlock;
    int     m_iSetRange;
    int     m_iCurFrameStart;
    const TCHAR* m_pHintText;
    RECT    m_tHintRect;
};

