#ifndef CMASK_H
#define CMASK_H

#include "CClipPath.h"

namespace SVG
{
	class CMask : public CClipPath
	{
	public:
		CMask(XmlUtils::CXmlNode& oNode);

		bool Apply(IRenderer* pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) override;
	};
}

#endif // CMASK_H
