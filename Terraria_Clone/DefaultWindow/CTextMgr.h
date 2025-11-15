#pragma once
class CTextMgr
{
public:
	// 农扁 喊肺 迄飘 积己 格利
	enum FONTID { FONT_16, FONT_24, FONT_40, FONT_END};

public:
	CTextMgr();
	CTextMgr(const CTextMgr& rhs) = delete;
	CTextMgr& operator=(CTextMgr& rhs) = delete;
	~CTextMgr();

public:
	static CTextMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTextMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Initialize();
	void	Release();

	void	Set_Font(HDC hDC, FONTID eID);
	void	Unset_Font(HDC hDC);

private:
	static CTextMgr*	m_pInstance;
	HFONT				m_hOldFont;
	HFONT				m_hFont[FONT_END];
};