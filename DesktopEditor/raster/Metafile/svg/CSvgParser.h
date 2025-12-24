#ifndef CSVGPARSER_H
#define CSVGPARSER_H

#include "../../../common/Directory.h"
#include "../../../graphics/pro/Fonts.h"

#include "SvgObjects/CContainer.h"
#include "SvgReader.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser
	{
	public:
		CSvgParser(NSFonts::IFontManager* pFontManager = NULL);
		~CSvgParser();

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool LoadFromFile(const std::wstring& wsFile, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContent, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;

		template <class ObjectType>
		bool ReadObject(CSvgReader& oReader, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;
	private:
		template <class ObjectType>
		bool ReadChildrens(CSvgReader& oReader, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;

		bool ScanStyles(CSvgReader& oReader, CSvgFile* pFile) const;
		void ParseStyles(const std::wstring& wsStyles, CSvgFile *pFile) const;

		template <class ObjectType>
		bool AddObject(ObjectType* pObject, CContainer<ObjectType>* pContainer) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
