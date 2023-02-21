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

		bool LoadFromFile(const std::wstring& wsFile, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContente, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;
		bool LoadFromXmlNode(XmlUtils::CXmlNode& oElement, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;

		bool ReadElement(XmlUtils::CXmlNode& oElement, CGraphicsContainer* pContainer, CSvgFile* pFile) const;

		bool ReadDefs(XmlUtils::CXmlNode& oElement, CDefs* pDefs, CSvgFile* pFile) const;
	private:
		template <typename TypeContainer>
		bool ReadChildrens(XmlUtils::CXmlNode& oElement, CContainer<TypeContainer>* pContainer, CSvgFile* pFile) const;

		bool ScanElement(XmlUtils::CXmlNode& oElement, const std::wstring& wsElementName, CSvgFile* pFile) const;

		bool IsDefs(const std::wstring& wsNodeName) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
