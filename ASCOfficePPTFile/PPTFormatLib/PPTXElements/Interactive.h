#pragma once
#include "Attributes.h"

#if !defined(_WIN32) && !defined(_WIN64)
    #include "../../DesktopEditor/common/ASCVariant.h"
#endif

namespace NSPresentationEditor
{
	class CTextRange
	{
	public:
		int m_lStart;
		int m_lEnd;
	
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

		std::vector<CTextRange> m_arRanges;

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

			m_arRanges.insert(m_arRanges.end(), oSrc.m_arRanges.begin(), oSrc.m_arRanges.end());

			return *this;
		}
		CInteractiveInfo(const CInteractiveInfo& oSrc)
		{
			*this = oSrc;
		}
	};
}
