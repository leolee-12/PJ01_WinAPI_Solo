#pragma once
#include "Define.h"

class CSoundMgr
{
private:
	CSoundMgr();
	CSoundMgr(const CSoundMgr& rhs) = delete;		// 삭제
	CSoundMgr& operator=(CSoundMgr& rObj) = delete;	// 삭제
	~CSoundMgr();

public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

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
	void Initialize();
	void Release();

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlaySoundQuick(const TCHAR* pSoundKey, CHANNELID eID, float fVolume, int iRepeatCount, int iDelayMs);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

	const bool IsPlayingBGM() const { return m_bPlayingBGM; }

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

	bool	m_bPlayingBGM;
};

