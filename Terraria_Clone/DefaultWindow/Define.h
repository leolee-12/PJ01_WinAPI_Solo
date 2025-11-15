#pragma once

#define WINCX			1024
#define WINCY			768
#define MAPCX			(WINCX * 2)
#define MAPCY			(WINCY * 2)

#define	PURE			= 0

#define	OBJ_NOEVENT		0
#define	OBJ_DEAD		1

#define PCT_NONE		2
#define PCT_X			3
#define PCT_Y			4

#define PI				3.141592f
#define	VK_MAX			0xff

#define	TILECX			16
#define	TILECY			16

#define	TILEX			(MAPCX / TILECX)
#define	TILEY			(MAPCY / TILECY)

#define OBJ_TILEX		int(m_tInfo.fCX / TILECX)
#define OBJ_TILEY		int(m_tInfo.fCY / TILECY)

class CObj;

extern HWND		g_hWnd;
extern float	g_fVolume;
extern bool		g_bESC;
extern bool		g_bInteraction;
extern bool		g_bShop;

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

enum OBJID
{
	FURNITURE,
	MONSTER_BULLET,
	PLAYER_BULLET,
	MONSTER,
	BOSS,
	MINION,
	NPC,
	PLAYER,
	OBJ_END
};

enum ITEMID
{
	IT_BLOCK,
	IT_FURNITURE,
	IT_NONCONSUMABLE,
	IT_CONSUMABLE,
	IT_WEAPON,
	IT_ARMOR,
	IT_END
};

enum UIID
{
	UIM_ICON,
	UIM_BUTTON_START,
	UIM_BUTTON_EDIT,
	UIM_BUTTON_EXIT,
	UIS_HPBAR,
	UIS_MESSAGEBOX,
	UIS_NPCBUTTON,
	UIS_INVENBAG,
	UIS_INVENSHOP,
	UIS_QUESTBOX,
	UIS_INVENEQUIP,
	UIS_CRAFT,
	UIS_BOSSHPBAR,
	UIS_TEXT,
	UIS_PLAYERDEADTEXT,
	UI_END
};

enum EFFECTID
{
	EF_PLAYERDEAD,
	EF_MONSTERDEAD,
	EF_BOSSDEAD,
	EF_GUIDEDEAD,
	EF_DYNAMITE,
	EF_END
};

enum CHANNELID
{
	SOUND_BGM,
	SOUND_EFFECT1,	// Player
	SOUND_EFFECT2,	// Monster
	SOUND_EFFECT3,	// Weapon-Tool
	SOUND_EFFECT4,	// Weapon
	SOUND_EFFECT5,	// Block, Furniture, Consumable
	SOUND_EFFECT6,	// UI
	SOUND_EFFECT7,	// NPC
	SOUND_EFFECT8,	// Weapon Sound Effect
	SOUND_EFFECT9,	// Weapon Sound Effect
	SOUND_UI,
	MAXCHANNEL
};

enum TIMERID
{
	TM_GRAVITY,
	TM_INVINCIBLE,
	TM_USE,
	TM_PATTERN,
	TM_BUFF,
	TM_RESPAWN,
	TM_END
};

typedef enum CollisionType
{
	CD_LEFT,
	CD_TOP,
	CD_RIGHT,
	CD_BOTTOM,
	CD_END
}COLTYPE;

typedef struct tagInfo
{
	float fX;
	float fY;
	float fCX;
	float fCY;

	tagInfo() { ZeroMemory(this, sizeof(INFO)); }
	tagInfo(float _fX, float _fY, float _fCX, float _fCY)
	: fX(_fX), fY(_fY), fCX(_fCX), fCY(_fCY) { }

}INFO;

typedef struct tagLinePoint
{
	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

	float	fX;
	float	fY;

}LINEPOINT;

