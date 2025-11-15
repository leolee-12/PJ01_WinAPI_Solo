#pragma once
#include "CItem.h"
class CItem_Furniture : public CItem
{
public:
	CItem_Furniture();
	CItem_Furniture(ITEMINFO tItemInfo);
	virtual ~CItem_Furniture();

	void Initialize()				override;
	void Render(HDC hDC)			override;
	void Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle) override;
	void Use_Item(CObj* pPlayer)	override;
	void Place_Furniture(int iIndex);

private:
	float		m_fFTCoolTime;
	int			m_iFTTileX;
	int			m_iFTTileY;
	INFO		m_tFTInfo;
};

