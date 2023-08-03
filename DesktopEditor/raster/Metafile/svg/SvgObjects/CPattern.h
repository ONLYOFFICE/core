#ifndef CPATTERN_H
#define CPATTERN_H

#include "CContainer.h"

namespace SVG
{
	typedef enum
	{
		objectBoundingBox,
		userSpaceOnUse
	} PatternUnits;

	class CPattern : public CAppliedObject
	{
	public:
		CPattern(XmlUtils::CXmlNode& oNode, NSFonts::IFontManager *pFontManager = NULL);
		virtual ~CPattern();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CSvgFile* pFile, const TBounds &oObjectBounds) override;

		CGraphicsContainer& GetContainer();
	private:
		void Update(const CSvgFile* pFile, const TBounds &oObjectBounds);

		CGraphicsContainer m_oContainer;

		NSFonts::IFontManager *m_pFontManager;

		Aggplus::CImage *m_pImage;

		PatternUnits m_enPatternUnits;
	};
}

#endif // CPATTERN_H
