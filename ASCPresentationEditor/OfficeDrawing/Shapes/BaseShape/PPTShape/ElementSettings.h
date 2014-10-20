#pragma once
#include "../../../../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "Enums.h"

// это класс, использующийся для передачи свойств объектов,
// например - указатель на картинку... (по PID'у)

class CProperty
{
public:
	NSOfficeDrawing::PropertyId m_ePID;

	bool m_bIsBlip;
	bool m_bComplex;

	// само значение
	DWORD m_lValue;

	// чтобы не плодить классы - это value, когда m_bComplex == true
	BYTE* m_pOptions;

	BOOL m_bIsTruncated;

public:
	CProperty()
	{
		m_ePID = NSOfficeDrawing::left;
		m_bIsBlip = false;
		m_bComplex = false;
		m_lValue = 0;
		m_pOptions = NULL;

		m_bIsTruncated = FALSE;
	}
	~CProperty()
	{
		RELEASEARRAYOBJECTS(m_pOptions);
	}

	void FromStream(IStream* pStream)
	{
		// читаем из стрима...
		// только пока без учета bComplex
		// т.к. Complex - учитывается в контейнере, хранящем все проперти
		USHORT lMem = StreamUtils::ReadWORD(pStream);
		m_ePID = (NSOfficeDrawing::PropertyId)(lMem & 0x3FFF);
		
		m_bIsBlip = ((lMem & 0x4000) == 0x4000);
		m_bComplex = ((lMem & 0x8000) == 0x8000);

		m_lValue = StreamUtils::ReadDWORD(pStream);
	}

	void ComplexFromStream(IStream* pStream)
	{
		if (m_bComplex && 0 != m_lValue)
		{
			if (NSOfficeDrawing::dgmConstrainBounds == m_ePID ||
				NSOfficeDrawing::fillShadeColors == m_ePID ||
				NSOfficeDrawing::lineDashStyle == m_ePID ||
				NSOfficeDrawing::pAdjustHandles == m_ePID ||
				NSOfficeDrawing::pConnectionSites == m_ePID ||
				NSOfficeDrawing::pConnectionSitesDir == m_ePID ||
				NSOfficeDrawing::pInscribe == m_ePID ||
				NSOfficeDrawing::pSegmentInfo == m_ePID ||
				NSOfficeDrawing::pVertices == m_ePID ||
				NSOfficeDrawing::pGuides == m_ePID ||
				NSOfficeDrawing::pWrapPolygonVertices == m_ePID ||
				NSOfficeDrawing::pRelationTbl == m_ePID ||
				NSOfficeDrawing::tableRowProperties == m_ePID ||
				NSOfficeDrawing::lineLeftDashStyle == m_ePID ||
				NSOfficeDrawing::lineTopDashStyle == m_ePID ||
				NSOfficeDrawing::lineRightDashStyle == m_ePID ||
				NSOfficeDrawing::lineBottomDashStyle == m_ePID)
			{
				WORD nElems = StreamUtils::ReadWORD(pStream);
				WORD nElemsAlloc = StreamUtils::ReadWORD(pStream);
				WORD nElemSize = StreamUtils::ReadWORD(pStream);

				if (0xFFF0 == nElemSize)
				{
					nElemSize = 4;
					m_bIsTruncated = TRUE;
				}

				LONG dwSize = nElems * nElemSize;			

				if (m_lValue != (dwSize + 6))
				{
					BOOL b = FALSE;
				}

				m_lValue = dwSize;
			}
			
			if (0 == m_lValue)
			{
				return;
			}

			RELEASEARRAYOBJECTS(m_pOptions);
			m_pOptions = new BYTE[m_lValue];

			ULONG lReadBytes = 0;
			pStream->Read(m_pOptions, m_lValue, &lReadBytes);
			if (lReadBytes != m_lValue)
			{
				return;
			}
		}
	}

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("%d,%d,%d,%d"), (long)m_ePID, (long)m_bIsBlip, (long)m_bComplex, (long)m_lValue);
		// не будем ничего писать - а то xml - не распознаются
		/*if (m_bComplex)
		{
			CString strProp = CDirectory::BYTEArrayToString(m_pOptions, m_lValue);
			if (NSOfficeDrawing::pibName == m_ePID)
			{
				strProp = (CString)CDirectory::BYTEArrayToStringW(m_pOptions, m_lValue);
			}
			str += _T(" options=") + strProp;
		}*/
		return _T("<Property command='") + str + _T("'/>");
	}
};

// А вот контейнер пропертей
class CProperties
{
public:
    std::vector<CProperty> m_arProperties;
	// по идее - это instance, но нам так удобнее,
	// тем более это класс - не связанный с RecordHeader
	long m_lCount;

public:
	CProperties() : m_arProperties()
	{
	}
	~CProperties()
	{
		m_lCount = 0;
        m_arProperties.clear();
	}
	
	void FromStream(IStream* pStream, long lCount)
	{
		m_lCount = lCount;
		for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
		{
            CProperty elem;
            m_arProperties.push_back(elem);
			m_arProperties[lIndex].FromStream(pStream);
		}
		// теперь читаем дополнительную информацию 
		// сортировано по pid'ам (но у нас пока просто по-порядку)
		for (long lIndex = 0; lIndex < m_lCount; ++lIndex)
		{
			m_arProperties[lIndex].ComplexFromStream(pStream);
		}
	}

	CString ToString()
	{
		CString str = _T("");
        for (size_t nIndex = 0; nIndex < m_arProperties.size(); ++nIndex)
		{
			str += m_arProperties[nIndex].ToString();
		}
		return _T("<Properties>") + str + _T("</Properties>");
	}

	DWORD GetLen()
	{
		DWORD dwLen = 6 * m_lCount;
		for (long nIndex = 0; nIndex < m_lCount; ++nIndex)
		{
			if (m_arProperties[nIndex].m_bComplex)
			{
				dwLen += m_arProperties[nIndex].m_lValue;
			}
		}
		return dwLen;
	}
};
