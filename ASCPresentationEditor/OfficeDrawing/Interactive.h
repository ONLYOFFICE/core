#pragma once
#include "Attributes.h"

namespace NSPresentationEditor
{
	class CTextRange
	{
	public:
		LONG m_lStart;
		LONG m_lEnd;
	
	public:
		CTextRange()
		{
			m_lStart	= 0;
			m_lEnd		= 0;
		}
		CTextRange& operator=(const CTextRange& oSrc)
		{
			m_lStart	= oSrc.m_lStart;
			m_lEnd		= oSrc.m_lEnd;

			return *this;
		}
		CTextRange(const CTextRange& oSrc)
		{
			*this = oSrc;
		}
	};

	class CInteractiveInfo
	{
	private:
		long	m_lType;
		VARIANT m_varParameter;
		
	public:
		bool	m_bPresent;

		CAtlArray<CTextRange> m_arRanges;

	public:
		CInteractiveInfo()
		{
			m_bPresent = false;
		}
		~CInteractiveInfo()
		{
		}

		CInteractiveInfo& operator=(const CInteractiveInfo& oSrc)
		{
			m_lType			= oSrc.m_lType;
			m_varParameter	= oSrc.m_varParameter;

			m_bPresent		= oSrc.m_bPresent;

			m_arRanges.Copy(oSrc.m_arRanges);

			return *this;
		}
		CInteractiveInfo(const CInteractiveInfo& oSrc)
		{
			*this = oSrc;
		}
	};
}