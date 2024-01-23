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

		bool LoadFromFile(const std::wstring& wsFile, CGraphicsContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContentent, CGraphicsContainer* pContainer, CSvgFile* pFile) const;
		bool LoadFromXmlNode(XmlUtils::CXmlNode& oElement, CGraphicsContainer* pContainer, CSvgFile* pFile) const;

		template <class ObjectType>
		bool ReadObject(XmlUtils::CXmlNode& oElement, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;
	private:
		template <class ObjectType>
		bool ReadChildrens(XmlUtils::CXmlNode& oElement, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;

		bool ScanStyles(XmlUtils::CXmlNode& oElement, CSvgFile* pFile) const;

		void UpdateStyles(CObject* pObject, CSvgFile* pFile) const;
		bool MarkObject(CObject* pObject, CSvgFile* pFile) const;
		template <class ObjectType>
		bool AddObject(ObjectType* pObject, CContainer<ObjectType>* pContainer) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
