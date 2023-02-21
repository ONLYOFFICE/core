#ifndef CPATTERN_H
#define CPATTERN_H

#include "CDefs.h"

namespace SVG
{
	typedef enum
	{
		objectBoundingBox,
		userSpaceOnUse
	} PatternUnits;

	class CPattern : public CGraphicsContainer, public CDefObject
	{
	public:
		CPattern(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);
		virtual ~CPattern();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds) override;
	private:
		void Update(const CDefs *pDefs, const TBounds &oObjectBounds);

		NSFonts::IFontManager *m_pFontManager;

		Aggplus::CImage *m_pImage;

		PatternUnits m_enPatternUnits;
	};
}

#endif // CPATTERN_H
