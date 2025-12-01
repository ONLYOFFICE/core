#ifndef CPATTERN_H
#define CPATTERN_H

#include "CContainer.h"
#include "../../../../graphics/pro/Fonts.h"

namespace SVG
{
	typedef enum
	{
		objectBoundingBox,
		userSpaceOnUse
	} PatternUnits;

	class CPattern : public CAppliedObject
	{
		friend class CObject;
		CPattern(CSvgReader& oReader, NSFonts::IFontManager *pFontManager = NULL);
	public:
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