typedef struct tagLine
{
	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(LINEPOINT& tLeftPoint, LINEPOINT& tRightPoint)
		: tLeft(tLeftPoint), tRight(tRightPoint) {}

	LINEPOINT		tLeft;
	LINEPOINT		tRight;

}LINEINFO;

typedef struct tagFrame
{
	int		iStart;
	int		iEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;

}FRAME;

typedef struct tagCollisionMsg
{
	CObj*		pObj;
	COLTYPE		eColType;
	float		diffX;
	float		diffY;
	bool		bFunction;

public:
	tagCollisionMsg() { ZeroMemory(this, sizeof(COLMSG)); }

	tagCollisionMsg(CObj* _pObj, COLTYPE _eColType, float _diffX, float _diffY, bool _bFunction = false)
		: pObj(_pObj), eColType(_eColType), diffX(_diffX), diffY(_diffY), bFunction(_bFunction) {};
}COLMSG;

typedef struct tagTileInfo
{
	// 블록 정보, 0일 시 없음
	int			iBlock;
	int			iWall;
	int			iRope;

	// 옆 타일에 동일한 블록 존재 여부
	bool		bLeft;
	bool		bTop;
	bool		bRight;
	bool		bBottom;

	tagTileInfo() { ZeroMemory(this, sizeof(tagTileInfo)); }
}TILEINFO;

typedef struct tagItemInfo
{
	ITEMID	eItemID;
	UINT	iItemSubID;
	UINT	iItemCode;
	UINT	iCount;

	tagItemInfo() { ZeroMemory(this, sizeof(ITEMINFO)); }
	tagItemInfo(ITEMID _eID, UINT _iSubID, UINT _iCode, UINT _iCount)
		: eItemID(_eID), iItemSubID(_iSubID), iItemCode(_iCode), iCount(_iCount) {}
	tagItemInfo(UINT _eID, UINT _iSubID, UINT _iCode, UINT _iCount)
		: eItemID(ITEMID(_eID)), iItemSubID(_iSubID), iItemCode(_iCode), iCount(_iCount) {}

	bool operator<(const tagItemInfo& other) const
	{
		if (eItemID != other.eItemID)		return eItemID < other.eItemID;
		if (iItemSubID != other.iItemSubID)	return iItemSubID < other.iItemSubID;
		if (iItemCode != other.iItemCode)	return iItemCode < other.iItemCode;
		return iCount < other.iCount;
	}

	bool operator!=(const tagItemInfo& other) const
	{
		if ((eItemID != other.eItemID) ||
			(iItemSubID != other.iItemSubID) ||
			(iItemCode != other.iItemCode) ||
			(iCount != other.iCount)) return true;
		else return false;
	}
}ITEMINFO;

typedef struct tagItemKey
{
	ITEMID	eItemID;
	UINT	iItemSubID;
	UINT	iItemCode;

	tagItemKey() { ZeroMemory(this, sizeof(ITEMKEY)); }
	tagItemKey(ITEMID _eID, UINT _iSubID, UINT _iCode)
		: eItemID(_eID), iItemSubID(_iSubID), iItemCode(_iCode) {}

	bool operator<(const tagItemKey& other) const
	{
		if (eItemID != other.eItemID)		return eItemID < other.eItemID;
		if (iItemSubID != other.iItemSubID)	return iItemSubID < other.iItemSubID;
		return iItemCode < other.iItemCode;
	}
}ITEMKEY;

typedef struct tagEffectKey
{
	EFFECTID	eEffectID;
	int			iEffectCode;

	tagEffectKey() { ZeroMemory(this, sizeof(EFFECTKEY)); }
	tagEffectKey(EFFECTID _eID, int _iCode)
		: eEffectID(_eID), iEffectCode(_iCode) {}

	bool operator<(const tagEffectKey& other) const
	{
		if (eEffectID != other.eEffectID) return eEffectID < other.eEffectID;
		
		return iEffectCode < other.iEffectCode;
	}
}EFFECTKEY;