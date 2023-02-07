#ifndef CSVGPARSER_H
#define CSVGPARSER_H

#include "../../graphics/pro/Fonts.h"
#include "../../../common/Directory.h"
#include "../../../xml/include/xmlutils.h"

#include "SvgObjects/CContainer.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser
	{
	public:
		CSvgParser();
		~CSvgParser();

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool LoadFromFile(const std::wstring& wsFile, CContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContente, CContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromXmlNode(XmlUtils::CXmlNode& oElement, CContainer* pContainer, CSvgFile* pFile) const;

		void AddStyle(const std::wstring& wsStyle);
	private:
		bool ReadElement(XmlUtils::CXmlNode& oElement, CContainer* pContainer, CSvgFile* pFile) const;
		bool ReadChildrens(XmlUtils::CXmlNode& oElement, CContainer* pContainer, CSvgFile* pFile) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
