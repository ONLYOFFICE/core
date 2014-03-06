#ifndef _BUILD_IMAGEFILESCACHE_H_
#define _BUILD_IMAGEFILESCACHE_H_

#include "Image.h"
#include "TemporaryCS.h"
#include <map>

class CCacheImage
{
private:
	Aggplus::CImage m_oImage;
	LONG m_lRef;

public:
	CCacheImage() : m_oImage()
	{
		m_lRef = 1;		
	}
	CCacheImage(const std::wstring& strFile) : m_oImage(strFile)
	{
		m_lRef = 1;
	}

	LONG AddRef()
	{
		++m_lRef;
		return m_lRef;
	}
	LONG Release()
	{
		--m_lRef;

		if (0 == m_lRef)
		{
			delete this;
			return 0;
		}

		return m_lRef;
	}
	
	Aggplus::CImage* GetImage()
	{
		return &m_oImage;
	}
};

class CImageFilesCache
{
private:
	std::map<std::wstring, CCacheImage*> m_mapImages;
	LONG m_lMaxCount;

	LONG m_lRef;

	NSCriticalSection::CRITICAL_SECTION m_oCS;

public:
	CImageFilesCache()
	{
		m_lMaxCount = 10;
		m_lRef = 1;

		m_oCS.InitializeCriticalSection();
	}

	~CImageFilesCache()
	{
		Clear();
		m_oCS.DeleteCriticalSection();
	}

	void Clear()
	{
		CTemporaryCS oCS(&m_oCS);

		for (std::map<std::wstring,CCacheImage*>::iterator it = m_mapImages.begin(); it != m_mapImages.end(); ++it)
		{
			it->second->Release();
		}

		m_mapImages.clear();
	}

	CCacheImage* Lock(const std::wstring& strFile)
	{
		CTemporaryCS oCS(&m_oCS);

		std::map<std::wstring,CCacheImage*>::iterator it = m_mapImages.find(strFile);
		if (it != m_mapImages.end())
		{
			CCacheImage* pImage = it->second;
			pImage->AddRef();
			return pImage;
		}

		int nCount = (int)m_mapImages.size();
		if (nCount >= m_lMaxCount)
		{
			int nNeedDelete = nCount - m_lMaxCount;
			for (std::map<std::wstring,CCacheImage*>::iterator it2 = m_mapImages.begin(); it2 != m_mapImages.end(); ++it2)
			{
				if (nNeedDelete == 0)
					break;

				it2->second->Release();
				m_mapImages.erase(it2);
			}
		}

		CCacheImage* pImage = new CCacheImage(strFile);
		m_mapImages[strFile] = pImage;

		pImage->AddRef();
		return pImage;
	}
	
	LONG AddRef()
	{
		++m_lRef;
		return m_lRef;
	}
	LONG Release()
	{
		m_oCS.Enter();
		--m_lRef;

		if (0 == m_lRef)
		{
			m_oCS.Leave();
			delete this;
			return 0;
		}

		m_oCS.Leave();
		return m_lRef;
	}
};

#endif // _BUILD_IMAGEFILESCACHE_H_