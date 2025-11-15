#pragma once
#include "CItem.h"

class CItem_Weapon : public CItem
{
public:
	enum WEAPON_ID { MELEE, RANGE, MAGIC, SUMMON, TOOL, WI_END };	// SubID

	enum WEAPON_TYPE { SWING, STAB, HOLD, WT_END };

	typedef struct tagWeaponStat
	{
		WEAPON_TYPE	eWeaponType;
		float		fCoolTime;
		float		fRange;
		float		fDamage;
		float		fKnockBack;

		tagWeaponStat() { ZeroMemory(this, sizeof(WPSTAT)); }
		tagWeaponStat(WEAPON_TYPE _eType, float _fCoolTime, float _fRange, float _fDamage, float _fKnockBack)
			: eWeaponType(_eType), fCoolTime(_fCoolTime), fRange(_fRange), fDamage(_fDamage), fKnockBack(_fKnockBack) {}
	}WPSTAT;

public:
	CItem_Weapon();
	CItem_Weapon(ITEMINFO tItemInfo);
	virtual ~CItem_Weapon();

	void	Late_Update()	override {};
	void	Render(HDC hDC)	override;

	void	Render_Hand(HDC hDC, float _fX, float _fY, float fHandAngle) override;
	void	Use_Item(CObj* pPlayer)		override;
	//void	OnCollision(COLMSG _colMSG) override;

	void	Set_WeaponStat(WPSTAT _tStat) { m_tWeaponStat = _tStat; }
	const WPSTAT*	Get_WeaponStat() { return &m_tWeaponStat; }

	void Use_SwingWeapon(CObj* pPlayer);
	void Use_StabWeapon(CObj* pPlayer);
	void Use_HoldWeapon(CObj* pPlayer);

	void Use_Pickaxe(int iIndex, POINT pt);
	void Use_Axe(int iIndex, POINT pt);
	void Use_Hammer(int iIndex, POINT pt);

private:
	POINT	m_tPosin;
	WPSTAT	m_tWeaponStat;
};