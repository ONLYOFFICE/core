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

	class CPattern : public CContainer, public IDefObject
	{
	public:
		CPattern(CObjectBase* pParent = NULL, NSFonts::IFontManager *pFontManager = NULL);
		virtual ~CPattern();

		void SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode) override;

		bool Apply(IRenderer* pRenderer, CDefs *pDefs, const TBounds &oObjectBounds) override;
	private:
		void Update(CDefs *pDefs, const TBounds &oObjectBounds);

		NSFonts::IFontManager *m_pFontManager;

		Aggplus::CImage *m_pImage;

		PatternUnits m_enPatternUnits;
	};
}

#endif // CPATTERN_H
