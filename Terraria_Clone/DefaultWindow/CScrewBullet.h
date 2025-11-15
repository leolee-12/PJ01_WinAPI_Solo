#pragma once
#include "CObj.h"
class CScrewBullet :
    public CObj
{
public:
    CScrewBullet();
    virtual ~CScrewBullet();
    
public:
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    bool    m_bStart;

    POINT   m_tCenter;

    float   m_fRotAngle;
    float   m_fRotSpeed;

};

