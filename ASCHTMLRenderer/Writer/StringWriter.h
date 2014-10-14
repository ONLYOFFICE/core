#pragma once
#include "../../DesktopEditor/common/File.h"
#include "TextItem.h"
#include "Common.h"

class CDstInfo
{
public:
	CString m_strDstFilePath;
	CString m_strAdditionalPath;
	CString m_strDstMedia;
	BOOL	m_bIsWeb;

public:
	CDstInfo()
	{
		m_strDstFilePath	= _T("");
		m_strAdditionalPath = _T("");
		m_strDstMedia		= _T("");
		m_bIsWeb			= false;
	}
	CDstInfo(const CDstInfo& oInfo)
	{
		*this = oInfo;
	}
	CDstInfo& operator=(const CDstInfo& oSrc)
	{
		m_strDstFilePath	= oSrc.m_strDstFilePath;
		m_strAdditionalPath = oSrc.m_strAdditionalPath;
		m_strDstMedia		= oSrc.m_strDstMedia;
		m_bIsWeb			= oSrc.m_bIsWeb;
		return *this;
	}
};

namespace NSStrings
{
	class CStringWriter : public CTextItem
	{
	public:
		CStringWriter() : CTextItem()
		{
		}
		virtual ~CStringWriter()
		{
		}

	public:
		
		AVSINLINE void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			CTextItem::WriteString(pString, nLen);
		}
		AVSINLINE void Write(CStringWriter& oWriter)
		{
			CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}
	};
}