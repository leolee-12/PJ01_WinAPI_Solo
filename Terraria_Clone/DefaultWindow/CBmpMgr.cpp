#include "pch.h"
#include "CBmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

HDC CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](auto& Pair)-> bool
		{
			if (!lstrcmp(pImgKey, Pair.first))
				return true;

			return false;
		});

	if (iter == m_mapBit.end())
		return nullptr;


	return iter->second->Get_MemDC();
}

void CBmpMgr::Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	auto iter = find_if(m_mapBit.begin(), m_mapBit.end(), [&](auto& Pair)-> bool
		{
			if (!lstrcmp(pImgKey, Pair.first))
				return true;

			return false;
		});

	if (m_mapBit.end() == iter)
	{
		CMyBmp* pMyBmp = new CMyBmp;

		pMyBmp->Load_Bmp(pFilePath);

		m_mapBit.insert({ pImgKey, pMyBmp });
	}
}

void CBmpMgr::Release()
{
	for_each(m_mapBit.begin(), m_mapBit.end(), [](auto& Pair)
		{
			if (nullptr != Pair.second)
			{
				delete Pair.second;
				Pair.second = nullptr;
			}
		});

	m_mapBit.clear();
}
