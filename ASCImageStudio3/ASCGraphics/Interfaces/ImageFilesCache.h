#pragma once
#include "..\stdafx.h"

#include "..\agg\AVSWrapper\Image.h"
#include "TemporaryCS.h"

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
	CCacheImage(CString& strFile) : m_oImage(strFile)
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
	CAtlMap<CString, CCacheImage*> m_mapImages;
	LONG m_lMaxCount;

	LONG m_lRef;

	CRITICAL_SECTION m_oCS;

public:
	CImageFilesCache()
	{
		m_lMaxCount = 10;
		m_lRef = 1;

		InitializeCriticalSection(&m_oCS);
	}

	~CImageFilesCache()
	{
		Clear();
		DeleteCriticalSection(&m_oCS);
	}

	void Clear()
	{
		CTemporaryCS oCS(&m_oCS);
		POSITION pos = m_mapImages.GetStartPosition();
		while (NULL != pos)
		{
			CCacheImage* pImage = m_mapImages.GetNextValue(pos); 
			pImage->Release();
		}

		m_mapImages.RemoveAll();
	}

	CCacheImage* Lock(CString& strFile)
	{
		CTemporaryCS oCS(&m_oCS);

		CAtlMap<CString, CCacheImage*>::CPair* pPair = m_mapImages.Lookup(strFile);
		if (NULL != pPair)
		{
			CCacheImage* pImage = pPair->m_value;
			pImage->AddRef();
			return pImage;
		}

		if ((LONG)m_mapImages.GetCount() >= m_lMaxCount)
		{
			LONG lNeedDelete = (LONG)m_mapImages.GetCount() - m_lMaxCount + 1;

			POSITION pos = m_mapImages.GetStartPosition();
			while ((NULL != pos) && (lNeedDelete > 0))
			{
				POSITION posRemove = pos;				
				CCacheImage* pImage = m_mapImages.GetNextValue(pos);
				m_mapImages.RemoveAtPos(posRemove);
				--lNeedDelete;
				pImage->Release();
			}
		}

		CCacheImage* pImage = new CCacheImage(strFile);
		m_mapImages.SetAt(strFile, pImage);

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
		--m_lRef;

		if (0 == m_lRef)
		{
			delete this;
			return 0;
		}

		return m_lRef;
	}
};