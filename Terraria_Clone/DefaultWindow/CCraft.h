#pragma once
#include "CUI.h"
#include "CTileSlot.h"

class CCraft : public CUI
{
public:
	enum CRAFTLEVEL { CL_HAND, CL_FURNACE, CL_ANVIL, CL_END };

	//struct RECIPE_DATA
	//{
	//	int level;
	//	ITEMINFO product;
	//	int rawCount;
	//	ITEMINFO raws[3];  // 최대 3개로 가정
	//};

public:
	CCraft();
	virtual ~CCraft();

	void	Initialize()			override;
	int		Update()				override;
	void	Late_Update()			override;
	void	Render(HDC hDC)			override;
	void	Release()				override;
	void	Set_Data(int _iData)	override;

	void	Load_Recipe();
	//void	Save_Recipe_Simple();

	bool	Check_Recipe(CRAFTLEVEL eLevel, ITEMINFO tItemInfo);
	void	Set_Level(CRAFTLEVEL eLV) { m_bCraftLevel[eLV] = true; }
	bool	Get_Level(CRAFTLEVEL eLV) { return m_bCraftLevel[eLV]; }
	void	Key_Input();
	void	Craft_Item(ITEMINFO pProductInfo);

private:
	CUI*	m_pInven_Bag;
	bool	m_bCraftLevel[CL_END];
	int		m_iSelectIndex;

	map<ITEMINFO, vector<ITEMINFO>>	m_mapRecipeLV0;
	map<ITEMINFO, vector<ITEMINFO>>	m_mapRecipeLV1;
	map<ITEMINFO, vector<ITEMINFO>>	m_mapRecipeLV2;

	vector<CTile*>		m_vecRawSlot;
	vector<CTile*>		m_vecProductSlot;
	vector<ITEMINFO>	m_vecRender;
	ITEMINFO			m_tRawInfo[3];
	ITEMINFO			m_tProductInfo;
};