#pragma once
#include "CObj.h"

class CFurniture : public CObj
{
public:
	enum FURNITUREID {	FT_NONE,
						FT_DOOR, FT_TABLE, FT_CHAIR,
						FT_FURNACE, FT_CHEST, FT_ANVIL,
						FT_WOOD1, FT_WOOD2, FT_WOOD3, FT_WOOD4,
						FT_END };

public:
	CFurniture();
	virtual ~CFurniture();
	
	void	Initialize()		override;
	int		Update()			override;
	void	Late_Update()		override;
	void	Render(HDC hDC)		override;
	void	Release()			override;

	void	OnCollision(COLMSG _colMsg) override;
	void	Set_Angle(float fFTOption) { m_iFTOption = FURNITUREID(fFTOption); }

	void		Set_FurnitureInfo(int iNum);
	static void Set_FurnitureInfo(int iNum, INFO& tInfo);

private:
	FURNITUREID	m_iFTOption;
	int			m_iHP;
};

