#pragma once
#include "..\..\DocumentViewer\DIB.h"
#include "EMFInterpreter.h"
#include "..\..\Objects\AVSFontManager.h"
#include "TemporaryCS.h"

class CEMFFile
{
public:
	CRITICAL_SECTION m_oCS;

	CEMFInterpreter m_oEMF;

	HENHMETAFILE	m_hEmf;
	HDC				m_memDC;

	LONG			m_lObjects;

public:
	CEMFFile()
	{
		m_hEmf		= NULL;
		m_memDC		= NULL;
		m_lObjects	= 0;

		InitializeCriticalSection(&m_oCS);
	}
	~CEMFFile()
	{
		DeleteCriticalSection(&m_oCS);
	}

	inline void SetFontManager(IAVSFontManager* pManager)
	{
		m_oEMF.SetFontManager(pManager);
	}

	void Open(BSTR bsFileName)
	{
		HDC hDC = GetDC(NULL);
		m_memDC = CreateCompatibleDC(hDC);

		ReleaseDC(NULL, hDC);

		m_hEmf = GetEnhMetaFile(bsFileName);

		ENHMETAHEADER EmfHeader;
		if (::GetEnhMetaFileHeader(m_hEmf, sizeof(ENHMETAHEADER), &EmfHeader))
		{
			m_lObjects = EmfHeader.nHandles;
		} 

		m_oEMF.Init(m_memDC, (UINT)m_lObjects);
	}

	void Close()
	{
		m_oEMF.SetRenderer(NULL);
		if (NULL != m_hEmf)
		{
			DeleteEnhMetaFile(m_hEmf);
			m_hEmf = NULL;
		}
		if (NULL != m_memDC)
		{
			DeleteDC(m_memDC);
		}
	}

	void DrawOnRenderer(IAVSRenderer* pRenderer, double x, double y, double width, double height)
	{
		CTemporaryCS oCS (&m_oCS);
		
		if (NULL == pRenderer)
			return;

		m_oEMF.SetRenderer(pRenderer);
		m_oEMF.SetRectDst(x, y, width, height);

		RECT oRect;
		oRect.left		= 0;
		oRect.top		= 0;
		oRect.right		= 100;
		oRect.bottom	= 100;

		int nEmumRes = ::EnumEnhMetaFile(m_memDC, m_hEmf, (ENHMFENUMPROC)EnumEMFCallbak, (void*)this, &oRect);
	}

protected:
	static int CALLBACK EnumEMFCallbak(HDC hDC, HANDLETABLE FAR *lpHandleTable, ENHMETARECORD FAR *lpEnhMetaRecord,	
							 int nObj, LPARAM lpData)
	{
		CEMFFile* pFile = (CEMFFile*)lpData;
		
		pFile->m_oEMF.SetRecord(lpEnhMetaRecord);
		pFile->m_oEMF.SetTable(lpHandleTable);

		pFile->m_oEMF.ProduceRecord();
		
		return 1;
	}
};