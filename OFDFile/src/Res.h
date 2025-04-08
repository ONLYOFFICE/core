#ifndef RES_H
#define RES_H

#include <string>

#include "Types/ColorSpace.h"
#include "Types/DrawParam.h"
#include "Types/Font.h"
#include "Types/MultiMedia.h"
#include "Types/CompositeGraphicUnit.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"

namespace OFD
{
class CRes
{
	std::map<unsigned int, CColorSpace*>           m_mColorSpaces;
	std::map<unsigned int, CDrawParam*>            m_mDrawParams;
	std::map<unsigned int, CFont*>                 m_mFonts;
	std::map<unsigned int, CMultiMedia*>           m_mMultiMedias;
	std::map<unsigned int, CCompositeGraphicUnit*> m_mCCompositeGraphicUnits;
public:
	CRes();
	~CRes();

	bool Read(const std::wstring& wsFilePath);

	const CColorSpace*           GetColorSpace(unsigned int unId) const;
	const CDrawParam*            GetDrawParam(unsigned int unId) const;
	const CFont*                 GetFont(unsigned int unId) const;
	const CMultiMedia*           GetMultiMedia(unsigned int unId) const;
	const CCompositeGraphicUnit* GetCompositeGraphicUnit(unsigned int unId) const;
};
}

#endif // RES_H
