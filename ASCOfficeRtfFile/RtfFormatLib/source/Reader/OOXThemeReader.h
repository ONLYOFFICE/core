#pragma once
#include "OOXColorSchemeReader.h"
#include "OOXFontSchemeReader.h"
#include "../Utils.h"

class OOXThemeReader
{
private: 
	OOX::CTheme* m_ooxTheme;

public: 
	OOXThemeReader( OOX::CTheme* ooxTheme)
	{
		m_ooxTheme = ooxTheme;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxTheme == NULL) return false;

		{
			OOXColorSchemeReader oColorSchemeReader (&m_ooxTheme->m_oThemeElements.m_oClrScheme);
			oColorSchemeReader.Parse( oParam );
		}
		{
			OOXFontSchemeReader oFontSchemeReader (&m_ooxTheme->m_oThemeElements.m_oFontScheme );
			oFontSchemeReader.Parse( oParam );
		}
		return true;
	}
};
