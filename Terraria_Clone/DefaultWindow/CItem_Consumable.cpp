#include "pch.h"
#include "CItem_Consumable.h"
#include "CItemMgr.h"
#include "CScrollMgr.h"
#include "CUIMgr.h"
#include "CPlayer.h"
#include "CHPBar.h"
#include "CObjMgr.h"
#include "CDynamite.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"

CItem_Consumable::CItem_Consumable()
	: m_fCSCoolTime(500.f)
{
}

CItem_Consumable::CItem_Consumable(ITEMINFO tItemInfo)
	:	CItem(tItemInfo),
		m_fCSCoolTime(500.f)
{
}

CItem_Consumable::~CItem_Consumable()
{
}

void CItem_Consumable::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		CItemMgr::m_vecItemDC[m_eItemID][m_iItemSubID * 10 + m_iItemCode],
		0,
		0,
		(int)m_tInfo.fCX,		// 복사할 이미지 가로
		(int)m_tInfo.fCY,		// 복사할 이미지 세로
		RGB(255, 255, 255));	// 제거할 픽셀 색상 값
}

void CItem_Consumable::Use_Item(CObj* pPlayer)
{
	DWORD tempTime = GetTickCount();

	if (m_fTime[TM_USE] + m_fCSCoolTime < tempTime)
	{
		CPlayer* tempPlayer = static_cast<CPlayer*>(pPlayer);

		switch (m_iItemCode)
		{
		case 1:	// HP Potion
		{
			if (tempPlayer->Get_HP() == tempPlayer->Get_MaxHP())
			{
				return;
			}
			else if (tempPlayer->Get_HP() >= tempPlayer->Get_MaxHP() - 50)
			{
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Potion.wav", SOUND_EFFECT5, g_fVolume, 1, m_fCSCoolTime);
				tempPlayer->Set_HP(tempPlayer->Get_MaxHP());
			}
			else
			{
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Potion.wav", SOUND_EFFECT5, g_fVolume, 1, m_fCSCoolTime);
				tempPlayer->Set_HP(tempPlayer->Get_HP() + 50);
			}
		}
		break;

		case 2:	// Heart Crystal
		{
			CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_HeartCrystal.wav", SOUND_EFFECT5, g_fVolume, 1, m_fCSCoolTime);
			tempPlayer->Set_MaxHP(tempPlayer->Get_MaxHP() + 20);
			CUIMgr::Get_Instance()->Get_UI(UIS_HPBAR)->Set_Data(-1);
		}
		break;

		case 3:	// Dynamite
		{
			CObjMgr::Get_Instance()->Add_Object(FURNITURE, CAbstractFactory<CDynamite>::Create_Obj(pPlayer->Get_Info()->fX, pPlayer->Get_Info()->fY, m_fAngle));
		}
		break;

		case 4:	// Gravity Potion
		{
			if (tempPlayer->Get_GravityState())
			{
				return;
			}
			else
			{
				CSoundMgr::Get_Instance()->PlaySoundQuick(L"Sound_Potion.wav", SOUND_EFFECT5, g_fVolume, 1, m_fCSCoolTime);
				tempPlayer->Set_GravityState();
			}
		}
		break;
		}

		m_iCount--;
		m_fTime[TM_USE] = tempTime;
	}
}